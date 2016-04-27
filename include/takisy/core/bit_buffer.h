#ifndef bit_buffer_h_20140425
#define bit_buffer_h_20140425

#include <takisy/core/stretchy_buffer.h>

struct bit_buffer_exception_code
{
    static const unsigned int kOutOfBuffer = 1;
};

class bit_buffer_i
{
    class implement;

public:
    bit_buffer_i(const unsigned char* buffer, unsigned int size);
    bit_buffer_i(const bit_buffer_i& bbi);
   ~bit_buffer_i(void);

    bit_buffer_i& operator=(const bit_buffer_i& bbi);

public:
    void align(void);
    unsigned int read(register unsigned int n);
    unsigned int peek(register unsigned int n);

private:
    class implement* impl_;
};

class bit_buffer_o
{
    class implement;

public:
    bit_buffer_o(void);
    bit_buffer_o(const bit_buffer_o& bbo);
   ~bit_buffer_o(void);

    bit_buffer_o& operator=(const bit_buffer_o& bbo);

public:
    void align(void);
    void write(unsigned int bits, unsigned int count);

public:
    stretchy_buffer<unsigned char> buffer(void) const;

private:
    class implement* impl_;
};

#endif // bit_buffer_h_20140425
