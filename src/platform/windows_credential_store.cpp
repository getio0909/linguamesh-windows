#include "linguamesh/windows/credential_store.hpp"

#ifdef _WIN32

#include <winrt/Windows.Security.Credentials.h>
#include <winrt/base.h>

namespace linguamesh::windows {

CredentialResult WindowsCredentialStore::save(const std::string& profile_id,
                                              const std::string& secret) {
    if (profile_id.empty() || secret.empty()) {
        return {{}, {}, CredentialError::EmptyValue};
    }
    try {
        winrt::Windows::Security::Credentials::PasswordVault vault;
        const auto resource = winrt::to_hstring("LinguaMesh/" + profile_id);
        const auto username = winrt::to_hstring(profile_id);
        const auto password = winrt::to_hstring(secret);
        try {
            vault.Remove(vault.Retrieve(resource, username));
        } catch (const winrt::hresult_error&) {
        }
        vault.Add(winrt::Windows::Security::Credentials::PasswordCredential(resource, username, password));
        return {{profile_id}, {}, CredentialError::None};
    } catch (const winrt::hresult_error&) {
        return {{}, {}, CredentialError::SecureStorageUnavailable};
    }
}

CredentialResult WindowsCredentialStore::resolve(const SecretRef& reference) const {
    if (reference.id.empty()) {
        return {{}, {}, CredentialError::NotFound};
    }
    try {
        winrt::Windows::Security::Credentials::PasswordVault vault;
        const auto credential = vault.Retrieve(winrt::to_hstring("LinguaMesh/" + reference.id),
                                               winrt::to_hstring(reference.id));
        credential.RetrievePassword();
        return {{reference.id}, winrt::to_string(credential.Password()), CredentialError::None};
    } catch (const winrt::hresult_error&) {
        return {{}, {}, CredentialError::NotFound};
    }
}

CredentialError WindowsCredentialStore::remove(const SecretRef& reference) {
    if (reference.id.empty()) {
        return CredentialError::NotFound;
    }
    try {
        winrt::Windows::Security::Credentials::PasswordVault vault;
        const auto credential = vault.Retrieve(winrt::to_hstring("LinguaMesh/" + reference.id),
                                               winrt::to_hstring(reference.id));
        vault.Remove(credential);
        return CredentialError::None;
    } catch (const winrt::hresult_error&) {
        return CredentialError::NotFound;
    }
}

}

#else

namespace linguamesh::windows {

CredentialResult WindowsCredentialStore::save(const std::string&, const std::string&) {
    return {{}, {}, CredentialError::SecureStorageUnavailable};
}

CredentialResult WindowsCredentialStore::resolve(const SecretRef&) const {
    return {{}, {}, CredentialError::SecureStorageUnavailable};
}

CredentialError WindowsCredentialStore::remove(const SecretRef&) {
    return CredentialError::SecureStorageUnavailable;
}

}

#endif
