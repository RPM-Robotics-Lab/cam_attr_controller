#include "TimeStamp.h"

namespace bot_util
{

int64_t timestamp_now()
{
    std::chrono::system_clock::time_point now = std::chrono::high_resolution_clock::now();
    std::chrono::system_clock::duration duration_now = now.time_since_epoch();

    return std::chrono::duration_cast<std::chrono::microseconds>(duration_now).count();
}

}

