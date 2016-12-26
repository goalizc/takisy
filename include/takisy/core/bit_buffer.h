#ifndef bit_buffer_h_20140425
#define bit_buffer_h_20140425

#include <takisy/core/stretchy_buffer.h>

class bit_buffer
{
public:
    enum ExceptionCode
    {
        ecOutOfBuffer = 1,
    };

public:
    class input
    {
        class implement;

    public:
        input(const unsigned char* buffer, unsigned int size);
        input(const input& bbi);
       ~input(void);

        input& operator=(const input& bbi);

    public:
        void align(void);
        unsigned int read(register unsigned int n);
        unsigned int peek(register unsigned int n);

    private:
        class implement* impl_;
    };

public:
    class output
    {
        class implement;

    public:
        output(void);
        output(const output& bbo);
       ~output(void);

        output& operator=(const output& bbo);

    public:
        void align(void);
        void write(unsigned int bits, unsigned int count);

    public:
        stretchy_buffer<unsigned char> buffer(void) const;

    private:
        class implement* impl_;
    };
};
#endif // bit_buffer_h_20140425
