#ifndef json_h_20170119
#define json_h_20170119

#include <memory>
#include <string>
#include <vector>
#include <takisy/core/stream.h>

class json
{
    class value;
    class implement;

    typedef value* valueptr;
    typedef std::shared_ptr<value> valuesptr;

public:
    enum Type
    {
        vtUndefined,
        vtNull,
        vtBoolTrue,
        vtBoolFalse,
        vtNumberDouble,
        vtNumberInteger,
        vtString,
        vtArray,
        vtObject
    };

    class array;
    class object;

public:
    json(void);
    json(Type type);
    json(const std::string& buffer);
    json(const stream& istream);
    json(valueptr value);
    json(valuesptr& value);
    json(json& jsonobj);
    json(json&& jsonobj);
    json(const json& jsonobj);
   ~json(void);

public:
    bool load(const std::string& buffer);
    bool load(const stream& istream);

    std::string dump(void) const;
    std::string dump(unsigned int indent) const;
    bool dump(stream&  ostream) const;
    bool dump(stream&& ostream) const;
    bool dump(stream&  ostream, unsigned int indent) const;
    bool dump(stream&& ostream, unsigned int indent) const;

public:
    json& operator=(Type type);
    json& operator=(std::nullptr_t);
    json& operator=(bool boolean);
    json& operator=(int number);
    json& operator=(unsigned int number);
    json& operator=(long number);
    json& operator=(unsigned long number);
    json& operator=(long long number);
    json& operator=(unsigned long long number);
    json& operator=(double number);
    json& operator=(char ch);
    json& operator=(const char* string);
    json& operator=(const std::string& string);
    json& operator=(json& jsonobj);
    json& operator=(json&& jsonobj);
    json& operator=(const json& jsonobj);

public:
    bool append(Type type);
    bool append(std::nullptr_t);
    bool append(bool boolean);
    bool append(int number);
    bool append(unsigned int number);
    bool append(long number);
    bool append(unsigned long number);
    bool append(long long number);
    bool append(unsigned long long number);
    bool append(double number);
    bool append(char ch);
    bool append(const char* string);
    bool append(const std::string& string);
    bool append(json& jsonobj);
    bool append(json&& jsonobj);
    bool append(const json& jsonobj);

    bool clear(void);
    bool remove(unsigned int index);
    bool remove(const std::string& key);

public:
    unsigned int size(void) const;
    bool has_key(const std::string& key) const;
    std::vector<std::string> keys(void) const;
    json operator[](unsigned int index);
    json operator[](const std::string& key);
    const json operator[](unsigned int index) const;
    const json operator[](const std::string& key) const;

public:
    json copy(void) const;

public:
    Type        type(void) const;
    bool        as_bool(void) const;
    double      as_double(void) const;
    long long   as_integer(void) const;
    std::string as_string(void) const;

private:
    class implement* impl_;
};

class json::array : public json
{
public:
    array(void)
        : json(vtArray)
    {}

    template <typename... Items>
    array(Items&&... items)
        : json(vtArray)
    {
        append(std::forward<Items>(items)...);
    }

private:
    template <typename Item>
    void append(Item&& item)
    {
        json::append(std::forward<Item>(item));
    }

    template <typename Item, typename... Others>
    void append(Item&& item, Others&&... others)
    {
        json::append(std::forward<Item>(item));
        append(std::forward<Others>(others)...);
    }
};

class json::object : public json
{
public:
    object(void)
        : json(vtObject)
    {}

    template <typename... KeyValues>
    object(KeyValues&&... keyvalues)
        : json(vtObject)
    {
        insert(std::forward<KeyValues>(keyvalues)...);
    }

private:
    template <typename Key, typename Value>
    void insert(Key&& key, Value&& value)
    {
        json::operator[](key) = value;
    }

    template <typename Key, typename Value, typename... KeyValues>
    void insert(Key&& key, Value&& value, KeyValues&&... keyvalues)
    {
        json::operator[](key) = value;
        insert(std::forward<KeyValues>(keyvalues)...);
    }
};

#endif // json_h_20170119
