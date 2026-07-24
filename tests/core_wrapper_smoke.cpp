#include <linguamesh/linguamesh.hpp>

#include <chrono>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {

void require(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

}

int main() {
    try {
        const auto compatibility = linguamesh::engine::query_compatibility();
        require(compatibility.abi_major == LM_ABI_VERSION_MAJOR, "ABI version mismatch");
        require(compatibility.protocol == LM_PROTOCOL_VERSION, "protocol version mismatch");

        auto engine = linguamesh::engine::create();
        const std::vector<std::uint8_t> path{
            'C', ':', '\\', 'T', 'e', 'm', 'p', '\\', 'L', 'i', 'n', 'g', 'u', 'a', 'M', 'e', 's', 'h'};
        auto lease = engine.create_desktop_path(path);
        require(lease.valid() && lease.is_active(), "desktop path lease was not activated");
        lease.expire();
        require(!lease.is_active(), "expired desktop path lease remained active");
        lease.reset();

        const std::vector<std::uint8_t> malformed{0xff};
        try {
            engine.submit(malformed);
            throw std::runtime_error("malformed command was accepted");
        } catch (const linguamesh::core_error& error) {
            require(
                error.code() == linguamesh::result_code::malformed_message,
                "malformed command returned the wrong result");
        }

        require(engine.poll_event(std::chrono::milliseconds(0)).empty(), "unexpected event was queued");
        engine.shutdown();
        std::cout << "Pinned Core C++ wrapper smoke passed.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Pinned Core C++ wrapper smoke failed: " << error.what() << '\n';
        return 1;
    }
}
