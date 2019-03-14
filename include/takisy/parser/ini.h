#ifndef ini_h_20170126
#define ini_h_20170126

#include <string>
#include <map>
#include <takisy/algorithm/stralgo.h>
#include <takisy/core/stream.h>

class ini
{
    class implement;

public:
    typedef std::map<std::string, stralgo::string> section_type;

public:
    ini(void);
    ini(const std::string& buffer);
    ini(const stream& istream);
    ini(const ini& ini);
    ini(ini&& ini);
   ~ini(void);

    ini& operator=(const ini& ini);
    ini& operator=(ini&& ini);

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
    section_type& operator[](const std::string& section);

private:
    class implement* impl_;
};

#endif // ini_h_20170126
