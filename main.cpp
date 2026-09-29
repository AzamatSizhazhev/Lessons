#include <iostream>
#include </Users/azamatsizazev/CLionProjects/CProject/Vector.cpp>

class Vec3d {
    double x, y, z;
public:
    Vec3d(double x=0, double y=0, double z=0) : x(x), y(y), z(z) {}
    Vec3d(const Vec3d& other) : x(other.x), y(other.y), z(other.z) {}

    double get_x() const { return x; }
    double get_y() const { return y; }
    double get_z() const { return z; }
};

template<class T>
class Cmp {
public:
    static bool operator()(T a){ return a != 0; }
};

class Vec3d_cmp {
    static double _x, _y, _z;
public:
    static void set(double x, double y, double z) { _x = x; _y = y; _z = z; }
    static bool operator()(Vec3d v) {
        return v.get_x() > _x && v.get_y() > _y && v.get_z() > _z;
    }
};

double Vec3d_cmp::_x = 0;
double Vec3d_cmp::_y = 0;
double Vec3d_cmp::_z = 0;


class LongStringCmp {
    static std::size_t _minLen;
public:
    static void set(std::size_t n) { _minLen = n; }
    static bool operator()(const std::string& s) { return s.size() > _minLen; }
};

std::size_t LongStringCmp::_minLen = 0;


int main() {
    Vector<int> v_int;
    Vector<double> v_double;
    Vector<Vec3d> v_my_class;

    v_int.push(1);
    v_int.push(0);
    v_int.push(5);
    v_int.push(0);
    v_int.push(3);

    int n1 = v_int.countInVector<Cmp<int>>();
    std::cout << "Cmp<int>: " << n1 << std::endl;

    v_double.push(0.0);
    v_double.push(2.5);
    v_double.push(-1.0);
    v_double.push(0.0);

    int n2 = v_double.countInVector<Cmp<double>>();
    std::cout << "Cmp<double>: " << n2 << std::endl;

    v_my_class.push(Vec3d(1, 2, 3));   // +
    v_my_class.push(Vec3d(-1, 2, 3));  // -
    v_my_class.push(Vec3d(1, -2, 3));  // -
    v_my_class.push(Vec3d(4, 5, 6));   // +
    v_my_class.push(Vec3d(0, 1, 1));   // -

    Vec3d_cmp::set(0, 0, 0);
    int n3 = v_my_class.countInVector<Vec3d_cmp>();
    std::cout << "Vec3d_cmp: " << n3 << std::endl;

    Vector<std::string> v_str;
    v_str.push("a");
    v_str.push("hello");
    v_str.push("hi");
    v_str.push("world!");
    v_str.push("C++");

    LongStringCmp::set(2);
    int n4 = v_str.countInVector<LongStringCmp>();
    std::cout << "LongStringCmp > 2: " << n4 << std::endl;

    LongStringCmp::set(5);
    int n5 = v_str.countInVector<LongStringCmp>();
    std::cout << "LongStringCmp > 5: " << n5 << std::endl;

    return 0;
}