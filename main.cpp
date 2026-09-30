#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <iomanip>
#include <string>

using namespace std;

class Matrix {
    size_t rows;
    size_t cols;
    vector<vector<int>> data;

public:
    Matrix() : rows(0), cols(0) {}

    Matrix(size_t r, size_t c) : rows(r), cols(c), data(r, vector<int>(c, 0)) {}

    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }

    int get(size_t i, size_t j) const { return data[i][j]; }
    void set(size_t i, size_t j, int value) { data[i][j] = value; }

    void fillRandom(unsigned seed) {
        mt19937 gen(seed);
        uniform_int_distribution<int> dist(0, 9);
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                data[i][j] = dist(gen);
    }

    void print() const {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j)
                cout << setw(5) << data[i][j];
            cout << endl;
        }
    }
};

Matrix multiplySequential(const Matrix& A, const Matrix& B) {
    size_t n = A.getRows();
    size_t m = A.getCols();
    size_t k = B.getCols();

    Matrix C(n, k);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < k; ++j) {
            int sum = 0;
            for (size_t t = 0; t < m; ++t)
                sum += A.get(i, t) * B.get(t, j);
            C.set(i, j, sum);
        }
    }
    return C;
}

void multiplyRange(const Matrix& A, const Matrix& B, Matrix& C,
                   size_t rowFrom, size_t rowTo) {
    size_t m = A.getCols();
    size_t k = B.getCols();

    for (size_t i = rowFrom; i < rowTo; ++i) {
        for (size_t j = 0; j < k; ++j) {
            int sum = 0;
            for (size_t t = 0; t < m; ++t)
                sum += A.get(i, t) * B.get(t, j);
            C.set(i, j, sum);
        }
    }
}

Matrix multiplyParallel(const Matrix& A, const Matrix& B, int numThreads) {
    size_t n = A.getRows();
    size_t k = B.getCols();

    Matrix C(n, k);

    if (static_cast<size_t>(numThreads) > n)
        numThreads = static_cast<int>(n);

    vector<thread> threads;
    threads.reserve(numThreads);

    size_t blockSize = n / numThreads;
    size_t remainder = n % numThreads;

    size_t start = 0;
    for (int t = 0; t < numThreads; ++t) {
        size_t extra = (static_cast<size_t>(t) < remainder) ? 1 : 0;
        size_t end = start + blockSize + extra;
        threads.push_back(thread(multiplyRange, cref(A), cref(B), ref(C), start, end));
        start = end;
    }

    for (size_t t = 0; t < threads.size(); ++t)
        threads[t].join();

    return C;
}

double measureSequential(const Matrix& A, const Matrix& B, int runs) {
    using namespace chrono;

    double total = 0.0;
    for (int r = 0; r < runs; ++r) {
        steady_clock::time_point start = steady_clock::now();
        Matrix C = multiplySequential(A, B);
        steady_clock::time_point end = steady_clock::now();

        total += duration_cast<duration<double, milli>>(end - start).count();
    }
    return total / runs;
}

double measureParallel(const Matrix& A, const Matrix& B, int numThreads, int runs) {
    using namespace chrono;

    double total = 0.0;
    for (int r = 0; r < runs; ++r) {
        steady_clock::time_point start = steady_clock::now();
        Matrix C = multiplyParallel(A, B, numThreads);
        steady_clock::time_point end = steady_clock::now();

        total += duration_cast<duration<double, milli>>(end - start).count();
    }
    return total / runs;
}

int main() {
    const size_t N = 300;
    const int RUNS = 3;

    cout << "=== Умножение матриц " << N << "x" << N << " ===" << endl;
    cout << "Число аппаратных ядер: " << thread::hardware_concurrency() << endl << endl;

    Matrix A(N, N);
    Matrix B(N, N);
    A.fillRandom(42);
    B.fillRandom(43);

    double tSeq = measureSequential(A, B, RUNS);
    cout << fixed << setprecision(3);
    cout << "Последовательно: " << tSeq << " ms" << endl << endl;

    cout << left
         << setw(16) << "Потоков"
         << setw(20) << "Время (ms)"
         << setw(22) << "Ускорение"
         << setw(16) << "Эффективность" << endl;
    cout << string(54, '-') << endl;

    vector<int> threadCounts;
    threadCounts.push_back(1);
    threadCounts.push_back(2);
    threadCounts.push_back(4);
    threadCounts.push_back(8);
    threadCounts.push_back(16);

    for (size_t i = 0; i < threadCounts.size(); ++i) {
        int t = threadCounts[i];
        double tPar = measureParallel(A, B, t, RUNS);
        double speedup = tSeq / tPar;
        double efficiency = speedup / t;

        cout << left
             << setw(10) << t
             << setw(16) << tPar
             << setw(14) << speedup
             << setw(14) << efficiency << endl;
    }

    return 0;
}