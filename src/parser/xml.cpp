/*#include <map>
#include <vector>
#include <stdexcept>
#include <takisy/core/os.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/parser/xml.h>

class xml_exception : public std::exception
{
public:
    xml_exception(const char* what)
        : what_(what)
    {}

    template <typename... Args>
    xml_exception(const char* format, const Args&... args)
    {
        what_ = stralgo::format(format, args...);
    }

    ~xml_exception(void)
    {}

public:
    const char* what(void) const noexcept override
    {
        return what_.c_str();
    }

private:
    std::string what_;
};

#define throw(format, args...) \
    ({ throw xml_exception(format, ##args); })

struct xml::object
{
    enum ObjectType
    {
        otXml, otData
    } type;

    union Union
    {
        class xml* xml;
        std::string data;

    public:
        Union(void)
            : xml(nullptr)
        {}
    } u;
};

class xml::implement
{
    friend class xml;

private:
    std::vector<xml*> elements_;
    std::map<std::string, std::string> attributes_;
};

xml::xml(void)
    : impl_(new implement)
{}

xml::xml(const char* filepath_or_content)
    : xml()
{
    if (os::path::isfile(filepath_or_content))
        load_file(filepath_or_content);
    else
        load(filepath_or_content);
}

xml::xml(const stream& stream)
    : xml()
{
    load(stream);
}

xml::xml(const xml& xml)
    : xml()
{
    operator=(xml);
}

xml::xml(object*&)
    : xml()
{

}

xml::~xml(void)
{
    delete impl_;
}

bool xml::load(const char* content)
{

}

bool xml::load(const stream& stream)
{

}

bool xml::load_file(const char* filepath)
{

}

std::string xml::dump(void) const
{

}

std::string xml::dump(int indent) const
{

}

bool xml::dump(stream& stream) const
{

}

bool xml::dump(stream& stream, int indent) const
{

}

bool xml::dump_file(const char* filepath) const
{

}

bool xml::dump_file(const char* filepath, int indent) const
{

}

*/
