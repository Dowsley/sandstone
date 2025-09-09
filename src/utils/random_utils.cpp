//
// Created by João Dowsley on 09/09/25.
//

#include "random_utils.h"

#include <chrono>
#include <limits>

// Global seed state used to seed thread-local engines.
std::atomic<uint64_t> &RandomUtils::global_seed_state()
{
    static std::atomic state(
        static_cast<uint64_t>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count()
        )
    );
    return state;
}

uint64_t RandomUtils::next_seed()
{
    // xorshift64* on an atomic state for cross-thread unique seeds
    uint64_t x = global_seed_state().fetch_add(0x9E3779B97F4A7C15ull, std::memory_order_relaxed);
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    return x * 2685821657736338717ull;
}

std::mt19937 &RandomUtils::engine()
{
    thread_local std::mt19937 eng(static_cast<uint32_t>(next_seed()));
    return eng;
}

void RandomUtils::reseed(const uint32_t seed)
{
    // Reset global seed state and reseed this thread's engine
    global_seed_state().store(seed, std::memory_order_relaxed);
    engine().seed(seed);
}

int RandomUtils::uniform_int(const int min_inclusive, const int max_inclusive)
{
    std::uniform_int_distribution dist(min_inclusive, max_inclusive);
    return dist(engine());
}

float RandomUtils::uniform_float(const float min_inclusive, const float max_exclusive)
{
    std::uniform_real_distribution<float> dist(min_inclusive, std::nextafter(max_exclusive, std::numeric_limits<float>::max()));
    return dist(engine());
}

bool RandomUtils::coin_flip()
{
    return uniform_int(0, 1) == 1;
}

int RandomUtils::index(const int size)
{
    if (size <= 0) return 0;
    return uniform_int(0, size - 1);
}


