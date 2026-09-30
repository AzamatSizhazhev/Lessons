#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

class IntCollection {
    vector<int> data;

public:
    IntCollection() {}

    explicit IntCollection(size_t n) : data(n) {}

    void add(int value) { data.push_back(value); }

    size_t size() const { return data.size(); }

    int at(size_t i) const { return data[i]; }

    void print() const {
        for (size_t i = 0; i < data.size(); ++i)
            cout << data[i] << " ";
        cout << endl;
    }

    void sortAsc() { std::sort(data.begin(), data.end()); }

    int countEven() const {
        return static_cast<int>(
            std::count_if(data.begin(), data.end(),[](int x) { return x % 2 == 0; })
        );
    }

    size_t findFirstGreater(int threshold) const {
        vector<int>::const_iterator it = std::find_if(data.begin(), data.end(), [threshold](int x) { return x > threshold; });

        if (it == data.end())
            return data.size();

        return static_cast<size_t>(it - data.begin());
    }


};


int main() {
    {
        IntCollection c;
        c.add(5);
        c.add(3);
        c.add(8);
        c.add(1);
        c.add(4);

        cout << "До сортировки: ";
        c.print();

        c.sortAsc();

        cout << "После сортировки: ";
        c.print();
    }

    cout << endl;

    {
        IntCollection c;
        c.add(1);
        c.add(2);
        c.add(3);
        c.add(4);
        c.add(6);

        int n = c.countEven();

        cout << "Вектор: ";
        c.print();
        cout << "Чётных: " << n << endl;
    }

    cout << endl;

    {
        IntCollection c;
        c.add(2);
        c.add(4);
        c.add(7);
        c.add(9);
        c.add(10);

        size_t idx = c.findFirstGreater(6);

        cout << "Вектор: ";
        c.print();
        cout << "Первый > 6: индекс " << idx << endl;
    }

    cout << endl;

    {
        IntCollection c;
        c.add(1);
        c.add(2);
        c.add(3);

        size_t idx = c.findFirstGreater(100);

        cout << "Вектор: ";
        c.print();
        cout << "Первый > 100: индекс " << idx << " (size = " << c.size() << ")" << endl;
    }

    return 0;
}