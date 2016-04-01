#include <cstdlib>
#include <takisy/core/os.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/parser/ini.h>

ini::ini(void)
{}

ini::ini(const char* filepath_or_content)
{
    if (os::path::isfile(filepath_or_content))
        load_file(filepath_or_content);
    else
        load(filepath_or_content);
}

ini::ini(const stream& stream)
{
    load(stream);
}

ini::ini(const ini& ini)
    : sections_(ini.sections_)
{}

ini::~ini(void)
{}

ini& ini::operator=(const ini& ini)
{
    if (this != &ini)
        sections_ = ini.sections_;

    return *this;
}

bool ini::load(const char* content)
{
    return load(buffer_stream(content, strlen(content)));
}

bool ini::load(const stream& stream)
{
    std::string section_name = "default";

    while (stream.readable())
    {
        std::string line;
        char ch;

        while (stream.read(&ch, sizeof(ch)) == sizeof(ch) && ch != '\n')
            line += ch;

        if (stralgo::trim(line).empty())
            continue;

        switch (line.front())
        {
        case '[':
            {
                std::string::size_type pos = line.find(']', 1);
                if (pos == std::string::npos)
                    return false;
                else
                    section_name = line.substr(1, pos - 1);
            }
        case ';':
            break;
        default:
            {
                stralgo::strings parts    = stralgo::split(line, ';', 1);
                stralgo::strings keyvalue = stralgo::split(parts[0], '=', 1);

                if (keyvalue.size() != 2)
                    return false;

                sections_[section_name]
                    [stralgo::trim(keyvalue[0])] = stralgo::trim(keyvalue[1]);
            }
            break;
        }
    }

    return true;
}

bool ini::load_file(const char* filepath)
{
    return load(file_stream(filepath, "r"));
}

std::string ini::dump(void) const
{
    std::string content;

    for (const decltype(sections_)::value_type& section : sections_)
    {
        content += stralgo::format("[%s]\n", section.first.c_str());

        for (const section_type::value_type& pair : section.second)
            content += stralgo::format("    %s=%s\n", pair.first.c_str(),
                                                      pair.second.as_string());

        content += "\n";
    }

    return content;
}

bool ini::dump(stream& stream) const
{
    std::string content = dump();

    return stream.write(content.data(), content.size()) == content.size();
}

bool ini::dump_file(const char* filepath) const
{
    file_stream fstream(filepath, "w");

    return dump(fstream);
}

ini::section_type& ini::operator[](const std::string& section_name)
{
    return sections_[section_name];
}

ini::value::value(void)
{}

ini::value::value(const std::string& value)
    : value_(value)
{}

ini::value::value(const value& value)
    : value_(value.value_)
{}

ini::value::~value(void)
{}

ini::value& ini::value::operator=(const value& value)
{
    if (this != &value)
        value_ = value.value_;

    return *this;
}

void ini::value::operator=(bool boolean)
{
    value_ = boolean ? "true" : "false";
}

void ini::value::operator=(int number)
{
    operator=(static_cast<double>(number));
}

void ini::value::operator=(long long number)
{
    operator=(static_cast<double>(number));
}

void ini::value::operator=(double number)
{
    value_ = stralgo::format("%g", number);
}

void ini::value::operator=(const char* string)
{
    value_ = string;
}

bool ini::value::as_bool(void) const
{
    return stralgo::lowerc(value_) == "true";
}

double ini::value::as_number(void) const
{
    return strtod(value_.c_str(), nullptr);
}

const char* ini::value::as_string(void) const
{
    return value_.c_str();
}

ini::value::operator const char*(void) const
{
    return as_string();
}
