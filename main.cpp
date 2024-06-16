#include <iostream>
#include <cmath>
#include <type_traits>

template <typename T>
concept IsFloatingPt = std::is_floating_point_v<T>;

template <IsFloatingPt T>
constexpr inline auto fast_inv_sqrt(const T val) -> T {
    constexpr auto wtf_val = (sizeof(T) == 8 ? 0x5fe6eb50c7b537a9 : 0x5f3759df);
    using int_type = typename std::conditional<sizeof(T) == 8, std::int64_t, std::int32_t>::type;
    T y = val;
    T x2 = y * 0.5;
    int_type i = *(int_type *)&y;
    i = wtf_val - (i >> 1);
    y = *(T *)&i;
    return y * (1.5 - (x2 * y * y));
}


int main() {
    const double i = 10.0;
    {
        auto start = std::chrono::system_clock::now();
        std::cout << 1.0 / sqrt(i) << std::endl;
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> delta_t = end-start;

        std::cout << delta_t.count() * 1000000.0 << "micro sec" << std::endl;
    }

    {
        auto start = std::chrono::system_clock::now();
        std::cout << fast_inv_sqrt(10.0) << std::endl;
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> delta_t = end-start;

        std::cout << delta_t.count() * 1000000.0 << "micro sec" << std::endl;
    }

    return 0;
}
