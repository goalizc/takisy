#include <unistd.h>
#include <sys/stat.h>
#include <takisy/core/macro.h>
#include <takisy/core/os.h>

#define stat_default(default_return)           \
    ({  struct stat _stat;                     \
        if (::stat(path.c_str(), &_stat) != 0) \
            return default_return;             \
        _stat; })

std::string os::path::basename(const std::string& path)
{
    return split(path).second;
}

std::string os::path::dirname(const std::string& path)
{
    return split(path).first;
}

bool os::path::exists(const std::string& path)
{
    return ::access(path.c_str(), 0) == 0;
}

time_t os::path::getatime(const std::string& path)
{
    return stat_default(0).st_atime;
}

time_t os::path::getctime(const std::string& path)
{
    return stat_default(0).st_ctime;
}

time_t os::path::getmtime(const std::string& path)
{
    return stat_default(0).st_mtime;
}

off_t os::path::getsize(const std::string& path)
{
    return stat_default(0).st_size;
}

bool os::path::isabs(const std::string& path)
{
#ifdef __os_win__
    return path[1] == ':';
#else
    return path[0] == '/';
#endif
}

bool os::path::isdir(const std::string& path)
{
    return stat_default(false).st_mode & S_IFDIR;
}

bool os::path::isfile(const std::string& path)
{
    return stat_default(false).st_mode & S_IFREG;
}

std::string os::path::prefix(const std::string& path)
{
    return splitext(basename(path)).first;
}

os::path::pair os::path::split(const std::string& path)
{
    for (unsigned int i = path.size() - 1; i >= 0; --i)
    {
        if (path[i] == '/' || path[i] == '\\')
            return pair { path.substr(0, i + 1), path.substr(i + 1) };
    }

    return pair { std::string(), path };
}

os::path::pair os::path::splitdrive(const std::string& path)
{
#ifdef __os_win__
    if (path[1] == ':')
        return pair { path.substr(0, 2), path.substr(2) };
    else
        return pair { std::string(), path };
#else
    return pair { std::string(), path };
#endif
}

os::path::pair os::path::splitext(const std::string& path)
{
    for (unsigned int i = path.size() - 1; i >= 0; --i)
    {
        if (path[i] == '.')
            return pair { path.substr(0, i), path.substr(i) };
        else
        if (path[i] == '/' || path[i] == '\\')
            return pair { path, std::string() };
    }

    return pair { path, std::string() };
}

std::string os::path::suffix(const std::string& path)
{
    return splitext(path).second;
}
