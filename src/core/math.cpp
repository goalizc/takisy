#include <chrono>
#include <takisy/core/math.h>

unsigned int math::seed_ =
    std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch())
        .count();

// float math::sqrt(float n)
// {
//     return sqrt(n, 1);
// }
//
// float math::sqrt(float n, unsigned int iteration_times)
// {
//     int& i = *(int*)(&n);
//     float half = 0.5f * n;
//
//     i = 0x5f375a86 - (i >> 1);
//     for (unsigned int i = 0; i < iteration_times; ++i)
//         n *= 1.5f - half * n * n;
//
//     return 1 / n;
// }
