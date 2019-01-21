#pragma once
#include <xrndr/config.hh>

namespace xrndr
{

/*
    an alternative for std::stack<> providing more convenient interface:
    - added 'reserve' method for memory reallocation control
    - added 'clear' method
*/

template<class T>
class Stack
{
public:
    using Value = T;

    Stack();

    bool empty() const;

    const T & top() const;
    T & top();

    void push(const T & object);
    void pop();

    void clear();

    void reserve(size_t count);
    void shrinkToFit();

private:
    std::vector<T> _imp;
};

template<class T>
inline Stack<T>::Stack()
{
}

template<class T>
inline bool Stack<T>::empty() const
{
    return _imp.empty();
}

template<class T>
inline const T & Stack<T>::top() const
{
    return _imp.back();
}

template<class T>
inline T & Stack<T>::top()
{
    return _imp.back();
}

template<class T>
inline void Stack<T>::push(const T & object)
{
    _imp.push_back(object);
}

template<class T>
inline void Stack<T>::pop()
{
    _imp.pop_back();
}

template<class T>
inline void Stack<T>::clear()
{
    _imp.clear();
}

template<class T>
inline void Stack<T>::reserve(size_t count)
{
    _imp.reserve(count);
}

template<class T>
inline void Stack<T>::shrinkToFit()
{
    _imp.shrink_to_fit();
}

}
