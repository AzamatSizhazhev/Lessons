#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>

using namespace std;


template<class T>
int partition(vector<T>& a, int low, int high) {
    T pivot = a[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (a[j] < pivot) {
            ++i;
            swap(a[i], a[j]);
        }
    }
    swap(a[i + 1], a[high]);
    return i + 1;
}

template<class T>
void quickSort(vector<T>& a, int low, int high) {
    if (low < high) {
        int p = partition(a, low, high);
        quickSort(a, low, p - 1);
        quickSort(a, p + 1, high);
    }
}

template<class T>
void myQuickSort(vector<T>& a) {
    if (!a.empty())
        quickSort(a, 0, static_cast<int>(a.size()) - 1);
}

vector<int> generateInts(size_t n, unsigned seed) {
    mt19937 gen(seed);
    uniform_int_distribution<int> dist(0, 1000000);
    vector<int> v(n);
    for (size_t i = 0; i < n; ++i)
        v[i] = dist(gen);
    return v;
}

vector<string> generateStrings(size_t n, unsigned seed) {
    mt19937 gen(seed);
    uniform_int_distribution<int> lenDist(1, 10);
    uniform_int_distribution<int> chDist('a', 'z');
    vector<string> v(n);
    for (size_t i = 0; i < n; ++i) {
        int len = lenDist(gen);
        string s;
        s.resize(len);
        for (int j = 0; j < len; ++j)
            s[j] = static_cast<char>(chDist(gen));
        v[i] = s;
    }
    return v;
}

template<class T, class Sorter>
double measure(const vector<T>& original, Sorter sorter, int runs) {
    using namespace chrono;

    double total = 0.0;
    for (int r = 0; r < runs; ++r) {
        vector<T> copy = original;

        high_resolution_clock::time_point start = high_resolution_clock::now();
        sorter(copy);
        high_resolution_clock::time_point end = high_resolution_clock::now();

        total += duration_cast<duration<double, milli>>(end - start).count();
    }
    return total / runs;
}

void printHeader(const string& title) {
    cout << endl;
    cout << "=== " << title << " ===" << endl;
    cout << left
         << setw(14) << "N"
         << setw(16) << "MyQuickSort(ms)"
         << setw(16) << "std::sort(ms)"
         << setw(14) << "Ratio" << endl;
    cout << string(60, '-') << endl;
}

void printRow(size_t n, double myTime, double stdTime) {
    cout << left
         << setw(14) << n
         << setw(16) << fixed << setprecision(3) << myTime
         << setw(16) << stdTime
         << setw(14) << (stdTime > 0 ? myTime / stdTime : 0.0)
         << endl;
}

int main() {
    const int RUNS = 5;

    vector<size_t> sizes;
    sizes.push_back(10000);
    sizes.push_back(50000);
    sizes.push_back(100000);

    printHeader("int");

    for (size_t k = 0; k < sizes.size(); ++k) {
        size_t n = sizes[k];
        vector<int> data = generateInts(n, 42);

        double tMy  = measure(data, [](vector<int>& v){ myQuickSort(v); }, RUNS);
        double tStd = measure(data, [](vector<int>& v){ std::sort(v.begin(), v.end()); }, RUNS);

        printRow(n, tMy, tStd);
    }

    printHeader("std::string");

    for (size_t k = 0; k < sizes.size(); ++k) {
        size_t n = sizes[k];
        vector<string> data = generateStrings(n, 42);

        double tMy  = measure(data, [](vector<string>& v){ myQuickSort(v); }, RUNS);
        double tStd = measure(data, [](vector<string>& v){ std::sort(v.begin(), v.end()); }, RUNS);

        printRow(n, tMy, tStd);
    }

    return 0;
}