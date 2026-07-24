#include "linguamesh/windows/app_model.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

namespace {

using namespace linguamesh::windows;

void require(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void test_fake_provider_streams_and_completes() {
    auto bridge = std::make_shared<FakeCoreBridge>();
    auto store = std::make_shared<MemoryCredentialStore>();
    AppModel model(bridge, store);

    require(model.select_model("fake-translator"), "fake model selection failed");
    model.start_translation("Hello, LinguaMesh");
    require(model.wait_for_terminal(std::chrono::seconds(2)), "translation did not terminate");

    const auto snapshot = model.snapshot();
    require(snapshot.status == TranslationStatus::Completed, "translation did not complete");
    require(snapshot.output == "你好，LinguaMesh！", "unexpected translated output");
    require(snapshot.error == ErrorKind::None, "successful translation has an error");
}

void test_cancellation_retains_partial_output() {
    auto bridge = std::make_shared<FakeCoreBridge>();
    auto store = std::make_shared<MemoryCredentialStore>();
    AppModel model(bridge, store);

    require(model.select_model("fake-slow-translator"), "slow fake model selection failed");
    model.start_translation("Cancellation keeps partial output");
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    model.cancel_translation();
    require(model.wait_for_terminal(std::chrono::seconds(2)), "cancellation did not terminate");

    const auto snapshot = model.snapshot();
    require(snapshot.status == TranslationStatus::Cancelled, "translation was not cancelled");
    require(!snapshot.output.empty(), "cancellation discarded partial output");
    require(snapshot.error_key == "status.cancelled", "cancellation error key is not localized");
}

void test_secure_store_returns_reference_without_plaintext_state() {
    MemoryCredentialStore store;
    const auto saved = store.save("profile-a", "secret-canary");
    require(saved.ok(), "memory secure-store save failed");
    require(saved.reference.id == "profile-a", "secret reference was not returned");
    require(saved.value.empty(), "save result exposed the secret value");

    const auto resolved = store.resolve(saved.reference);
    require(resolved.ok(), "memory secure-store resolve failed");
    require(resolved.value == "secret-canary", "secure-store value did not round-trip");
    require(store.remove(saved.reference) == CredentialError::None, "secure-store remove failed");
    require(store.resolve(saved.reference).error == CredentialError::NotFound,
            "removed credential remained available");
}

void test_typed_invalid_request_and_locale_switch() {
    auto bridge = std::make_shared<FakeCoreBridge>();
    auto store = std::make_shared<MemoryCredentialStore>();
    AppModel model(bridge, store);

    require(model.set_locale("zh-CN"), "locale switch failed");
    require(!model.select_model("unknown-model"), "unknown model was accepted");
    model.start_translation("");
    require(model.wait_for_terminal(std::chrono::seconds(2)), "invalid request did not terminate");

    const auto snapshot = model.snapshot();
    require(snapshot.status == TranslationStatus::Failed, "invalid request did not fail");
    require(snapshot.error == ErrorKind::InvalidRequest, "invalid request error was not typed");
    require(snapshot.error_key == "error.invalid_request", "invalid request key is not localized");
    require(snapshot.locale == "zh-CN", "locale was not retained in immutable state");
}

}

int main() {
    try {
        test_fake_provider_streams_and_completes();
        test_cancellation_retains_partial_output();
        test_secure_store_returns_reference_without_plaintext_state();
        test_typed_invalid_request_and_locale_switch();
        std::cout << "Windows reference slice tests passed.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Windows reference slice tests failed: " << error.what() << '\n';
        return 1;
    }
}
