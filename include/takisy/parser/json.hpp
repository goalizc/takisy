#ifndef json_hpp_20180720
#define json_hpp_20180720

#include <cstring>
#include <cstdio>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

class json {
    class mempool {
        struct block {
            unsigned long size;
            mutable unsigned char *data;
        public:
            block(void)
                : size(0), data(nullptr) {}
            block(unsigned long size)
                : size(size), data((unsigned char*)malloc(size)) {}
            block(const block& mb)
                : size(mb.size), data(mb.data) { mb.data = nullptr; }
           ~block(void) { free(data); }
        };

    public:
        mempool(unsigned long initsize = 1024)
            : i_(0)
        {
            blocks_.emplace_back(initsize);
        }

    public:
        void reset(void) {
            i_ = 0;
            blocks_.resize(1);
        }

        template <typename T, typename... V>
        T* alloc(V&&... v) {
            if (blocks_.back().size - i_ < sizeof(T)) {
                i_ = 0;
                blocks_.emplace_back(blocks_.back().size << 1);
            }

            T* ptr = new (blocks_.back().data + i_) T(std::forward<V>(v)...);
            i_ += sizeof(T);

            return ptr;
        }

    private:
        unsigned long i_;
        std::vector<block> blocks_;
    };

    class strstream {
    public:
        strstream(const std::string& str)
            : i_(0), str_(str)
        {}

    public:
        void take(void) { ++i_; }
        void unget(void) { --i_; }

        char peek(void) { return str_[i_]; }
        char peek_nospace(void) {
            while (isspace(peek()))
                take();
            return peek();
        }

        char get(void) { return str_[i_++]; }
        char get_nospace(void) {
            char ch = peek_nospace();
            ++i_;
            return ch;
        }

        bool expect(char ch) {
            if (peek() == ch) return take(), true;
            return false;
        }

        template <int N>
        bool expect(const char (&str)[N]) {
            for (int i = 0; i < N - 1; ++i)
                if (str[i] != str_[i_ + i]) return false;
            i_ += N - 1;
            return true;
        }

        std::string::const_iterator milestone(void) const
            { return str_.begin() + i_; }
        std::string substr(std::string::const_iterator milestone) const
            { return std::string(milestone, str_.begin() + i_); }

    private:
        unsigned long i_;
        const std::string& str_;
    };

public:
    enum value_type {
        vtNull,
        vtBoolTrue,
        vtBoolFalse,
        vtNumberDouble,
        vtNumberInteger,
        vtString,
        vtArray,
        vtObject
    };

    struct value {
        void* v;
        value_type t;

    public:
        value(value_type type, void* value = nullptr)
            : v(value), t(type)
        {}

       ~value(void) {
            switch (t) {
            case vtString:
                reinterpret_cast<string*>(v)->~string();
                break;
            case vtArray:
                for (value* value : *reinterpret_cast<array*>(v))
                    value->~value();
                reinterpret_cast<array*>(v)->~array();
                break;
            case vtObject:
                for (object::value_type& pair : *reinterpret_cast<object*>(v)) {
                    pair.first->~value();
                    pair.second->~value();
                }
                reinterpret_cast<object*>(v)->~object();
                break;
            default:
                break;
            }
        }

    public:
        void dump(std::string& buffer, int level, int indent) const {
            char ch64[64];

            switch (t) {
            case vtNull:
                buffer += "null";
                break;
            case vtBoolTrue:
                buffer += "true";
                break;
            case vtBoolFalse:
                buffer += "false";
                break;
            case vtNumberDouble:
                sprintf(ch64, "%.15g", *reinterpret_cast<double*>(v));
                buffer += ch64;
                break;
            case vtNumberInteger:
                sprintf(ch64, "%lld", *reinterpret_cast<long long*>(v));
                buffer += ch64;
                break;
            case vtString:
                dump_string(buffer);
                break;
            case vtArray:
                dump_array(buffer, level, indent);
                break;
            case vtObject:
                dump_object(buffer, level, indent);
                break;
            default:
                break;
            }
        }

    private:
        void dump_string(std::string& buffer) const {
            buffer += '"';
            for (char c : *reinterpret_cast<string*>(v)) {
                switch (c) {
                default:           buffer += c;          break;
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

        void dump_array(std::string& buffer, int level, int indent) const {
            bool hasitem = false, isfirst = true;
            buffer += '[';
            ++level;
            for (value* item : *reinterpret_cast<array*>(v)) {
                if (item) {
                    hasitem = true;
                    if (isfirst) isfirst = false; else buffer += ',';
                    if (indent) {
                        buffer += '\n';
                        buffer.append(level * indent, ' ');
                    }
                    item->dump(buffer, level, indent);
                }
            }
            --level;
            if (indent && hasitem) {
                buffer += '\n';
                buffer.append(level * indent, ' ');
            }
            buffer += ']';
        }

        void dump_object(std::string& buffer, int level, int indent) const {
            bool hasitem = false, isfirst = true;
            buffer += '{';
            ++level;
            for (object::value_type& pair : *reinterpret_cast<object*>(v)) {
                if (pair.second) {
                    hasitem = true;
                    if (isfirst) isfirst = false; else buffer += ',';
                    if (indent) {
                        buffer += '\n';
                        buffer.append(level * indent, ' ');
                    }
                    pair.first->dump_string(buffer);
                    buffer += indent ? ": " : ":";
                    pair.second->dump(buffer, level, indent);
                }
            }
            --level;
            if (indent && hasitem) {
                buffer += '\n';
                buffer.append(level * indent, ' ');
            }
            buffer += '}';
        }
    };

    typedef std::string string;
    typedef std::vector<value*> array;
    typedef std::unordered_map<value*, value*> object;

public:
    json(void)
        : value_(nullptr)
    {}

    json(const std::string& jsonstr)
        : json()
    {
        parse(jsonstr);
    }

   ~json(void) {
        value_->~value();
    }

public:
    bool parse(const std::string& jsonstr) {
        strstream ss(jsonstr);
        if (value_) {
            value_->~value();
            mempool_.reset();
        }
        return (value_ =  parse(ss));
    }

    std::string dump(int indent = 0) const {
        if (value_) {
            std::string buffer;
            value_->dump(buffer, 0, indent < 0 ? 0 : indent);
            return std::move(buffer);
        }
        return "";
    }

private:
    value* parse(strstream& ss) {
        typedef std::numeric_limits<double> limits;

        switch (ss.get_nospace()) {
        case 'n':
            if (ss.expect("ull"))
                return make_value(vtNull);
            else if (ss.expect("an"))
                return make_value(vtNumberDouble, limits::quiet_NaN());
            return nullptr;

        case 'i':
            if (ss.expect("nf"))
                return make_value(vtNumberDouble, limits::infinity());
            return nullptr;

        case 't':
            if (ss.expect("rue"))
                return make_value(vtBoolTrue);
            return nullptr;

        case 'f':
            if (ss.expect("alse"))
                return make_value(vtBoolFalse);
            return nullptr;

        case '-':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            ss.unget();
            return parse_number(ss);

        case '"':
            return parse_string(ss);

        case '[':
            return parse_array(ss);

        case '{':
            return parse_object(ss);

        default:
            return nullptr;
        }
    }

    value* parse_number(strstream& ss) {
        bool isdouble = false;
        std::string::const_iterator milestone = ss.milestone();

        ss.expect('-');
        if (!isdigit(ss.peek()))
            return nullptr;
        else if (!ss.expect('0'))
            do { ss.take(); } while (isdigit(ss.peek()));

        if (ss.expect('.')) {
            isdouble = true;
            if (!isdigit(ss.peek()))
                return nullptr;
            else
                do { ss.take(); } while (isdigit(ss.peek()));
        }

        if (ss.peek() == 'e' || ss.peek() == 'E') {
            ss.take();
            isdouble = true;
            if (ss.peek() == '-' || ss.peek() == '+')
                ss.take();
            if (!isdigit(ss.peek()))
                return nullptr;
            else
                do { ss.take(); } while (isdigit(ss.peek()));
        }

        std::string substr = ss.substr(milestone);
        if (isdouble)
            return make_value(vtNumberDouble, atof(substr.c_str()));
        else
            return make_value(vtNumberInteger, atoll(substr.c_str()));
    }

    value* parse_string(strstream& ss) {
        string str;

        while (true) {
            switch (char ch = ss.get()) {
            default: str += ch; break;
            case 0x5c:
                switch (char ch2 = ss.get()) {
                default:   str += 0x5c; str += ch2; break;
                case 0x5c: str += 0x5c; break;
                case 0x22: str += 0x22; break;
                case 0x2f: str += 0x2f; break;
                case 0x62: str += 0x08; break;
                case 0x66: str += 0x0c; break;
                case 0x6e: str += 0x0a; break;
                case 0x72: str += 0x0d; break;
                case 0x74: str += 0x09; break;
                case 0x75:
                    {
                        char ch4[4];
                        for (int i = 0; i < 4; ++i) {
                            ch4[i] = ss.get();
                            if (!isxdigit(ch4[i]))
                                return nullptr;
                        }
                        str += (unhex(ch4[0]) << 4) | unhex(ch4[1]);
                        str += (unhex(ch4[2]) << 4) | unhex(ch4[3]);
                    }
                    break;
                }
                break;
            case '"':
                return make_value(vtString, std::move(str));
            case  0 :
                return nullptr;
            }
        }
    }

    value* parse_array(strstream& ss){
        array array;

        if (ss.peek_nospace() == ']') {
            ss.take();
            return make_value(vtArray, std::move(array));
        }

        while (true) {
            value* value = parse(ss);
            if (value)
                array.emplace_back(value);
            else
                goto return_null;

            switch (ss.get_nospace()) {
            case ']': return make_value(vtArray, std::move(array));
            case ',': break;
            default : goto return_null;
            }
        }

    return_null:
        for (value* value : array)
            value->~value();
        return nullptr;
    }

    value* parse_object(strstream& ss) {
        object object;
        value* key;

        while (true) {
            switch (ss.get_nospace()) {
            case '}':
                return make_value(vtObject, std::move(object));
            case '"':
                key = parse_string(ss);
                if (!key || ss.get_nospace() != ':')
            default:
                    goto return_null;
            }

            value* value = parse(ss);
            if (value)
                object[key] = value;
            else
                goto return_null;

            if (ss.peek_nospace() == ',')
                ss.take();
        }

    return_null:
        if (key)
            key->~value();
        for (auto& pair : object) {
            pair.first->~value();
            pair.second->~value();
        }
        return nullptr;
    }

private:
    value* make_value(value_type t) {
        return mempool_.alloc<value>(t);
    }

    template <typename V>
    value* make_value(value_type t, V&& v) {
        return mempool_.alloc<value>(t, mempool_.alloc<V>(std::forward<V>(v)));
    }

    char unhex(char hex) const {
        switch (hex) {
        default :
        case '0':           return  0;
        case '1':           return  1;
        case '2':           return  2;
        case '3':           return  3;
        case '4':           return  4;
        case '5':           return  5;
        case '6':           return  6;
        case '7':           return  7;
        case '8':           return  8;
        case '9':           return  9;
        case 'a': case 'A': return 10;
        case 'b': case 'B': return 11;
        case 'c': case 'C': return 12;
        case 'd': case 'D': return 13;
        case 'e': case 'E': return 14;
        case 'f': case 'F': return 15;
        }
    }

private:
    value* value_;
    mempool mempool_;
};

#endif // json_hpp_20180720
