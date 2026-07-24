#pragma once

#include <atomic>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace linguamesh::windows {

enum class TranslationStatus {
    Idle,
    Streaming,
    Completed,
    Cancelled,
    Failed,
};

enum class ErrorKind {
    None,
    Authentication,
    Network,
    InvalidRequest,
    CoreUnavailable,
    Cancelled,
};

struct TranslationRequest {
    std::string operation_id;
    std::string source_text;
    std::string model_id;
    std::string target_locale;
};

struct TranslationEvent {
    std::string operation_id;
    std::uint64_t sequence = 0;
    TranslationStatus status = TranslationStatus::Idle;
    std::string delta;
    ErrorKind error = ErrorKind::None;
};

class CancellationToken final {
public:
    void cancel() noexcept;
    [[nodiscard]] bool cancelled() const noexcept;

private:
    std::atomic<bool> cancelled_{false};
};

using EventSink = std::function<void(const TranslationEvent&)>;

class CoreBridge {
public:
    virtual ~CoreBridge() = default;

    [[nodiscard]] virtual std::vector<std::string> list_models() const = 0;
    virtual void translate(const TranslationRequest& request,
                           const EventSink& sink,
                           CancellationToken& cancellation) = 0;
};

class FakeCoreBridge final : public CoreBridge {
public:
    [[nodiscard]] std::vector<std::string> list_models() const override;
    void translate(const TranslationRequest& request,
                   const EventSink& sink,
                   CancellationToken& cancellation) override;
};

[[nodiscard]] const char* localized_error_key(ErrorKind error) noexcept;

}
