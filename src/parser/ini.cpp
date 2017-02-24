#include <algorithm>
#include <takisy/algorithm/stralgo.h>
#include <takisy/parser/ini.h>

class ini::implement
{
public:
    std::map<std::string, section_type> sections;
};

ini::ini(void)
    : impl_(new implement)
{}

ini::ini(const std::string& buffer)
    : ini()
{
    load(buffer);
}

ini::ini(const stream& istream)
    : ini()
{
    load(istream);
}

ini::ini(const ini& _ini)
    : ini()
{
    operator=(_ini);
}

ini::ini(ini&& _ini)
    : ini()
{
    operator=(std::move(_ini));
}

ini::~ini(void)
{
    delete impl_;
}

ini& ini::operator=(const ini& ini)
{
    if (this != &ini)
        impl_->sections = ini.impl_->sections;

    return *this;
}

ini& ini::operator=(ini&& ini)
{
    std::swap(impl_, ini.impl_);

    return *this;
}

bool ini::load(const std::string& buffer)
{
    return load(buffer_stream(buffer.data(), buffer.size()));
}

bool ini::load(const stream& istream)
{
    std::string section = "global";

    while (istream.readable())
    {
        std::string line = stralgo::trim(istream.read_line());
        if (line.empty())
            continue;

        switch (line.front())
        {
        case ';':
            break;
        case '[':
            if (line.back() != ']')
                return false;
            else
                section = stralgo::trim(line.substr(1, line.size() - 2));
            break;
        default:
            {
                stralgo::strings keyvalue = stralgo::split(line, "=", 1);
                if (keyvalue.size() != 2)
                    return false;
                else
                    impl_->sections[section][stralgo::trim(keyvalue[0])]
                        = stralgo::trim(keyvalue[1]);
            }
            break;
        }
    }

    return true;
}

std::string ini::dump(void) const
{
    return dump(0);
}

std::string ini::dump(unsigned int indent) const
{
    std::string buffer;

    for (const auto& section : impl_->sections)
    {
        buffer += '[' + section.first + "]\n";
        for (const auto& keyvalue : section.second)
            buffer += std::string(indent, 0x20)
                    + keyvalue.first + " = " + keyvalue.second  + '\n';
        buffer += '\n';
    }

    return buffer;
}

bool ini::dump(stream& ostream) const
{
    return dump(ostream, 0);
}

bool ini::dump(stream&& ostream) const
{
    return dump(ostream, 0);
}

bool ini::dump(stream& ostream, unsigned int indent) const
{
    std::string buffer = dump(indent);

    return ostream.write(buffer.data(), buffer.size()) == buffer.size();
}

bool ini::dump(stream&& ostream, unsigned int indent) const
{
    return dump(ostream, indent);
}

ini::section_type& ini::operator[](const std::string& section)
{
    return impl_->sections[section];
}
