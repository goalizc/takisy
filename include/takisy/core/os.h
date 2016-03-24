#ifndef os_h_20160316
#define os_h_20160316

#include <string>
#include <sys/types.h>
#include <takisy/core/enhancive_basic_string.h>

class os
{
public:
    class path;
};

class os::path
{
public:
    typedef struct { std::string first, second; } pair;

public:
    static std::string basename  (const std::string& path);
    static std::string dirname   (const std::string& path);
    static bool        exists    (const std::string& path);
    static time_t      getatime  (const std::string& path);
    static time_t      getctime  (const std::string& path);
    static time_t      getmtime  (const std::string& path);
    static off_t       getsize   (const std::string& path);
    static bool        isabs     (const std::string& path);
    static bool        isdir     (const std::string& path);
    static bool        isfile    (const std::string& path);
    static std::string prefix    (const std::string& path);
    static pair        split     (const std::string& path);
    static pair        splitdrive(const std::string& path);
    static pair        splitext  (const std::string& path);
    static std::string suffix    (const std::string& path);
};

#endif // os_h_20160316
