#ifndef args_h_20130604
#define args_h_20130604

#include <takisy/core/stretchy_buffer.h>

typedef class args
{
    friend const char* operator%(const char* format, const args& args);

public:
    args(void) {}
    template <typename... Args>
    args(const Args&... nargs) { append(nargs...); }

public:
    template <typename Arg>
    inline void append(const Arg arg)
    {
        static constexpr int kPtrLen = sizeof(const void*);
        static constexpr int kArgLen = sizeof(arg);
        static constexpr int kAlign  = (kPtrLen - kArgLen % kPtrLen) % kPtrLen;

        stack_.append(reinterpret_cast<const char*>(&arg), kArgLen);

        for (int i = 0; i < kAlign; ++i)
            stack_.append(0);
    }

    template <typename Arg, typename... Args>
    inline void append(const Arg first_arg, const Args&... another_args)
    {
        append(first_arg);
        append(another_args...);
    }

private:
    stretchy_buffer<char> stack_;
} _, tuple;

const char* operator%(const char* format, const args& args);

#endif //args_h_20130604
