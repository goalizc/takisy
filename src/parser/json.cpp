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
    struct boolean;
    struct number_double;
    struct number_integer;
    struct string;
    struct array;
    struct object;

public:
    virtual ~value(void)
    {}

public:
    virtual valuesptr& get(unsigned int index)
    {
        throw std::logic_error(
            stralgo::sprintf("json type(%d) is not array(6).", type())
        );
    }

    virtual valuesptr& get(const std::string& key)
    {
        throw std::logic_error(
            stralgo::sprintf("json type(%d) is not object(7).", type())
        );
    }

    virtual Type type(void) const = 0;
    virtual valueptr copy(void) const = 0;
    virtual void dump(std::string& buffer, int level, int indent) const = 0;
};

struct json::value::null : public value
{
    Type type(void) const override
    {
        return vtNull;
    }

    valueptr copy(void) const override
    {
        return new null();
    }

    void dump(std::string& buffer, int, int) const override
    {
        buffer += "null";
    }
};

struct json::value::boolean : public value
{
    bool value;

public:
    boolean(bool value)
        : value(value)
    {}

public:
    Type type(void) const override
    {
        return vtBoolean;
    }

    valueptr copy(void) const override
    {
        return new boolean(value);
    }

    void dump(std::string& buffer, int, int) const override
    {
        buffer += value ? "true" : "false";
    }
};

struct json::value::number_double : public value
{
    double value;

public:
    number_double(double value)
        : value(value)
    {}

public:
    Type type(void) const override
    {
        return vtNumberDouble;
    }

    valueptr copy(void) const override
    {
        return new number_double(value);
    }

    void dump(std::string& buffer, int, int) const override
    {
        buffer += stralgo::strf(value);
    }
};

struct json::value::number_integer : public value
{
    long long value;

public:
    number_integer(long long value)
        : value(value)
    {}

public:
    Type type(void) const override
    {
        return vtNumberInteger;
    }

    valueptr copy(void) const override
    {
        return new number_integer(value);
    }

    void dump(std::string& buffer, int, int) const override
    {
        buffer += stralgo::strf(value);
    }
};

struct json::value::string : public value
{
    std::string value;

public:
    string(const std::string& value)
        : value(value)
    {}

public:
    Type type(void) const override
    {
        return vtString;
    }

    valueptr copy(void) const override
    {
        return new string(value);
    }

    void dump(std::string& buffer, int, int) const override
    {
        dump_string(buffer, value);
    }

public:
    static void dump_string(std::string& buffer, const std::string& str)
    {
        buffer += '"';

        for (unsigned int i = 0; i < str.size(); ++i)
        {
            switch (str[i])
            {
            default: buffer += str[i]; break;

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

struct json::value::array : public value
{
    std::vector<valuesptr> value;

public:
    ~array(void)
    {
        for (valuesptr& item : value)
            item.reset();
    }

public:
    valuesptr& get(unsigned int index) override
    {
        return value[index];
    }

    Type type(void) const override
    {
        return vtArray;
    }

    valueptr copy(void) const override
    {
        array* array = new struct array;

        for (const valuesptr& item : value)
            array->value.emplace_back(item->copy());

        return array;
    }

    void dump(std::string& buffer, int level, int indent) const override
    {
        bool has_value = false, is_first = true;
        buffer += '[';
        ++level;
        for (const valuesptr& item : value)
        {
            if (!item) continue;
            else has_value = true;
            if (is_first) is_first = false;
            else buffer += ", ";
            if (indent > 0)
                buffer += '\n', buffer.append(level * indent, ' ');
            item->dump(buffer, level, indent);
        }
        --level;
        if (indent > 0 && has_value)
            buffer += '\n', buffer.append(level * indent, ' ');
        buffer += ']';
    }
};

struct json::value::object : public value
{
    typedef std::unordered_map<std::string, valuesptr> container;
    typedef container::value_type value_type;

    container value;

public:
    ~object(void)
    {
        for (value_type& pair : value)
            pair.second.reset();
    }

public:
    unsigned int realsize(void) const
    {
        return std::accumulate(value.begin(), value.end(), 0,
            [](unsigned int sum, const value_type& pair)
            {
                return pair.second ? sum + 1 : sum;
            });
    }

public:
    valuesptr& get(const std::string& key) override
    {
        return value[key];
    }

    Type type(void) const override
    {
        return vtObject;
    }

    valueptr copy(void) const override
    {
        object* object = new struct object;

        for (const value_type& pair : value)
            object->value[pair.first] = valuesptr(pair.second->copy());

        return object;
    }

    void dump(std::string& buffer, int level, int indent) const override
    {
        bool has_value = false, is_first = true;
        buffer += '{';
        ++level;
        for (const value_type& pair : value)
        {
            if (!pair.second) continue;
            else has_value = true;
            if (is_first) is_first = false;
            else buffer += ", ";
            if (indent > 0)
                buffer += '\n', buffer.append(level * indent, ' ');
            string::dump_string(buffer, pair.first);
            buffer += ": ";
            pair.second->dump(buffer, level, indent);
        }
        --level;
        if (indent > 0 && has_value)
            buffer += '\n', buffer.append(level * indent, ' ');
        buffer += '}';
    }
};

class json::implement
{
    friend class json;

public:
    implement(void) : value_() {}
    implement(valuesptr& value) : value_(value) {}

public:
    valuesptr& value(void) { return value_.value(); }
    const valuesptr& value(void) const { return value_.value(); }
    template <typename T> T* value(void) const
    {
        return dynamic_cast<T*>(value().get());
    }

    bool isref(void) const { return value_.isref;   }
    Type type (void) const { return value() ? value()->type() : vtUndefined; }

public:
    void value (valueptr  valueptr ) { value(valuesptr(valueptr)); }
    void value (valuesptr valuesptr) { value() = valuesptr; }
    bool append(valueptr  valueptr ) { return append(valuesptr(valueptr)); }
    bool append(valuesptr valuesptr)
    {
        if (!value())
            value(new value::array());
        else if (type() != vtArray)
            return false;

        if (!valuesptr)
            return false;
        else
            return value<value::array>()->value.push_back(valuesptr), true;
    }

public:
    static valueptr type2value(Type type)
    {
        switch (type)
        {
        default:
            return nullptr;
        case vtNull:
            return new value::null();
        case vtBoolean:
            return new value::boolean(false);
        case vtNumberDouble:
            return new value::number_double(0);
        case vtNumberInteger:
            return new value::number_integer(0);
        case vtString:
            return new value::string("");
        case vtArray:
            return new value::array();
        case vtObject:
            return new value::object();
        }
    }

public:
    static json::valueptr       parse(const stream& s);
    static json::valueptr       parse_number(const stream& s);
    static json::value::string* parse_string(const stream& s);
    static json::value::array*  parse_array(const stream& s);
    static json::value::object* parse_object(const stream& s);

public:
    template <bool skip_space>
    static char read_char(const stream& s);

private:
    struct Value {
        bool isref;
        valuesptr obj, *ref;
    public:
        Value(void) : isref(false), obj(nullptr) {}
        Value(valuesptr& value) : isref(true), ref(&value) {}
    public:
        valuesptr& value(void) { return isref ? *ref : obj; }
        const valuesptr& value(void) const { return isref ? *ref : obj; }
    } value_;
};

template <>
char json::implement::read_char<false>(const stream& s)
{
    char ch;

    return s.read(ch) ? ch : 0;
}

template <>
char json::implement::read_char<true>(const stream& s)
{
    char ch;

    while (true)
        if (!s.read(ch))
            return 0;
        else if (!stralgo::isspace(ch))
            break;

    return ch;
}

json::valueptr json::implement::parse(const stream& s)
{
    typedef std::numeric_limits<double> double_limits;

    switch (read_char<true>(s))
    {
    case 'n':
        if (s.read_chars(3) == "ull")
            return new value::null();
        else
            s.seek(-3, stream::stCurrent);
        if (s.read_chars(2) == "an")
            return new value::number_double(double_limits::quiet_NaN());
        return nullptr;

    case 'i':
        if (s.read_chars(2) == "nf")
            return new value::number_double(double_limits::infinity());
        return nullptr;

    case 't':
        if (s.read_chars(3) == "rue")
            return new value::boolean(true);
        return nullptr;

    case 'f':
        if (s.read_chars(4) == "alse")
            return new value::boolean(false);

    case 0:
        return nullptr;

    default:
        s.seek(-1, stream::stCurrent);
        return parse_number(s);

    case '"':
        return parse_string(s);

    case '[':
        return parse_array(s);

    case '{':
        return parse_object(s);

    };

    return nullptr;
}

json::valueptr json::implement::parse_number(const stream& s)
{
    std::string number;
    bool is_double = false;
    char ch;

    ch = read_char<false>(s);
    number += ch;
    if (ch == '-')
        number += read_char<false>(s);

    if (!stralgo::isdigit(number.back()))
        return nullptr;
    else if (number.back() != '0')
        while (stralgo::isdigit((ch = read_char<false>(s))))
            number += ch;
    else
        ch = read_char<false>(s);

    if (ch == '.')
    {
        is_double = true;
        number += ch;
        while (stralgo::isdigit((ch = read_char<false>(s))))
            number += ch;
    }

    if (ch == 'e' || ch == 'E')
    {
        is_double = true;
        number += ch;
        ch = read_char<false>(s);
        if (ch == '+' || ch == '-')
            number += ch;
        else if (!stralgo::isdigit(ch))
            return nullptr;
        else
            number += ch;
        while (stralgo::isdigit((ch = read_char<false>(s))))
            number += ch;
    }

    s.seek(-1, stream::stCurrent);

    if (is_double)
        return new value::number_double(stralgo::atof(number.c_str()));
    else
        return new value::number_integer(stralgo::atoll(number.c_str()));
}

json::value::string* json::implement::parse_string(const stream& s)
{
    std::string str;
    char ch, ch2;

    while (true)
    {
        switch ((ch = read_char<false>(s)))
        {
        default: str += ch; break;
        case 0x5c:
            switch ((ch2 = read_char<false>(s)))
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
            case 0x75: str += strtol(s.read_chars(2).c_str(), nullptr, 16);
                       str += strtol(s.read_chars(2).c_str(), nullptr, 16);
                break;
            }
            break;
        case  0 : return nullptr;
        case '"': return new value::string(str);
        }
    }
}

json::value::array* json::implement::parse_array(const stream& s)
{
    std::unique_ptr<value::array> array(new value::array());
    if (read_char<true>(s) == ']')
        return array.release();
    else
        s.seek(-1, stream::stCurrent);

    while (true)
    {
        json::valueptr value = parse(s);
        if (!value)
            return nullptr;
        else
            array->value.push_back(valuesptr(value));

        switch (read_char<true>(s))
        {
        default:
            return nullptr;
        case ']':
            return array.release();
        case ',':
            break;
        }
    }
}

json::value::object* json::implement::parse_object(const stream& s)
{
    std::unique_ptr<value::object> object(new value::object());
    std::unique_ptr<value::string> key;
    char ch = read_char<true>(s);

    while (true)
    {
        switch (ch)
        {
        case '"':
            key.reset(parse_string(s));
            if (!key)
        default:
                return nullptr;
            break;
        case '}':
            return object.release();
        }

        if (read_char<true>(s) != ':')
            return nullptr;

        json::valueptr value = parse(s);
        if (!value)
            return nullptr;
        else
            object->value[key->value] = valuesptr(value);

        ch = read_char<true>(s);
        if (ch == ',')
            ch = read_char<true>(s);
    }
}

json::json(void)
    : impl_(new implement)
{}

json::json(Type type)
    : json()
{
    impl_->value(implement::type2value(type));
}

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

json::json(valuesptr& value)
    : impl_(new implement(value))
{}

json::json(const json& jsonobj)
    : json()
{
    impl_->value(jsonobj.impl_->value());
}

json::~json(void)
{
    delete impl_;
}

bool json::load(const std::string& buffer)
{
    buffer_stream bufst(buffer.data(), buffer.size());
    valueptr value = implement::parse(bufst);

    if (!value)
        return false;
    else
        impl_->value(value);

    return true;
}

bool json::load(const stream& istream)
{
    valueptr value;

    if (istream.seekable())
        value = implement::parse(istream);
    else
        value = implement::parse(seek_stream(istream));

    if (!value)
        return false;
    else
        impl_->value(value);

    return true;
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

    return buffer;
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
    return impl_->value(new value::boolean(boolean)), *this;
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

json& json::operator=(const char* string)
{
    return impl_->value(new value::string(string)), *this;
}

json& json::operator=(const std::string& string)
{
    return impl_->value(new value::string(string)), *this;
}

json& json::operator=(const json& json)
{
    if (this != &json && impl_->value() != json.impl_->value())
        impl_->value(json.impl_->value());

    return *this;
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
    return impl_->append(new value::boolean(boolean));
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

bool json::append(const char* string)
{
    return impl_->append(new value::string(string));
}

bool json::append(const std::string& string)
{
    return impl_->append(new value::string(string));
}

bool json::append(const json& json)
{
    return impl_->append(json.impl_->value());
}

bool json::clear(void)
{
    switch (impl_->type())
    {
    default:
        return false;
    case vtArray:
        impl_->value<value::array>()->value.clear();
        return true;
    case vtObject:
        impl_->value<value::object>()->value.clear();
        return true;
    }
}

bool json::remove(unsigned int index)
{
    if (impl_->type() == vtArray)
    {
        value::array* array = impl_->value<value::array>();

        if (index < array->value.size())
        {
            array->value.erase(array->value.begin() + index);
            return true;
        }
    }

    return false;
}

bool json::remove(const std::string& key)
{
    if (impl_->type() == vtObject)
    {
        impl_->value<value::object>()->value.erase(key);
        return true;
    }

    return false;
}

unsigned int json::size(void) const
{
    switch (impl_->type())
    {
    default:
        return 0;
    case vtString:
        return impl_->value<value::string>()->value.size();
    case vtArray:
        return impl_->value<value::array>()->value.size();
    case vtObject:
        return impl_->value<value::object>()->realsize();
    }
}

json json::operator[](const json& js)
{
    switch (js.impl_->type())
    {
    default:
        return json();
    case vtNumberDouble:
        return operator[](js.impl_->value<value::number_double>()->value);
    case vtNumberInteger:
        return operator[](js.impl_->value<value::number_integer>()->value);
    case vtString:
        return operator[](js.impl_->value<value::string>()->value);
    }
}

json json::operator[](unsigned int index)
{
    if (impl_->type() == vtObject)
    {
        typedef value::object::value_type value_type;

        for (const value_type& pair : impl_->value<value::object>()->value)
        {
            if (pair.second && index-- == 0)
            {
                json json;

                json["key"]   = pair.first;
                json["value"] = operator[](pair.first);

                return json;
            }
        }

        return json();
    }
    else
    {
        if (!impl_->value())
            impl_->value(new value::array());

        return impl_->value()->get(index);
    }
}

json json::operator[](const std::string& key)
{
    if (!impl_->value())
        impl_->value(new value::object());

    return impl_->value()->get(key);
}

const json json::operator[](const json& js) const
{
    return const_cast<json*>(this)->operator[](js);
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
    json copy;

    copy.impl_->value(impl_->value()->copy());

    return copy;
}

json::Type json::type(void) const
{
    return impl_->type();
}

bool json::as_bool(void) const
{
    switch (impl_->type())
    {
    default:
        return false;
    case vtBoolean:
        return impl_->value<value::boolean>()->value;
    case vtNumberDouble:
        return impl_->value<value::number_double>()->value;
    case vtNumberInteger:
        return impl_->value<value::number_integer>()->value;
    case vtString:
        return impl_->value<value::string>()->value.size();
    case vtArray:
        return impl_->value<value::array>()->value.size();
    case vtObject:
        return impl_->value<value::object>()->realsize();
    }
}

double json::as_double(void) const
{
    switch (impl_->type())
    {
    default:
        return 0;
    case vtBoolean:
        return impl_->value<value::boolean>()->value;
    case vtNumberDouble:
        return impl_->value<value::number_double>()->value;
    case vtNumberInteger:
        return impl_->value<value::number_integer>()->value;
    }
}

long long json::as_integer(void) const
{
    switch (impl_->type())
    {
    default:
        return 0;
    case vtBoolean:
        return impl_->value<value::boolean>()->value;
    case vtNumberDouble:
        return impl_->value<value::number_double>()->value;
    case vtNumberInteger:
        return impl_->value<value::number_integer>()->value;
    }
}

std::string json::as_string(void) const
{
    switch (impl_->type())
    {
    default:
        return "";
    case vtBoolean:
        return impl_->value<value::boolean>()->value ? "true" : "false";
    case vtNumberDouble:
        return stralgo::strf(impl_->value<value::number_double>()->value);
    case vtNumberInteger:
        return stralgo::strf(impl_->value<value::number_integer>()->value);
    case vtString:
        return impl_->value<value::string>()->value;
    }
}
