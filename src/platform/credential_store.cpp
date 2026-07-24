#include "linguamesh/windows/credential_store.hpp"

#include <algorithm>
#include <vector>

namespace linguamesh::windows {

CredentialResult MemoryCredentialStore::save(const std::string& profile_id,
                                             const std::string& secret) {
    if (profile_id.empty() || secret.empty()) {
        return {{}, {}, CredentialError::EmptyValue};
    }
    const auto found = std::find_if(entries_.begin(), entries_.end(), [&](const Entry& entry) {
        return entry.id == profile_id;
    });
    if (found == entries_.end()) {
        entries_.push_back({profile_id, secret});
    } else {
        found->value = secret;
    }
    return {{profile_id}, {}, CredentialError::None};
}

CredentialResult MemoryCredentialStore::resolve(const SecretRef& reference) const {
    const auto found = std::find_if(entries_.begin(), entries_.end(), [&](const Entry& entry) {
        return entry.id == reference.id;
    });
    if (found == entries_.end()) {
        return {{}, {}, CredentialError::NotFound};
    }
    return {{reference.id}, found->value, CredentialError::None};
}

CredentialError MemoryCredentialStore::remove(const SecretRef& reference) {
    const auto old_size = entries_.size();
    entries_.erase(std::remove_if(entries_.begin(), entries_.end(), [&](const Entry& entry) {
                       return entry.id == reference.id;
                   }),
                   entries_.end());
    return entries_.size() == old_size ? CredentialError::NotFound : CredentialError::None;
}

}
