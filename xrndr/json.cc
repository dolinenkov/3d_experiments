#include <xrndr/json.hh>

namespace xrndr
{
namespace json
{

//

Node::Node()
    : _node(nullptr)
{
}

Node::Node(json_t * node)
    : _node(node)
{
    /*if (_node)
    {
        json_incref(_node);
    }*/
}

Node::Node(const Node & another)
    : _node(another._node)
{
    if (_node)
    {
        json_incref(_node);
    }
}

Node & Node::operator=(const Node & another)
{
    if (&another != this)
    {
        if (_node)
        {
            json_decref(_node);
        }

        _node = another._node;

        if (_node)
        {
            json_incref(_node);
        }
    }
    return *this;
}

Node::~Node()
{
    if (_node)
    {
        json_decref(_node);
    }
}

size_t Node::getSizeAsArray() const
{
    return json_array_size(_node);
}

size_t Node::getSizeAsObject() const
{
    return json_object_size(_node);
}

bool Node::hasIndex(size_t index) const
{
    return getByIndex(index) != nullptr;
}

bool Node::hasName(const char * name) const
{
    return getByName(name) != nullptr;
}

bool Node::getBoolByIndex(size_t index) const
{
    return json_boolean_value(getByIndex(index));
}

bool Node::getBoolByName(const char * name) const
{
    return json_boolean_value(getByName(name));
}

int Node::getIntByIndex(size_t index) const
{
    return json_integer_value(getByIndex(index));
}

int Node::getIntByName(const char * name) const
{
    return json_integer_value(getByName(name));
}

float Node::getFloatByIndex(size_t index) const
{
    return json_number_value(getByIndex(index));
}

float Node::getFloatByName(const char * name) const
{
    return json_number_value(getByName(name));
}

std::string Node::getStringByIndex(size_t index) const
{
    return json_string_value(getByIndex(index));
}

std::string Node::getStringByName(const char * name) const
{
    return json_string_value(getByName(name));
}

Array Node::getArrayByIndex(size_t index) const
{
    json_t * const a = getByIndex(index);

    if (a && a->type == JSON_ARRAY)
        return Array(a);

    return Array();
}

Array Node::getArrayByName(const char * name) const
{
    json_t * const a = getByName(name);

    if (a && a->type == JSON_ARRAY)
        return Array(a);

    return Array();
}

Object Node::getObjectByIndex(size_t index) const
{
    json_t * const o = getByIndex(index);

    if (o && o->type == JSON_OBJECT)
        return Object(o);

    return Object();
}

Object Node::getObjectByName(const char * name) const
{
    json_t * const o = getByName(name);

    if (o && o->type == JSON_OBJECT)
        return Object(o);

    return Object();
}

json_t * Node::getByIndex(size_t index) const
{
    return json_array_get(_node, index);
}

json_t * Node::getByName(const char * name) const
{
    return json_object_get(_node, name);
}

//

Array::Array()
{
}

Array::Array(json_t * node)
    : Node(node)
{
}

Array::~Array()
{
}

bool Array::empty() const
{
    return size() == 0;
}

size_t Array::size() const
{
    return getSizeAsArray();
}

bool Array::has(size_t index) const
{
    return hasIndex(index);
}

bool Array::getBool(size_t index) const
{
    return getBoolByIndex(index);
}

int Array::getInt(size_t index) const
{
    return getIntByIndex(index);
}

float Array::getFloat(size_t index) const
{
    return getFloatByIndex(index);
}

std::string Array::getString(size_t index) const
{
    return getStringByIndex(index);
}

Array Array::getArray(size_t index) const
{
    return getArrayByIndex(index);
}

Object Array::getObject(size_t index) const
{
    return getObjectByIndex(index);
}

//

Object::Object()
{
}

Object::Object(json_t * node)
    : Node(node)
{
}

Object::Object(const char * str)
    : Node(json_loads(str, 0, nullptr))
{
}

Object::Object(const char * str, size_t len)
    : Node(json_loadb(str, len, 0, nullptr))
{
}

Object::~Object()
{
}

bool Object::empty() const
{
    return size() == 0;
}

size_t Object::size() const
{
    return getSizeAsObject();
}

bool Object::has(const char * name) const
{
    return hasName(name);
}

bool Object::getBool(const char * name) const
{
    return getBoolByName(name);
}

int Object::getInt(const char * name) const
{
    return getIntByName(name);
}

float Object::getFloat(const char * name) const
{
    return getFloatByName(name);
}

std::string Object::getString(const char * name) const
{
    return getStringByName(name);
}

Array Object::getArray(const char * name) const
{
    return getArrayByName(name);
}

Object Object::getObject(const char * name) const
{
    return getObjectByName(name);
}

}
}
