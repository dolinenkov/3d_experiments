#pragma once
#include "config.hh"

/*
    an alternative for std::stack<> providing more convenient interface:
    - added 'reserve' method for memory reallocation control
    - added 'clear' method
*/


template<class T>
class Stack
{
public:
    Stack();

    bool empty() const;
    const T & top() const;

    void push(const T & object);
    void pop();

    void clear();

    void reserve(size_t count);
    void shrinkToFit();

private:
    std::vector<T> _imp;
};


using MatrixStack = Stack<mat4>;


struct MatrixGroup
{
    mat4 modelViewProjectionMatrix;
    mat4 modelViewMatrix;
    mat4 modelMatrix;
    mat3 normalMatrix;
};


struct MatrixStackSetSettings
{
    static MatrixStackSetSettings defaults()
    {
        return MatrixStackSetSettings { 1, 1, 200 };
    }

    size_t projectionMatrixStackSize;
    size_t viewMatrixStackSize;
    size_t modelMatrixStackSize;
};


class MatrixStackSet
{
public:
    explicit MatrixStackSet(const MatrixStackSetSettings & settings = MatrixStackSetSettings::defaults());

    const MatrixStack & getProjectionMatrixStack() const;
    MatrixStack & getProjectionMatrixStack();

    const MatrixStack & getViewMatrixStack() const;
    MatrixStack & getViewMatrixStack();

    const MatrixStack & getModelMatrixStack() const;
    MatrixStack & getModelMatrixStack();

    const MatrixGroup & getMatrixGroup();

private:
    MatrixStack projectionMatrixStack;
    MatrixStack viewMatrixStack;
    MatrixStack modelMatrixStack;
    bool        needUpdateMatrixGroup;
    MatrixGroup matrixGroup;
};


//


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
