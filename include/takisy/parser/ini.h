#ifndef ini_h_20141014
#define ini_h_20141014

#include <map>
#include <string>

class ini
{
public:
    class value;

public:
    typedef std::map<std::string, value> section_type;

public:
    ini(void);
    ini(const char* file_path);
    ini(const ini& ini);
   ~ini(void);

    ini& operator=(const ini& ini);

public:
    bool load(const char* file_path);
    bool save(const char* file_path) const;

public:
    section_type& operator[](const std::string& section_name);

private:
    std::map<std::string, section_type> sections_;
};

class ini::value
{
public:
    value(void);
    value(const std::string& value);
    value(const value& value);
   ~value(void);

    value& operator=(const value& value);

public:
    void operator=(bool boolean);
    void operator=(int number);
    void operator=(long long number);
    void operator=(double number);
    void operator=(const char* string);

public:
    bool        as_bool(void) const;
    double      as_number(void) const;
    const char* as_string(void) const;

public:
    operator const char*(void) const;

private:
    std::string value_;
};

#endif // ini_h_20141014
