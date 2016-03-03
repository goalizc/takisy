#ifndef xml_h_20160224
#define xml_h_20160224

#include <string>
#include <takisy/core/stream.h>

class xml
{
    class object;
    class implement;

public:
    xml(void);
    xml(const char* filepath_or_content);
    xml(const stream& stream);
    xml(const xml& xml);
    xml(object*&);
   ~xml(void);

public:
    bool load(const char* content);
    bool load(const stream& stream);
    bool load_file(const char* filepath);

    std::string dump(void) const;
    std::string dump(int indent) const;
    bool dump(stream& stream) const;
    bool dump(stream& stream, int indent) const;
    bool dump_file(const char* filepath) const;
    bool dump_file(const char* filepath, int indent) const;

public:
    ;

public:
    bool        as_bool  (void) const;
    bool        as_number(void) const;
    const char* as_stream(void) const;

private:
    class implement* impl_;
};

#endif // xml_h_20160224
