#pragma once

#include <wifi_credentials.hpp>

#include <atomic>

namespace nfc {

static constexpr int32_t OPTIMAL_CHECK_DIFF_MS { 200 };

void init();

void turn_on();
void turn_off();

bool has_activity();

std::optional<WifiCredentials> consume_data();

struct SharedEnabler {
    SharedEnabler();
    ~SharedEnabler();

    SharedEnabler(const SharedEnabler &) = delete;
    SharedEnabler &operator=(const SharedEnabler &) = delete;

protected:
    static std::atomic<uint8_t> level;
};

}; // namespace nfc