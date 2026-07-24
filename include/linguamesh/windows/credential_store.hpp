#pragma once

#include <string>
#include <vector>

namespace linguamesh::windows {

struct SecretRef {
    std::string id;
};

enum class CredentialError {
    None,
    EmptyValue,
    SecureStorageUnavailable,
    NotFound,
};

struct CredentialResult {
    SecretRef reference;
    std::string value;
    CredentialError error = CredentialError::None;

    [[nodiscard]] bool ok() const noexcept {
        return error == CredentialError::None;
    }
};

class CredentialStore {
public:
    virtual ~CredentialStore() = default;

    [[nodiscard]] virtual CredentialResult save(const std::string& profile_id,
                                                const std::string& secret) = 0;
    [[nodiscard]] virtual CredentialResult resolve(const SecretRef& reference) const = 0;
    virtual CredentialError remove(const SecretRef& reference) = 0;
};

class MemoryCredentialStore final : public CredentialStore {
public:
    [[nodiscard]] CredentialResult save(const std::string& profile_id,
                                       const std::string& secret) override;
    [[nodiscard]] CredentialResult resolve(const SecretRef& reference) const override;
    CredentialError remove(const SecretRef& reference) override;

private:
    struct Entry {
        std::string id;
        std::string value;
    };
    std::vector<Entry> entries_;
};

class WindowsCredentialStore final : public CredentialStore {
public:
    [[nodiscard]] CredentialResult save(const std::string& profile_id,
                                       const std::string& secret) override;
    [[nodiscard]] CredentialResult resolve(const SecretRef& reference) const override;
    CredentialError remove(const SecretRef& reference) override;
};

}
