#ifndef json_h_20170119
#define json_h_20170119

#include <memory>
#include <string>
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
        vtBoolean, vtNumberDouble, vtNumberInteger, vtString,
        vtArray, vtObject
    };

public:
    json(void);
    json(Type type);
    json(const std::string& buffer);
    json(const stream& istream);
    json(valuesptr& value);
    json(const json& json);
   ~json(void);

public:
    bool load(const std::string& buffer);
    bool load(const stream& istream);

    std::string dump(void) const;
    std::string dump(unsigned int indent) const;
    bool dump(stream& ostream) const;
    bool dump(stream&& ostream) const;
    bool dump(stream& ostream, unsigned int indent) const;
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
    json& operator=(const char* string);
    json& operator=(const std::string& string);
    json& operator=(const json& json);

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
    bool append(const char* string);
    bool append(const std::string& string);
    bool append(const json& json);

    bool clear(void);
    bool remove(unsigned int index);
    bool remove(const std::string& key);

public:
    unsigned int size(void) const;
    json operator[](const json& json);
    json operator[](unsigned int index);
    json operator[](const std::string& key);
    const json operator[](const json& json) const;
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

#endif // json_h_20170119
