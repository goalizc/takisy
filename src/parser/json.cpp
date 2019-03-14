#include <limits>
#include <numeric>
#include <unordered_map>
#include <vector>
#include <takisy/algorithm/stralgo.h>
#include <takisy/parser/json.h>

class json::value
{
public:
    struct null;
    struct bool_true;
    struct bool_false;
    struct number_double;
    struct number_integer;
    struct string;
    struct array;
    struct object;

public:
    virtual ~value(void) {}
    virtual Type type(void) const = 0;
    virtual valueptr copy(void) const = 0;
    virtual void dump(std::string& buffer, int level, int indent) const = 0;
};

struct json::value::null : public json::value
{
    Type type(void) const override
        { return vtNull; }
    valueptr copy(void) const override
        { return new null(); }
    void dump(std::string& buffer, int, int) const override
        { buffer += "null"; }
};

struct json::value::bool_true : public json::value
{
    Type type(void) const override
        { return vtBoolTrue; }
    valueptr copy(void) const override
        { return new bool_true(); }
    void dump(std::string& buffer, int, int) const override
        { buffer += "true"; }
};

struct json::value::bool_false : public json::value
{
    Type type(void) const override
        { return vtBoolFalse; }
    valueptr copy(void) const override
        { return new bool_false(); }
    void dump(std::string& buffer, int, int) const override
        { buffer += "false"; }
};

struct json::value::number_double : public json::value
{
    double value;

public:
    number_double(double value)
        : value(value)
    {}

public:
    Type type(void) const override
        { return vtNumberDouble; }
    valueptr copy(void) const override
        { return new number_double(value); }
    void dump(std::string& buffer, int, int) const override
        { buffer += stralgo::strf(value); }
};

struct json::value::number_integer : public json::value
{
    long long value;

public:
    number_integer(long long value)
        : value(value)
    {}

public:
    Type type(void) const override
        { return vtNumberInteger; }
    valueptr copy(void) const override
        { return new number_integer(value); }
    void dump(std::string& buffer, int, int) const override
        { buffer += stralgo::strf(value); }
};

struct json::value::string : public json::value
{
    std::string value;

public:
    string(void) {}

    template <typename... Args>
    string(Args&&... args)
        : value(std::forward<Args>(args)...)
    {}

public:
    Type type(void) const override
        { return vtString; }
    valueptr copy(void) const override
        { return new string(value); }
    void dump(std::string& buffer, int, int) const override
        { dump_string(buffer, value); }

public:
    static void dump_string(std::string& buffer, const std::string& str) {
        buffer += '"';
        for (char ch : str) {
            switch (ch) {
            default:           buffer += ch;         break;
            case 0x22 /* " */: buffer += "\x5c\x22"; break;
            case 0x5c /* \ */: buffer += "\x5c\x5c"; break;
            case 0x08 /* b */: buffer += "\x5c\x62"; break;
            case 0x0c /* f */: buffer += "\x5c\x66"; break;
            case 0x0a /* n */: buffer += "\x5c\x6e"; break;
            case 0x0d /* r */: buffer += "\x5c\x72"; break;
            case 0x09 /* t */: buffer += "\x5c\x74"; break;
            }
        }
        buffer += '"';
    }
};

struct json::value::array : public json::value
{
    std::vector<valuesptr> value;

public:
    Type type(void) const override {
        return vtArray;
    }

    valueptr copy(void) const override {
        array* array = new struct array;
        for (const valuesptr& item : value)
            if (item) array->value.emplace_back(item->copy());
        return array;
    }

    void dump(std::string& buffer, int level, int indent) const override {
        bool hasitem = false, isfirst = true;
        buffer += '[';
        ++level;
        for (const valuesptr& item : value) {
            if (item) {
                hasitem = true;
                if (isfirst) isfirst = false; else buffer += ',';
                if (indent) buffer += '\n', buffer.append(level * indent, ' ');
                item->dump(buffer, level, indent);
            }
        }
        --level;
        if (indent && hasitem)
            buffer += '\n', buffer.append(level * indent, ' ');
        buffer += ']';
    }
};

struct json::value::object : public json::value
{
    typedef std::unordered_map<std::string, valuesptr> dict_type;
    typedef dict_type::value_type value_type;
    dict_type value;

public:
    unsigned int size(void) const {
        return std::accumulate(value.begin(), value.end(), 0,
            [](unsigned int sum, const value_type& pair) {
                return pair.second ? sum + 1 : sum;
            });
    }

public:
    Type type(void) const override {
        return vtObject;
    }

    valueptr copy(void) const override {
        object* object = new struct object;
        for (const value_type& pair : value)
            if (pair.second)
                object->value[pair.first].reset(pair.second->copy());
        return object;
    }

    void dump(std::string& buffer, int level, int indent) const override {
        bool hasitem = false, isfirst = true;
        buffer += '{';
        ++level;
        for (const value_type& pair : value) {
            if (pair.second) {
                hasitem = true;
                if (isfirst) isfirst = false; else buffer += ',';
                if (indent) buffer += '\n', buffer.append(level * indent, ' ');
                string::dump_string(buffer, pair.first);
                buffer += indent ? ": " : ":";
                pair.second->dump(buffer, level, indent);
            }
        }
        --level;
        if (indent && hasitem)
            buffer += '\n', buffer.append(level * indent, ' ');
        buffer += '}';
    }
};

class json::implement
{
    friend class json;

public:
    implement(valueptr   value) : value_(value) {}
    implement(valuesptr& value) : value_(value) {}

public:
    Type type(void)
        { return value() ? value()->type() : vtUndefined; }
    valuesptr& value(void)
        { return value_.real(); }
    template <typename T> T* value(void) {
        return dynamic_cast<T*>(value().get());
    }

public:
    void value (valueptr  valueptr ) { value(valuesptr(valueptr)); }
    void value (valuesptr valuesptr) { value() = valuesptr; }

    bool append(valueptr  valueptr ) { return append(valuesptr(valueptr)); }
    bool append(valuesptr valuesptr) {
        if (!valuesptr)
            return false;
        if (!value())
             value(new value::array());
        else if (type() != vtArray)
            return false;
        value<value::array>()->value.emplace_back(valuesptr);
        return true;
    }

public:
    static valueptr type2value(Type type) {
        switch (type) {
        default:              return nullptr;
        case vtNull:          return new value::null();
        case vtBoolTrue:      return new value::bool_true();
        case vtBoolFalse:     return new value::bool_false();
        case vtNumberDouble:  return new value::number_double(0);
        case vtNumberInteger: return new value::number_integer(0);
        case vtString:        return new value::string();
        case vtArray:         return new value::array();
        case vtObject:        return new value::object();
        }
    }

public:
    static json::valueptr       parse       (const stream& s);
    static json::valueptr       parse_number(const stream& s, char ch);
    static json::value::string* parse_string(const stream& s);
    static json::value::array*  parse_array (const stream& s);
    static json::value::object* parse_object(const stream& s);

public:
    static char read_char(const stream& s) {
        char ch;
        return s.read(ch) ? ch : 0;
    }

    static char read_char_nospace(const stream& s) {
        char ch;
        do { if (!s.read(ch)) return 0; } while (stralgo::isspace(ch));
        return ch;
    }

    static std::string read_chars(const stream& s, unsigned int n) {
        std::string chars(n, 0);
        for (unsigned int i = 0; i < n; ++i)
            chars[i] = read_char(s);
        return std::move(chars);
    }

private:
    struct Value {
        bool isref;
        valuesptr obj, *ref;
    public:
        Value(valueptr   value) : isref(false), obj( value) {}
        Value(valuesptr& value) : isref(true),  ref(&value) {}
    public:
        valuesptr& real(void) {
            return isref ? *ref : obj;
        }
    } value_;
};

json::valueptr json::implement::parse(const stream& s)
{
    typedef std::numeric_limits<double> double_limits;

    switch (char ch = read_char_nospace(s))
    {
    case 'n':
        {
            std::string chars = read_chars(s, 2);
            if (chars == "an")
                return new value::number_double(double_limits::quiet_NaN());
            chars += read_char(s);
            if (chars == "ull")
                return new value::null();
            return nullptr;
        }

    case 'i':
        if (read_chars(s, 2) == "nf")
            return new value::number_double(double_limits::infinity());
        return nullptr;

    case 't':
        if (read_chars(s, 3) == "rue")
            return new value::bool_true();
        return nullptr;

    case 'f':
        if (read_chars(s, 4) == "alse")
            return new value::bool_false();

    case '-':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        return parse_number(s, ch);

    case '"':
        return parse_string(s);

    case '[':
        return parse_array(s);

    case '{':
        return parse_object(s);

    default:
        return nullptr;
    };
}

json::valueptr json::implement::parse_number(const stream& s, char ch)
{
    std::string number;
    bool isdouble = false;

    number += ch;
    if (ch == '-')
        number += read_char(s);

    if (!stralgo::isdigit(number.back()))
        return nullptr;
    else if (number.back() != '0')
        while (stralgo::isdigit((ch = read_char(s))))
            number += ch;
    else
        ch = read_char(s);

    if (ch == '.')
    {
        isdouble = true;
        number += ch;
        while (stralgo::isdigit((ch = read_char(s))))
            number += ch;
    }

    if (ch == 'e' || ch == 'E')
    {
        isdouble = true;
        number += ch;
        ch = read_char(s);
        if (ch == '+' || ch == '-')
            number += ch;
        else if (!stralgo::isdigit(ch))
            return nullptr;
        else
            number += ch;
        while (stralgo::isdigit((ch = read_char(s))))
            number += ch;
    }

    s.seek(-1, stream::cur);

    if (isdouble)
        return new value::number_double(stralgo::atof(number.c_str()));
    else
        return new value::number_integer(stralgo::atoll(number.c_str()));
}

json::value::string* json::implement::parse_string(const stream& s)
{
    std::string str;

    while (true)
    {
        switch (char ch = read_char(s))
        {
        default: str += ch; break;
        case 0x5c:
            switch (char ch2 = read_char(s))
            {
            default:   str += ch; str += ch2; break;
            case 0x5c: str += 0x5c; break;
            case 0x22: str += 0x22; break;
            case 0x2f: str += 0x2f; break;
            case 0x62: str += 0x08; break;
            case 0x66: str += 0x0c; break;
            case 0x6e: str += 0x0a; break;
            case 0x72: str += 0x0d; break;
            case 0x74: str += 0x09; break;
            case 0x75: str += strtol(read_chars(s, 2).c_str(), nullptr, 16);
                       str += strtol(read_chars(s, 2).c_str(), nullptr, 16);
                break;
            }
            break;
        case '"': return new value::string(std::move(str));
        case  0 : return nullptr;
        }
    }
}

json::value::array* json::implement::parse_array(const stream& s)
{
    std::unique_ptr<value::array> array(new value::array());
    if (read_char_nospace(s) == ']')
        return array.release();
    else
        s.seek(-1, stream::cur);

    while (true)
    {
        json::valueptr value = parse(s);
        if (value)
            array->value.emplace_back(value);
        else
            return nullptr;

        switch (read_char_nospace(s))
        {
        case ']':
            return array.release();
        case ',':
            break;
        default:
            return nullptr;
        }
    }
}

json::value::object* json::implement::parse_object(const stream& s)
{
    std::unique_ptr<value::object> object(new value::object());
    std::unique_ptr<value::string> key;
    char ch = read_char_nospace(s);

    while (true)
    {
        switch (ch)
        {
        case '}':
            return object.release();
        case '"':
            key.reset(parse_string(s));
            if (!key || read_char_nospace(s) != ':')
        default:
                return nullptr;
        }

        json::valueptr value = parse(s);
        if (value)
            object->value[key->value].reset(value);
        else
            return nullptr;

        ch = read_char_nospace(s);
        if (ch == ',')
            ch = read_char_nospace(s);
    }
}

json::json(void)
    : json(valueptr())
{}

json::json(Type type)
    : json(implement::type2value(type))
{}

json::json(const std::string& buffer)
    : json()
{
    load(buffer);
}

json::json(const stream& istream)
    : json()
{
    load(istream);
}

json::json(valueptr value)
    : impl_(new implement(value))
{}

json::json(valuesptr& value)
    : impl_(new implement(value))
{}

json::json(json& jsonobj)
    : json(jsonobj.impl_->value())
{}

json::json(json&& jsonobj)
    : impl_(jsonobj.impl_)
{
    jsonobj.impl_ = nullptr;
}

json::json(const json& jsonobj)
    : json(jsonobj.impl_->value()->copy())
{}

json::~json(void)
{
    delete impl_;
}

bool json::load(const std::string& buffer)
{
    buffer_stream bufst(buffer.data(), buffer.size());
    valueptr value = implement::parse(bufst);

    if (value)
    {
        impl_->value(value);
        return true;
    }

    return false;
}

bool json::load(const stream& istream)
{
    valueptr value;

    if (istream.seekable())
        value = implement::parse(istream);
    else
        value = implement::parse(seek_stream(istream));

    if (value)
    {
        impl_->value(value);
        return true;
    }

    return false;
}

std::string json::dump(void) const
{
    return dump(0);
}

std::string json::dump(unsigned int indent) const
{
    std::string buffer;

    if (impl_->value())
        impl_->value()->dump(buffer, 0, indent);

    return std::move(buffer);
}

bool json::dump(stream& ostream) const
{
    return dump(ostream, 0);
}

bool json::dump(stream&& ostream) const
{
    return dump(ostream, 0);
}

bool json::dump(stream& ostream, unsigned int indent) const
{
    std::string buffer = dump(indent);

    return ostream.write(buffer.data(), buffer.size()) == buffer.size();
}

bool json::dump(stream&& ostream, unsigned int indent) const
{
    return dump(ostream, indent);
}

json& json::operator=(Type type)
{
    return impl_->value(implement::type2value(type)), *this;
}

json& json::operator=(std::nullptr_t)
{
    return impl_->value(new value::null()), *this;
}

json& json::operator=(bool boolean)
{
    if (boolean)
        impl_->value(new value::bool_true());
    else
        impl_->value(new value::bool_false());

    return *this;
}

json& json::operator=(int number)
{
    return impl_->value(new value::number_integer(number)), *this;
}

json& json::operator=(unsigned int number)
{
    return impl_->value(new value::number_integer(number)), *this;
}

json& json::operator=(long number)
{
    return impl_->value(new value::number_integer(number)), *this;
}

json& json::operator=(unsigned long number)
{
    return impl_->value(new value::number_integer(number)), *this;
}

json& json::operator=(long long number)
{
    return impl_->value(new value::number_integer(number)), *this;
}

json& json::operator=(unsigned long long number)
{
    return impl_->value(new value::number_integer(number)), *this;
}

json& json::operator=(double number)
{
    return impl_->value(new value::number_double(number)), *this;
}

json& json::operator=(char ch)
{
    return impl_->value(new value::string(1, ch)), *this;
}

json& json::operator=(const char* string)
{
    return impl_->value(new value::string(string)), *this;
}

json& json::operator=(const std::string& string)
{
    return impl_->value(new value::string(string)), *this;
}

json& json::operator=(json& jsonobj)
{
    return impl_->value(jsonobj.impl_->value()), *this;
}

json& json::operator=(json&& jsonobj)
{
    return operator=(jsonobj);
}

json& json::operator=(const json& jsonobj)
{
    return impl_->value(jsonobj.impl_->value()->copy()), *this;
}

bool json::append(Type type)
{
    return impl_->append(implement::type2value(type));
}

bool json::append(std::nullptr_t)
{
    return impl_->append(new value::null());
}

bool json::append(bool boolean)
{
    if (boolean)
        return impl_->append(new value::bool_true());
    else
        return impl_->append(new value::bool_false());
}

bool json::append(int number)
{
    return impl_->append(new value::number_integer(number));
}

bool json::append(unsigned int number)
{
    return impl_->append(new value::number_integer(number));
}

bool json::append(long number)
{
    return impl_->append(new value::number_integer(number));
}

bool json::append(unsigned long number)
{
    return impl_->append(new value::number_integer(number));
}

bool json::append(long long number)
{
    return impl_->append(new value::number_integer(number));
}

bool json::append(unsigned long long number)
{
    return impl_->append(new value::number_integer(number));
}

bool json::append(double number)
{
    return impl_->append(new value::number_double(number));
}

bool json::append(char ch)
{
    return impl_->append(new value::string(1, ch));
}

bool json::append(const char* string)
{
    return impl_->append(new value::string(string));
}

bool json::append(const std::string& string)
{
    return impl_->append(new value::string(string));
}

bool json::append(json& jsonobj)
{
    return impl_->append(jsonobj.impl_->value());
}

bool json::append(json&& jsonobj)
{
    return append(jsonobj);
}

bool json::append(const json& jsonobj)
{
    return impl_->append(jsonobj.impl_->value()->copy());
}

bool json::clear(void)
{
    switch (type())
    {
    default:       return false;
    case vtArray:  return impl_->value<value::array >()->value.clear(), true;
    case vtObject: return impl_->value<value::object>()->value.clear(), true;
    }
}

bool json::remove(unsigned int index)
{
    if (type() == vtArray)
    {
        std::vector<valuesptr>& array = impl_->value<value::array>()->value;
        if (index < array.size())
        {
            array.erase(array.begin() + index);
            return true;
        }
    }

    return false;
}

bool json::remove(const std::string& key)
{
    if (type() == vtObject)
    {
        impl_->value<value::object>()->value.erase(key);
        return true;
    }

    return false;
}

unsigned int json::size(void) const
{
    switch (type())
    {
    default:       return 0;
    case vtString: return impl_->value<value::string>()->value.size();
    case vtArray:  return impl_->value<value::array >()->value.size();
    case vtObject: return impl_->value<value::object>()->size();
    }
}

bool json::has_key(const std::string& key) const
{
    return type() == vtObject
        && impl_->value<value::object>()->value.find(key)
        != impl_->value<value::object>()->value.end();
}

std::vector<std::string> json::keys(void) const
{
    std::vector<std::string> keys;

    if (type() == vtObject)
    {
        typedef value::object::value_type value_type;
        for (const value_type& pair : impl_->value<value::object>()->value)
            keys.emplace_back(pair.first);
    }

    return std::move(keys);
}

json json::operator[](unsigned int index)
{
    if (type() == vtArray)
        return impl_->value<value::array>()->value[index];

    return json();
}

json json::operator[](const std::string& key)
{
    if (!impl_->value())
         impl_->value(new value::object());
    else if (type() != vtObject)
        return json();

    return impl_->value<value::object>()->value[key];
}

const json json::operator[](unsigned int index) const
{
    return const_cast<json*>(this)->operator[](index);
}

const json json::operator[](const std::string& key) const
{
    return const_cast<json*>(this)->operator[](key);
}

json json::copy(void) const
{
    return json(impl_->value()->copy());
}

json::Type json::type(void) const
{
    return impl_->type();
}

bool json::as_bool(void) const
{
    switch (type())
    {
    default:
    case vtBoolFalse:     return false;
    case vtBoolTrue:      return true;
    case vtNumberDouble:  return impl_->value<value::number_double>()->value;
    case vtNumberInteger: return impl_->value<value::number_integer>()->value;
    case vtString:        return impl_->value<value::string>()->value.size();
    case vtArray:         return impl_->value<value::array >()->value.size();
    case vtObject:        return impl_->value<value::object>()->size();
    }
}

double json::as_double(void) const
{
    switch (type())
    {
    default:
    case vtBoolFalse:     return 0.0;
    case vtBoolTrue:      return 1.0;
    case vtNumberDouble:  return impl_->value<value::number_double>()->value;
    case vtNumberInteger: return impl_->value<value::number_integer>()->value;
    }
}

long long json::as_integer(void) const
{
    switch (type())
    {
    default:
    case vtBoolFalse:     return 0;
    case vtBoolTrue:      return 1;
    case vtNumberDouble:  return impl_->value<value::number_double>()->value;
    case vtNumberInteger: return impl_->value<value::number_integer>()->value;
    }
}

std::string json::as_string(void) const
{
    #define value(T) \
        impl_->value<value::T>()->value

    switch (type())
    {
    default:              return "";
    case vtBoolTrue:      return "true";
    case vtBoolFalse:     return "false";
    case vtNumberDouble:  return stralgo::strf(value(number_double));
    case vtNumberInteger: return stralgo::strf(value(number_integer));
    case vtString:        return value(string);
    }
}
