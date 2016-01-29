#include <map>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <takisy/algorithm/stralgo.h>
#include <takisy/parser/json.h>

class json_error : public std::exception
{
public:
    json_error(const char* what)
        : what_(what)
    {}

    template <typename... Args>
    json_error(const char* format, const Args&... args)
    {
        what_ = stralgo::format(format, args...);
    }

    ~json_error(void)
    {}

public:
    const char* what(void) const noexcept override
    {
        return what_.c_str();
    }

private:
    std::string what_;
};

#define throwje(format, args...) \
    throw json_error(format, ##args)

class json::object
{
public:
    virtual ~object(void) {};

public:
    virtual ObjectType type(void) const = 0;
    virtual json::object*& element(int index) = 0;
    virtual json::object*& element(const char* key) = 0;
    virtual json::object* clone(void) const = 0;
    virtual bool dump(std::string& content, int level, int indent) const = 0;

protected:
    static inline std::string nspace(int level, int indent)
    {
        level = abs(level);
        level = level > 0 ? level - 1 : 0;
        return std::string(level * indent, 0x20);
    }

    static inline int inc(int level)
    {
        return level != 0 ? abs(level) + 1 : 0;
    }
};

class json::implement
{
    friend class json;

    struct null;
    struct boolean;
    struct number;
    struct string;
    struct array;
    struct dict;

public:
    implement(void) : object_() {}
    implement(object*& object) : object_(object) {}

public:
    static json::object* parse_null  (const stream& stream);
    static json::object* parse_true  (const stream& stream);
    static json::object* parse_false (const stream& stream);
    static json::object* parse_number(const stream& stream, int& ch);
    static json::object* parse_string(const stream& stream);
    static json::object* parse_array (const stream& stream);
    static json::object* parse_dict  (const stream& stream);
    static json::object* parse       (const stream& stream, int& ch);

public:
    static char* read(const stream& stream, char* buffer, unsigned int size)
    {
        if (stream.read(buffer, size) != size)
            throwje("stream error: read(%d).", size);

        return buffer;
    }

    static char read_char(const stream& stream, bool skip_space = false)
    {
        char ch;

        do { read(stream, &ch, sizeof(ch)); }
        while (skip_space && stralgo::isspace(ch));

        return ch;
    }

public:
    inline json::object*& obj(void)
    {
        return object_.isref ? *object_.u.ref : object_.u.obj;
    }


public:
    static inline std::string type_desc(json::ObjectType ot)
    {
        switch (ot)
        {
        case otNull:    return "null";
        case otBoolean: return "boolean";
        case otNumber:  return "number";
        case otString:  return "string";
        case otArray:   return "array";
        case otDict:    return "dict";
        default:        return "undefined";
        }
    }

private:
    static inline bool compare(const stream& stream, const char* desc)
    {
        int   length = strlen(desc);
        char* chars  = new char [length + 1];

        if (!chars) return false;
        else chars[length] = 0;

        bool result = strcmp(read(stream, chars, length), desc) == 0;
        delete [] chars;

        return result;
    }

private:
    struct david
    {
        bool isref;
        union { object* obj; object** ref; } u;

    public:
        david(void) { isref = false; u.obj = nullptr; }
        david(object*& object) { isref = true; u.ref = &object; }
       ~david(void) { if (!isref && !!u.obj) delete u.obj; }
    } object_;
    std::string error_info_;
};

struct json::implement::null : public json::object
{
public:
    ~null(void) {}

public:
    inline ObjectType type(void) const override
    {
        return otNull;
    }

    inline json::object*& element(int index) override
    {
        throwje("invalid member function: null::operator[](int).");
    }

    inline json::object*& element(const char* key) override
    {
        throwje("invalid member function: null::operator[](string).");
    }

    inline json::object* clone(void) const override
    {
        return new class null;
    }

    bool dump(std::string& content, int level, int indent) const override
    {
        content += nspace(level, indent) + "null";

        return true;
    }
};

struct json::implement::boolean : public json::object
{
    bool value;

public:
    boolean(void) : value(false) {}
    boolean(bool boolean) : value(boolean) {}
   ~boolean(void) {}

public:
    inline ObjectType type(void) const override
    {
        return otBoolean;
    }

    inline json::object*& element(int index) override
    {
        throwje("invalid member function: boolean::operator[](int).");
    }

    inline json::object*& element(const char* key) override
    {
        throwje("invalid member function: boolean::operator[](string).");
    }

    inline json::object* clone(void) const override
    {
        return new class boolean(value);
    }

    bool dump(std::string& content, int level, int indent) const override
    {
        content += nspace(level, indent) + (value ? "true" : "false");

        return true;
    }
};

struct json::implement::number : public json::object
{
    double value;

public:
    number(void) : value(0) {}
    number(double number) : value(number) {}
   ~number(void) {}

private:
    static inline char getchar_nothrow(const stream& stream)
    {
        try { return read_char(stream); }
        catch (...) { return 0; }
    }

public:
    char load(const stream& stream, int ch)
    {
        std::string desc;
        char c;

        if (ch < 0)
            c = read_char(stream, true);
        else
            c = ch;

        if (c == '-' or stralgo::isdigit(c))
            desc += c;
        else
            throwje("undefined number beginning: '%c'.", c);

        while (stralgo::isdigit((c = getchar_nothrow(stream))))
            desc += c;

        if (c == '.')
        {
            desc += c;
            while (stralgo::isdigit((c = getchar_nothrow(stream))))
                desc += c;
        }

        if (c == 'e' || c == 'E')
        {
            desc += c, c = getchar_nothrow(stream);

            if (c == '+' || c == '-' || stralgo::isdigit(c))
                desc += c;
            else
                throwje("undefined `e' sign: %c", c);

            while(stralgo::isdigit((c = getchar_nothrow(stream))))
                desc += c;
        }

        if (!desc.empty())
            value = strtod(desc.c_str(), nullptr);

        return c;
    }

public:
    inline ObjectType type(void) const override
    {
        return otNumber;
    }

    inline json::object*& element(int index) override
    {
        throwje("invalid member function: number::operator[](int).");
    }

    inline json::object*& element(const char* key) override
    {
        throwje("invalid member function: number::operator[](string).");
    }

    inline json::object* clone(void) const override
    {
        return new class number(value);
    }

    bool dump(std::string& content, int level, int indent) const override
    {
       content += nspace(level, indent) + stralgo::format("%.15g", value);

        return true;
    }
};

struct json::implement::string : public json::object
{
    std::string value;

public:
    string(void) {}
    string(const char* string) : value(string) {}
    string(const string& string) { operator=(string); }
   ~string(void) {}

    inline string& operator=(const string& string)
    {
        if (this != &string)
            value = string.value;

        return *this;
    }

public:
    inline bool operator<(const string& string) const
    {
        return value < string.value;
    }

public:
    void load(const stream& stream)
    {
        char ch4[5] = {0};
        wchar_t wch;
        char ch;

        while (true)
        {
            switch (ch = read_char(stream))
            {
            case 0x5c:
                switch (ch = read_char(stream))
                {
                case 0x5c: value += 0x5c; break; // '\'
                case 0x22: value += 0x22; break; // '"'
                case 0x2f: value += 0x2f; break; // '/'
                case 0x62: value += 0x08; break; // 'b'
                case 0x66: value += 0x0c; break; // 'f'
                case 0x6e: value += 0x0a; break; // 'n'
                case 0x72: value += 0x0d; break; // 'r'
                case 0x74: value += 0x09; break; // 't'
                case 0x75:                       // 'u'
                    wch = strtol(read(stream, ch4, 4), nullptr, 16);
                    value.append(reinterpret_cast<char*>(&wch), sizeof(wch));
                    break;
                default:
                    throwje("invalid ESC: '%c'.", ch);
                }
                break;
            case 0x22:
                return;
            default:
                value += ch;
                break;
            }
        }
    }

public:
    inline ObjectType type(void) const override
    {
        return otString;
    }

    inline json::object*& element(int index) override
    {
        throwje("invalid member function: string::operator[](int).");
    }

    inline json::object*& element(const char* key) override
    {
        throwje("invalid member function: string::operator[](string).");
    }

    inline json::object* clone(void) const override
    {
        class string* object = new class string;

        if (object)
            object->value = value;

        return object;
    }

    bool dump(std::string& content, int level, int indent) const override
    {
        std::string nonascii;

        content += nspace(level, indent);
        content += '"';

        for (char ch : value)
        {
            switch (ch)
            {
            case 0x5c /* \ */: content += "\x5c\x5c"; break;
            case 0x22 /* " */: content += "\x5c\x22"; break;
            case 0x08 /* b */: content += "\x5c\x62"; break;
            case 0x0c /* f */: content += "\x5c\x66"; break;
            case 0x0a /* n */: content += "\x5c\x6e"; break;
            case 0x0d /* r */: content += "\x5c\x72"; break;
            case 0x09 /* t */: content += "\x5c\x74"; break;
            default:
                if (ch & 0x80)
                    nonascii += ch;
                else
                {
                    dump_nonascii(content, nonascii);
                    content += ch;
                }
                break;
            }
        }

        dump_nonascii(content, nonascii);
        content += '"';

        return true;
    }

private:
    void dump_nonascii(std::string& content, const std::string& nonascii) const
    {
        for (unsigned int i = 0; i < nonascii.size(); i += 2)
            content += stralgo::format("\x5c\x75%x%x",
                                       (unsigned char)nonascii[i],
                                       (unsigned char)nonascii[i + 1]);

        if (nonascii.size() & 1)
            content += nonascii.back();
    }
};

struct json::implement::array : public json::object
{
    std::vector<json::object*> elements;

public:
    ~array(void)
    {
        for (json::object* element : elements)
            if (element)
                delete element;
    }

public:
    inline void append(json::object* object)
    {
        elements.push_back(object);
    }

    inline unsigned int count(void) const
    {
        return elements.size();
    }

public:
    inline ObjectType type(void) const override
    {
        return otArray;
    }

    inline json::object*& element(int index) override
    {
        if (static_cast<unsigned int>(index) >= elements.size())
            throwje("index(%d) out of range[0, %d).", index, elements.size());

        return elements[index];
    }

    inline json::object*& element(const char* key) override
    {
        throwje("invalid member function: array::operator[](string).");
    }

    inline json::object* clone(void) const override
    {
        class array* object = new class array;

        if (object)
            for (json::object* element : elements)
                object->elements.push_back
                        (element ? element->clone() : nullptr);

        return object;
    }

    bool dump(std::string& content, int level, int indent) const override
    {
        std::string next_line(level == 0 ? "" : "\n");
        int inc_level = inc(level);
        unsigned int count = 0;

        if (level > 0)
            content += nspace(level, indent);
        content += '[';
        if (elements.empty())
            return content += ']', true;
        content += next_line;

        for (json::object* element : elements)
        {
            if (!element)
                continue;

            std::string element_content;
            if (!element->dump(element_content, inc_level, indent))
                return false;

            content += element_content;
            if (++count != elements.size())
                content += ", ";
            content += next_line;
        }

        content += nspace(level, indent);
        content += ']';

        return true;
    }
};

struct json::implement::dict : public json::object
{
    std::map<string, json::object*> pairs;

public:
    ~dict(void)
    {
        for (auto& pair : pairs)
            if (pair.second)
                delete pair.second;
    }

public:
    std::vector<std::string> keys(void) const
    {
        std::vector<std::string> keys;

        for (auto& pair : pairs)
            keys.push_back(pair.first.value);

        return keys;
    }

public:
    inline ObjectType type(void) const override
    {
        return otDict;
    }

    inline json::object*& element(int index) override
    {
        throwje("invalid member function: dict::operator[](int).");
    }

    inline json::object*& element(const char* key) override
    {
        return pairs[key];
    }

    inline json::object* clone(void) const override
    {
        class dict* object = new class dict;

        if (object)
            for (auto& pair : pairs)
                object->pairs[pair.first] =
                        pair.second ? pair.second->clone() : nullptr;

        return object;
    }

    bool dump(std::string& content, int level, int indent) const override
    {
        std::string next_line(level == 0 ? "" : "\n");
        int inc_level = inc(level);
        unsigned int count = 0;

        if (level > 0)
            content += nspace(level, indent);
        content += '{';
        if (pairs.empty())
            return content += '}', true;
        content += next_line;

        for (auto& pair : pairs)
        {
            if (!pair.second)
                continue;

            ObjectType type = pair.second->type();
            bool complex_type  = (type == otArray || type == otDict);
            int  complex_level = complex_type ? inc_level : 0;
            std::string key_content, value_content;

            if (   !pair.first.dump(key_content, inc_level, indent)
                || !pair.second->dump(value_content, -complex_level, indent))
                return false;

            content += stralgo::format
                ("%s: %s", key_content.c_str(), value_content.c_str());
            if (++count != pairs.size())
                content += ", ";
            content += next_line;
        }

        content += nspace(level, indent);
        content += '}';

        return true;
    }
};

json::object* json::implement::parse_null(const stream& stream)
{
    if (!compare(stream, "ull"))
        throwje("failed to parse null.");

    return new null;
}

json::object* json::implement::parse_true(const stream& stream)
{
    if (!compare(stream, "rue"))
        throwje("failed to parse boolean:true.");

    return new boolean(true);
}

json::object* json::implement::parse_false(const stream& stream)
{
    if (!compare(stream, "alse"))
        throwje("failed to parse boolean:false.");

    return new boolean(false);
}

json::object* json::implement::parse_number(const stream& stream, int& ch)
{
    number* number = new implement::number;

    if (number)
        ch = number->load(stream, ch);
    else
        ch = -1;

    return number;
}

json::object* json::implement::parse_string(const stream& stream)
{
    string* string = new implement::string;

    if (string)
        string->load(stream);

    return string;
}

json::object* json::implement::parse_array(const stream& stream)
{
    array* array = new implement::array;
    if (!array)
        return nullptr;

    int ch = read_char(stream, true);
    if (ch == ']')
        return array;

    while (true)
    {
        json::object* element = parse(stream, ch);
        array->elements.push_back(element);

        if (element->type() != otNumber || stralgo::isspace(ch))
            ch = read_char(stream, true);
        if (ch == ']') break; else
        if (ch == ',') ch=-1; else throwje("failed to parse array.");
    }

    return array;
}

json::object* json::implement::parse_dict(const stream& stream)
{
    dict* dict = new implement::dict;
    if (!dict)
        return nullptr;

    int ch = read_char(stream, true);
    if (ch == '}')
        return dict;

    while (true)
    {
        json::object* key = parse(stream, ch);
        if (key->type() != otString)
            throwje("dict's key must be string.");

        char colon = read_char(stream, true);
        if (colon != ':')
            throwje("colon couldn't be find between key and value.");

        json::object* value = parse(stream, ch);
        dict->pairs[*dynamic_cast<string*>(key)] = value;
        delete key;

        if (value->type() != otNumber || stralgo::isspace(ch))
            ch = read_char(stream, true);
        if (ch == '}') break; else
        if (ch == ',') ch=-1; else throwje("failed to parse dict.");
    }

    return dict;
}

json::object* json::implement::parse(const stream& stream, int& ch)
{
    json::object* object = nullptr;

    if (ch < 0)
        ch = implement::read_char(stream, true);

    switch (ch)
    {
    case  0 : throwje("parse ERROR.");
    case 'n': object = implement::parse_null  (stream); ch = -1; break;
    case 't': object = implement::parse_true  (stream); ch = -1; break;
    case 'f': object = implement::parse_false (stream); ch = -1; break;
    case '"': object = implement::parse_string(stream); ch = -1; break;
    case '[': object = implement::parse_array (stream); ch = -1; break;
    case '{': object = implement::parse_dict  (stream); ch = -1; break;
    default : object = implement::parse_number(stream , ch    ); break;
    }

    return object;
}

json::json(void)
    : impl_(new implement)
{}

json::json(const char* content)
    : json()
{
    load(content);
}

json::json(const stream& stream)
    : json()
{
    load(stream);
}

json::json(const json& _json)
    : json()
{
    operator=(_json);
}

json::json(object*& object)
    : impl_(new implement(object))
{}

json::~json(void)
{
    delete impl_;
}

bool json::load(const char* content)
{
    return load(buffer_stream(content, strlen(content)));
}

bool json::load(const stream& stream)
{
    try
    {
        int ch = -1;
        return !!(impl_->obj() = implement::parse(stream, ch));
    }
    catch (const char* error_info)
    {
        impl_->error_info_ = error_info;
        return false;
    }
}

bool json::load_file(const char* file_path)
{
    return load(file_stream(file_path, "r"));
}

std::string json::dump(void) const
{
    return dump(0);
}

std::string json::dump(int indent) const
{
    if (!impl_->obj())
        return "";

    std::string content;

    if (!impl_->obj()->dump(content, indent > 0 ? 1 : 0, indent))
        throwje("failed to dump json to string.");
    else
        content += '\n';

    return content;
}

bool json::dump(stream& stream) const
{
    return dump(stream, 0);
}

bool json::dump(stream& stream, int indent) const
{
    std::string content = dump(indent);

    return stream.write(content.data(), content.size()) == content.size();
}

bool json::dump_file(const char* file_path) const
{
    return dump_file(file_path, 0);
}

bool json::dump_file(const char* file_path, int indent) const
{
    file_stream fstream(file_path, "w");

    return dump(fstream, indent);
}

json& json::operator=(std::nullptr_t)
{
    if (impl_->obj())
        delete impl_->obj();

    impl_->obj() = new implement::null;

    return *this;
}

json& json::operator=(bool boolean)
{
    if (impl_->obj())
        delete impl_->obj();

    impl_->obj() = new implement::boolean(boolean);

    return *this;
}

json& json::operator=(int number)
{
    return operator=(static_cast<double>(number));
}

json& json::operator=(long long number)
{
    return operator=(static_cast<double>(number));
}

json& json::operator=(double number)
{
    if (impl_->obj())
        delete impl_->obj();

    impl_->obj() = new implement::number(number);

    return *this;
}

json& json::operator=(const char* string)
{
    if (impl_->obj())
        delete impl_->obj();

    impl_->obj() = new implement::string(string);

    return *this;
}

json& json::operator=(ObjectType object_type)
{
    switch (object_type)
    {
    case otNull:    return operator=(nullptr);
    case otBoolean: return operator=(false);
    case otNumber:  return operator=(0.0);
    case otString:  return operator=("");
    default: break;
    }

    if (impl_->obj())
        delete impl_->obj();

    switch (object_type)
    {
    case otArray: impl_->obj() = new implement::array(); break;
    case otDict:  impl_->obj() = new implement::dict();  break;
    default: throwje("invalid object type.");
    }

    return *this;
}

json& json::operator=(const json& json)
{
    if (impl_->obj() != json.impl_->obj())
    {
        if (impl_->obj())
            delete impl_->obj();

        if (!json.impl_->obj())
            impl_->obj() = json.impl_->obj()->clone();
        else
            impl_->obj() = nullptr;

        impl_->error_info_ = json.impl_->error_info_;
    }

    return *this;
}

void json::append(std::nullptr_t)
{
    if (type() != otArray)
        throwje("invalid member function: %s::append(null).",
                implement::type_desc(type()).c_str());

    dynamic_cast<implement::array*>(impl_->obj())
        ->append(new implement::null);
}

void json::append(bool boolean)
{
    if (type() != otArray)
        throwje("invalid member function: %s::append(boolean).",
                implement::type_desc(type()).c_str());

    dynamic_cast<implement::array*>(impl_->obj())
        ->append(new implement::boolean(boolean));
}

void json::append(int number)
{
    append(static_cast<double>(number));
}

void json::append(long long number)
{
    append(static_cast<double>(number));
}

void json::append(double number)
{
    if (type() != otArray)
        throwje("invalid member function: %s::append(number).",
                implement::type_desc(type()).c_str());

    dynamic_cast<implement::array*>(impl_->obj())
        ->append(new implement::number(number));
}

void json::append(const char* string)
{
    if (type() != otArray)
        throwje("invalid member function: %s::append(string).",
                implement::type_desc(type()).c_str());

    dynamic_cast<implement::array*>(impl_->obj())
        ->append(new implement::string(string));
}

void json::append(ObjectType object_type)
{
    if (type() != otArray)
        throwje("invalid member function: %s::append(object).",
                implement::type_desc(type()).c_str());

    object* object = nullptr;

    switch (object_type)
    {
    case otNull:    object = new implement::null;    break;
    case otBoolean: object = new implement::boolean; break;
    case otNumber:  object = new implement::number;  break;
    case otString:  object = new implement::string;  break;
    case otArray:   object = new implement::array;   break;
    case otDict:    object = new implement::dict;    break;
    default: throwje("Invalid object type.");
    }

    dynamic_cast<implement::array*>(impl_->obj())->append(object);
}

unsigned int json::count(void) const
{
    if (type() != otArray)
        throwje("invalid member function: %s::count(void).",
                implement::type_desc(type()).c_str());

    return dynamic_cast<implement::array*>(impl_->obj())->count();
}

json json::operator[](int index) const
{
    return impl_->obj()->element(index);
}

std::vector<std::string> json::keys(void) const
{
    if (type() != otDict)
        throwje("invalid member function: %s::keys(void).",
                implement::type_desc(type()).c_str());

    return dynamic_cast<implement::dict*>(impl_->obj())->keys();
}

json json::operator[](const char* key) const
{
    if (impl_->obj() == nullptr)
        impl_->obj() = new implement::dict;

    return impl_->obj()->element(key);
}

json::ObjectType json::type(void) const
{
    return !impl_->obj() ? otUndefined : impl_->obj()->type();
}

bool json::as_bool(void) const
{
    object* object = impl_->obj();
    if (!object)
        return false;

    switch (object->type())
    {
    case otUndefined:
    case otNull:
        return false;
    case otBoolean:
        return  dynamic_cast<implement::boolean*>(object)->value;
    case otNumber:
        return !dynamic_cast<implement::number*> (object)->value;
    case otString:
        return !dynamic_cast<implement::string*> (object)->value.empty();
    case otArray:
        return !dynamic_cast<implement::array*>  (object)->elements.empty();
    case otDict:
        return !dynamic_cast<implement::dict*>   (object)->pairs.empty();
    default:
        throwje("invalid object type.");
    }
}

double json::as_number(void) const
{
    if (impl_->obj()->type() != otNumber)
        throwje("json object is not number type.");

    return dynamic_cast<implement::number*>(impl_->obj())->value;
}

const char* json::as_string(void) const
{
    if (impl_->obj()->type() != otString)
        throwje("json object is not string type.");

    return dynamic_cast<implement::string*>(impl_->obj())->value.c_str();
}

const char* json::error_info(void) const
{
    return impl_->error_info_.c_str();
}
