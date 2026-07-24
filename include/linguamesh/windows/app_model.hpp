#pragma once

#include "linguamesh/windows/core_bridge.hpp"
#include "linguamesh/windows/credential_store.hpp"

#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>

namespace linguamesh::windows {

struct AppSnapshot {
    TranslationStatus status = TranslationStatus::Idle;
    ErrorKind error = ErrorKind::None;
    std::string error_key;
    std::string operation_id;
    std::string selected_model;
    std::string locale = "en-US";
    std::string output;
    std::uint64_t last_sequence = 0;
};

class AppModel final {
public:
    AppModel(std::shared_ptr<CoreBridge> bridge,
             std::shared_ptr<CredentialStore> credential_store);
    ~AppModel();

    AppModel(const AppModel&) = delete;
    AppModel& operator=(const AppModel&) = delete;

    [[nodiscard]] std::vector<std::string> discover_models() const;
    bool select_model(const std::string& model_id);
    bool set_locale(const std::string& locale);
    [[nodiscard]] CredentialResult save_profile_secret(const std::string& profile_id,
                                                       const std::string& secret);
    void start_translation(const std::string& source_text);
    void cancel_translation();
    [[nodiscard]] AppSnapshot snapshot() const;
    bool wait_for_terminal(std::chrono::milliseconds timeout);

private:
    void join_worker();
    void apply_event(const TranslationEvent& event);

    std::shared_ptr<CoreBridge> bridge_;
    std::shared_ptr<CredentialStore> credential_store_;
    mutable std::mutex mutex_;
    std::condition_variable condition_;
    AppSnapshot state_;
    std::shared_ptr<CancellationToken> cancellation_;
    std::thread worker_;
    std::uint64_t next_operation_ = 1;
};

}
