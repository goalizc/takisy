#ifndef json_h_20140926
#define json_h_20140926

#include <vector>
#include <string>
#include <takisy/core/stream.h>

class json
{
    class object;
    class implement;

public:
    enum ObjectType
    {
        otUndefined = 0,
        otNull      = 1,
        otBoolean   = 2,
        otNumber    = 3,
        otString    = 4,
        otArray     = 5,
        otDict      = 6,
    };

public:
    json(void);
    json(const char* filepath_or_content);
    json(stream& stream);
    json(const json& json);
    json(object*&);
   ~json(void);

public:
    bool load(const char* content);
    bool load(stream& stream);
    bool load_file(const char* filepath);

    std::string dump(void) const;
    std::string dump(int indent) const;
    bool dump(stream& stream) const;
    bool dump(stream& stream, int indent) const;
    bool dump_file(const char* filepath) const;
    bool dump_file(const char* filepath, int indent) const;

public: // set value
    json& operator=(std::nullptr_t);
    json& operator=(bool boolean);
    json& operator=(int number);
    json& operator=(long long number);
    json& operator=(double number);
    json& operator=(const char* string);
    json& operator=(ObjectType object_type);
    json& operator=(const json& json);

public: // only for array
    void append(std::nullptr_t);
    void append(bool boolean);
    void append(int number);
    void append(long long number);
    void append(double number);
    void append(const char* string);
    void append(ObjectType object_type);
    unsigned int count(void) const;
    json operator[](int index) const;

public: // only for dict
    std::vector<std::string> keys(void) const;
    json operator[](const char* key) const;

public:
    ObjectType  type     (void) const;

    bool        as_bool  (void) const;
    double      as_number(void) const;
    const char* as_string(void) const;

public:
    const char* error_info(void) const;

private:
    class implement* impl_;
};

#endif // json_h_20140926
