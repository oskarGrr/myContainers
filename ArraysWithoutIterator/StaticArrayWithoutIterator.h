#pragma once
#include "Vec4.h"

//templated class as a wrapper for a 
//default stack allocated, statically sized array (std::array<T, size_t>)
//iterator not necessary
template <class T, size_t size>
class StaticArray
{
private:
    T elems[size];

public:
    constexpr size_t getSize() const {return size;}

    T& operator[](size_t index) {return elems[index];}
    const T& operator[](size_t index) const {return elems[index];}

    StaticArray() = default;

    //rule of zero
    ~StaticArray() = default;
    StaticArray(StaticArray&&) = default;
    StaticArray(const StaticArray&) = default;
    StaticArray& operator=(StaticArray&&) = default;
    StaticArray& operator=(const StaticArray&) = default;

    //helper method to fill the array with a specific value
    void fill(const T& value) 
    {
        for(int i = 0; i < size; ++i)
            elems[i] = value;
    }

    void fill(T&& value)
    {
        for(int i = 0; i < size; ++i)
            elems[i] = std::move(value);
    }

    T* rawData() {return elems;}
    const T* rawData() const {return elems;}
};