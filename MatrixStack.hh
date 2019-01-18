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


class MatrixStack
{
public:
    using Value = typename Stack<mat4>::Value;

    explicit MatrixStack(const char * name);

    bool empty() const;

    const Value & top() const;
    Value & top();

    void push(const Value & object);
    void pop();

    void clear();

    void reserve(size_t count);
    void shrintToFit();

private:
    Stack<mat4>  _imp;
    const char * _name;
};



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
    class TransformationScope;

    explicit MatrixStackSet(const MatrixStackSetSettings & settings = MatrixStackSetSettings::defaults());

    TransformationScope transformProjection(const mat4 & matrix, bool additive = false);
    TransformationScope transformView(const mat4 & matrix, bool additive = false);
    TransformationScope transformModel(const mat4 & matrix, bool additive = false);

    const MatrixGroup & getMatrixGroup();

private:
    MatrixStack _projectionStack;
    MatrixStack _viewStack;
    MatrixStack _modelStack;
    MatrixGroup _group;
};


class MatrixStackSet::TransformationScope
{
public:
    TransformationScope(MatrixStackSet & matrixStackSet, MatrixStack & matrixStack, const mat4 & matrix, bool additive);
    ~TransformationScope();

private:
    void _updateGroup();

private:
    MatrixStack &    _matrixStack;
    MatrixStackSet & _matrixStackSet;
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
