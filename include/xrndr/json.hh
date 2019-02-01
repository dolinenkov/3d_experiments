#pragma once
#include <xrndr/config.hh>

namespace xrndr
{
namespace json
{

class Array;
class Object;

class Node
{
public:
    Node();

    explicit Node(json_t * node);

    Node(const Node & another);

    Node & operator=(const Node & another);

    ~Node();

protected:

    size_t getSizeAsArray() const;
    size_t getSizeAsObject() const;

    bool hasIndex(size_t index) const;
    bool hasName(const char * name) const;

    bool getBoolByIndex(size_t index) const;
    bool getBoolByName(const char * name) const;

    int getIntByIndex(size_t index) const;
    int getIntByName(const char * name) const;

    float getFloatByIndex(size_t index) const;
    float getFloatByName(const char * name) const;

    std::string getStringByIndex(size_t index) const;
    std::string getStringByName(const char * name) const;

    Array getArrayByIndex(size_t index) const;
    Array getArrayByName(const char * name) const;

    Object getObjectByIndex(size_t index) const;
    Object getObjectByName(const char * name) const;

private:
    json_t * getByIndex(size_t index) const;
    json_t * getByName(const char * name) const;

private:
    json_t * _node;
};

class Array: public Node
{
public:
    Array();
    Array(json_t * node);

    ~Array();

    bool empty() const;

    size_t size() const;

    bool has(size_t index) const;

    bool getBool(size_t index) const;
    int getInt(size_t index) const;
    float getFloat(size_t index) const;
    std::string getString(size_t index) const;
    Array getArray(size_t index) const;
    Object getObject(size_t index) const;
};

class Object: public Node
{
public:
    Object();

    explicit Object(json_t * node);

    template<size_t N>
    explicit Object(const char(&str)[N])
        : Object(str, N)
    {
    }

    explicit Object(const char * str);
    Object(const char * str, size_t len);

    ~Object();

    bool empty() const;

    size_t size() const;

    bool has(const char * name) const;

    bool getBool(const char * name) const;
    int getInt(const char * name) const;
    float getFloat(const char * name) const;
    std::string getString(const char * name) const;
    Array getArray(const char * name) const;
    Object getObject(const char * name) const;
};

}
}
