#include <takisy/core/file_system.h>

file_system::pair file_system::split(std::string path)
{
    for (char& c : path)
        if (c == '\\') c = '/';

    std::string::size_type pos = path.find_last_of('/');
    pair pair;

    if (pos == std::string::npos)
    {
        pair.first  = "";
        pair.second = path;
    }
    else
    {
        pair.first  = path.substr(0, pos);
        pair.second = path.substr(++pos);
    }

    return pair;
}
