//
// Created by João Dowsley on 09/09/25.
//

#ifndef SANDSTONE_RANDOM_UTILS_H
#define SANDSTONE_RANDOM_UTILS_H

#include <random>
#include <atomic>

/**
 * @brief Thread-safe randomness machine. 
 * 
 */
class RandomUtils {
private:
    // Global seed state used to initialize per-thread RNGs
    static std::atomic<uint64_t> &global_seed_state();
    static uint64_t next_seed();
    // Per-thread engine (thread-safe, no sharing across threads)
    static std::mt19937 &engine();

public:
    // Reseed with a fixed value (useful for tests/determinism)
    static void reseed(uint32_t seed);

    // Inclusive integer in [min, max]
    static int uniform_int(int min_inclusive, int max_inclusive);

    // Real in [min, max)
    static float uniform_float(float min_inclusive, float max_exclusive);

    // 50/50 coin flip
    static bool coin_flip();

    // Pick a random index in [0, size)
    static int index(int size);
};

#endif // SANDSTONE_RANDOM_UTILS_H


