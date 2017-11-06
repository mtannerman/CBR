#pragma once

namespace cbr {

template <class T>
class CounterT
{
public:
    void operator+=(const T value_)
    {
        value += value_; 
    }

    // void operator++()
    // {
    //     value++;
    // }

    T Get() const
    {
        return value;
    }

private:
    T value = T(0);
};


typedef CounterT<int> Counter;

}