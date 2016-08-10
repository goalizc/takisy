#ifndef rfc1951_h_20140326
#define rfc1951_h_20140326

#include <takisy/core/bit_buffer.h>
#include <takisy/core/stretchy_buffer.h>

namespace rfc1951
{

typedef stretchy_buffer<unsigned char> buffer_t;

enum ExceptionCode
{
    ecUnkown,

    ecOutOfBuffer = bit_buffer_exception_code::kOutOfBuffer,
    ecInvalidCode,
    ecInvalidBlockType,
    ecInvalidCodeLength,
    ecInvalidStoreLEN,
};

void     deflate(const unsigned char* buffer, unsigned int length,
                 bit_buffer_o& output, unsigned int level);
buffer_t inflate(bit_buffer_i& input);

} // end of namespace rfc1951

#endif // rfc1951_h_20140326
