#include "linguamesh/windows/app_model.hpp"

#include <algorithm>

namespace linguamesh::windows {

AppModel::AppModel(std::shared_ptr<CoreBridge> bridge,
                   std::shared_ptr<CredentialStore> credential_store)
    : bridge_(std::move(bridge)), credential_store_(std::move(credential_store)) {
    const auto models = bridge_->list_models();
    if (!models.empty()) {
        state_.selected_model = models.front();
    }
}

AppModel::~AppModel() {
    cancel_translation();
    join_worker();
}

std::vector<std::string> AppModel::discover_models() const {
    return bridge_->list_models();
}

bool AppModel::select_model(const std::string& model_id) {
    const auto models = bridge_->list_models();
    if (std::find(models.begin(), models.end(), model_id) == models.end()) {
        return false;
    }
    std::lock_guard lock(mutex_);
    state_.selected_model = model_id;
    return true;
}

bool AppModel::set_locale(const std::string& locale) {
    if (locale.empty()) {
        return false;
    }
    std::lock_guard lock(mutex_);
    state_.locale = locale;
    return true;
}

CredentialResult AppModel::save_profile_secret(const std::string& profile_id,
                                               const std::string& secret) {
    return credential_store_->save(profile_id, secret);
}

void AppModel::start_translation(const std::string& source_text) {
    cancel_translation();
    join_worker();

    std::string model;
    std::string operation_id;
    {
        std::lock_guard lock(mutex_);
        model = state_.selected_model;
        operation_id = "windows-op-" + std::to_string(next_operation_++);
        state_.operation_id = operation_id;
        state_.status = TranslationStatus::Streaming;
        state_.error = ErrorKind::None;
        state_.error_key.clear();
        state_.output.clear();
        state_.last_sequence = 0;
        cancellation_ = std::make_shared<CancellationToken>();
    }

    auto token = cancellation_;
    TranslationRequest request{operation_id, source_text, model, snapshot().locale};
    worker_ = std::thread([this, request = std::move(request), token] {
        bridge_->translate(request,
                           [this](const TranslationEvent& event) { apply_event(event); },
                           *token);
    });
}

void AppModel::cancel_translation() {
    std::shared_ptr<CancellationToken> token;
    {
        std::lock_guard lock(mutex_);
        token = cancellation_;
    }
    if (token) {
        token->cancel();
    }
}

AppSnapshot AppModel::snapshot() const {
    std::lock_guard lock(mutex_);
    return state_;
}

bool AppModel::wait_for_terminal(std::chrono::milliseconds timeout) {
    std::unique_lock lock(mutex_);
    return condition_.wait_for(lock, timeout, [this] {
        return state_.status == TranslationStatus::Completed ||
               state_.status == TranslationStatus::Cancelled ||
               state_.status == TranslationStatus::Failed;
    });
}

void AppModel::join_worker() {
    if (worker_.joinable()) {
        worker_.join();
    }
}

void AppModel::apply_event(const TranslationEvent& event) {
    std::lock_guard lock(mutex_);
    if (event.operation_id != state_.operation_id || event.sequence < state_.last_sequence) {
        return;
    }
    state_.last_sequence = event.sequence;
    state_.status = event.status;
    state_.output += event.delta;
    state_.error = event.error;
    state_.error_key = localized_error_key(event.error);
    if (event.status == TranslationStatus::Completed ||
        event.status == TranslationStatus::Cancelled ||
        event.status == TranslationStatus::Failed) {
        condition_.notify_all();
    }
}

}
