#ifndef exception_h_20130715
#define exception_h_20130715

#include <string>

namespace takisy {
namespace oldgui {

class Exception : public std::exception
{
public:
    Exception(const std::string& error_info);
    Exception(const Exception& exception);

    Exception& operator=(const Exception& exception);

public:
    const char* what(void) const noexcept override;

public:
    static Exception system_exception(unsigned int error_code);

private:
    std::string error_info_;
};

}}

#endif //exception_h_20130715
