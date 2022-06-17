#ifndef CHECKERSAI_DYNAMICARRAY_H
#define CHECKERSAI_DYNAMICARRAY_H

template <typename T>
class DynamicArray {
    T* t = nullptr;
    int length = 1;//dlugosc wektora
    int amountOfElements = 0;//ilosc elementów wektora

    void copy(T* to) const;
    void increaseLength();
public:
    DynamicArray();
    DynamicArray(const DynamicArray<T> &copy);
    void push(T elem);
    void push_many(const DynamicArray<T> &arr);
    T& operator[](int idx) const;
    T pop();
    int size() const;
    void resize(int size);
    T* begin() const;
    T* end() const;
    void clear();
    DynamicArray<T> clone() const;
    ~DynamicArray();
};

#include "DynamicArray_impl.h"
#endif //CHECKERSAI_DYNAMICARRAY_H
