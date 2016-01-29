#include <Windows.h>
#include <takisy/oldgui/basic/exception.h>

namespace takisy {
namespace oldgui {

Exception::Exception(const std::string& error_info)
    : error_info_(error_info)
{}

Exception::Exception(const Exception& exception)
    : error_info_(exception.error_info_)
{}

Exception& Exception::operator=(const Exception& exception)
{
    if (this != &exception)
        error_info_ = exception.error_info_;

    return *this;
}

const char* Exception::what(void) const noexcept
{
    return error_info_.c_str();
}

Exception Exception::system_exception(unsigned int error_code)
{
    char error_info[256] = {0};

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                  nullptr, error_code,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  reinterpret_cast<LPTSTR>(&error_info),
                  sizeof(error_info), nullptr);

    return Exception(error_info);
}

}}
