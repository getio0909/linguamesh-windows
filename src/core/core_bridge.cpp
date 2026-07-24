#include "linguamesh/windows/core_bridge.hpp"

#include <algorithm>
#include <chrono>
#include <thread>

namespace linguamesh::windows {

void CancellationToken::cancel() noexcept {
    cancelled_.store(true, std::memory_order_release);
}

bool CancellationToken::cancelled() const noexcept {
    return cancelled_.load(std::memory_order_acquire);
}

std::vector<std::string> FakeCoreBridge::list_models() const {
    return {"fake-translator", "fake-slow-translator"};
}

void FakeCoreBridge::translate(const TranslationRequest& request,
                               const EventSink& sink,
                               CancellationToken& cancellation) {
    if (request.operation_id.empty() || request.source_text.empty() || request.model_id.empty()) {
        sink({request.operation_id, 0, TranslationStatus::Failed, {}, ErrorKind::InvalidRequest});
        return;
    }

    const auto models = list_models();
    if (std::find(models.begin(), models.end(), request.model_id) == models.end()) {
        sink({request.operation_id, 0, TranslationStatus::Failed, {}, ErrorKind::InvalidRequest});
        return;
    }

    std::string translated = request.source_text;
    if (request.source_text == "Hello, LinguaMesh") {
        translated = "你好，LinguaMesh！";
    } else {
        translated = "Translated: " + request.source_text;
    }

    std::uint64_t sequence = 0;
    std::size_t offset = 0;
    while (offset < translated.size()) {
        if (cancellation.cancelled()) {
            sink({request.operation_id, sequence, TranslationStatus::Cancelled, {}, ErrorKind::Cancelled});
            return;
        }

        const auto chunk_size = std::min<std::size_t>(3, translated.size() - offset);
        sink({request.operation_id,
              sequence++,
              TranslationStatus::Streaming,
              translated.substr(offset, chunk_size),
              ErrorKind::None});
        offset += chunk_size;

        if (request.model_id == "fake-slow-translator") {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }

    if (cancellation.cancelled()) {
        sink({request.operation_id, sequence, TranslationStatus::Cancelled, {}, ErrorKind::Cancelled});
        return;
    }
    sink({request.operation_id, sequence, TranslationStatus::Completed, {}, ErrorKind::None});
}

const char* localized_error_key(ErrorKind error) noexcept {
    switch (error) {
    case ErrorKind::Authentication:
        return "error.authentication";
    case ErrorKind::Network:
        return "error.network";
    case ErrorKind::InvalidRequest:
        return "error.invalid_request";
    case ErrorKind::CoreUnavailable:
        return "error.core_unavailable";
    case ErrorKind::Cancelled:
        return "status.cancelled";
    case ErrorKind::None:
        return "";
    }
    return "error.unknown";
}

}
