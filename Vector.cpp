#include <iostream>

template<class T>
class Vector {
    T* vector;
    size_t length;

public:
    Vector() : vector(nullptr), length(0) {}

    explicit Vector(size_t n) : vector(new T[n]), length(n) {}

    Vector(const Vector& other) : vector(new T[other.length]), length(other.length) {
        for (int i = 0; i < length; i++)
            vector[i] = other.vector[i];
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] vector;
            length = other.length;
            vector = new T[length];
            for (int i = 0; i < length; i++)
                vector[i] = other.vector[i];
        }
        return *this;
    }

    T& operator[](size_t i) { return vector[i]; }
    const T& operator[](size_t i) const { return vector[i]; }

    T& elem(int i) { return vector[i]; }
    const T& elem(int i) const { return vector[i]; }

    void push(const T& value) {
        T* newVec = new T[length + 1];
        for (size_t i = 0; i < length; ++i)
            newVec[i] = vector[i];
        newVec[length] = value;
        delete[] vector;
        vector = newVec;
        ++length;
    }

    void swap(Vector& other) {
        T* tmpV = vector;
        vector = other.vector;
        other.vector = tmpV;

        int tmpSz = length;
        length = other.length;
        other.length = tmpSz;
    }

    template <class C>
    int countInVector() const {
        int count = 0;
        for (int i = 0; i < length; ++i)
            if (C::operator()(vector[i]))
                ++count;
        return count;
    }

    size_t len() const { return length; }
    ~Vector() { delete[] vector; }
};