#ifndef rfc1951_h_20140326
#define rfc1951_h_20140326

#include <takisy/core/bit_buffer.h>
#include <takisy/core/stretchy_buffer.h>

namespace rfc1951
{

typedef stretchy_buffer<unsigned char> buffer_type;

enum ExceptionCode
{
    ecUnkown,

    ecOutOfBuffer = bit_buffer::ecOutOfBuffer,
    ecInvalidCode,
    ecInvalidBlockType,
    ecInvalidCodeLength,
    ecInvalidStoreLEN,
};

void deflate(const unsigned char* buffer, unsigned int length,
             bit_buffer::output& output, unsigned int level);
buffer_type inflate(bit_buffer::input& input);

} // end of namespace rfc1951

#endif // rfc1951_h_20140326
