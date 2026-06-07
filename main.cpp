#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>

static const size_t N = 4096; // 4096×4096 matrix = 64MB

// accesses memory sequentially — one cache line feeds 16 consecutive elements
float row_sum(const float *mat, size_t rows, size_t cols)
{
    float sum = 0.0f;
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
            sum += mat[i * cols + j];
    return sum;
}

// jumps 4096 floats (16KB) between every access — almost every access is a cache miss
float col_sum(const float *mat, size_t rows, size_t cols)
{
    float sum = 0.0f;
    for (size_t j = 0; j < cols; j++)
        for (size_t i = 0; i < rows; i++)
            sum += mat[i * cols + j];
    return sum;
}

int main()
{
    std::vector<float> mat(N * N);
    std::iota(mat.begin(), mat.end(), 1.0f);

    volatile float sink = 0.0f; // prevents dead code elimination

    // {
    //     auto t0 = std::chrono::steady_clock::now();
    //     sink = row_sum(mat.data(), N, N);
    //     auto t1 = std::chrono::steady_clock::now();
    //     std::cout << "row_sum: "
    //               << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()
    //               << " ms  (result=" << sink << ")\n";
    // }

    {
        auto t0 = std::chrono::steady_clock::now();
        sink = col_sum(mat.data(), N, N);
        auto t1 = std::chrono::steady_clock::now();
        std::cout << "col_sum: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()
                  << " ms  (result=" << sink << ")\n";
    }

    return 0;
}