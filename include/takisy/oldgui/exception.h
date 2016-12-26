#ifndef exception_h_20130715
#define exception_h_20130715

#include <string>
#include <takisy/algorithm/stralgo.h>

namespace takisy {
namespace oldgui {

class Exception : public std::exception
{
public:
    Exception(const std::string& error_info)
        : error_info_(error_info)
    {}

    Exception(const Exception& exception)
        : error_info_(exception.error_info_)
    {}

    Exception& operator=(const Exception& exception)
    {
        if (this != &exception)
            error_info_ = exception.error_info_;

        return *this;
    }

public:
    const char* what(void) const noexcept override
    {
        return error_info_.c_str();
    }

public:
    static Exception system_exception(unsigned int error_code)
    {
        return Exception(stralgo::format("system exception: %d", error_code));
    }

private:
    std::string error_info_;
};

}}

#endif //exception_h_20130715
