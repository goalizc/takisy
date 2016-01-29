#ifndef file_system_h_20150619
#define file_system_h_20150619

#include <string>
#include <utility>

class file_system
{
public:
    typedef std::pair<std::string, std::string> pair;

public:
    static pair split(std::string path);
};

#endif // file_system_h_20150619
