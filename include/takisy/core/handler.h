#ifndef handler_h_20150715
#define handler_h_20150715

#include <cstddef>
#include <memory>

class handler
{
public:
    virtual ~handler(void) {}
    virtual void handle(void) = 0;
};

typedef std::shared_ptr<handler> handler_sptr;

template <typename Lambda, typename This>
inline handler_sptr make_lambda_handler(This t, Lambda l)
{
    class lambda_handler : public handler
    {
    public:
        inline lambda_handler(This t, Lambda l)
            : this_(t), lambda_(l)
        {}

    public:
        inline void handle() override
        {
            lambda_(*this_);
        }

    private:
        This this_;
        Lambda lambda_;
    };

    return handler_sptr(new lambda_handler(t, l));
}

#define ENABLE_HANDLER(h)                                                      \
public:                                                                        \
    template <typename L>                                                      \
    inline handler_sptr h(L l) { return h(make_lambda_handler(this, l)); }     \
    inline handler_sptr h(std::nullptr_t) { return h(handler_sptr(nullptr)); } \
    inline handler_sptr h(handler_sptr h) { h##_handler_.swap(h); return h; }  \
public:                                                                        \
    inline void         h(void) { if (h##_handler_) h##_handler_->handle(); }  \
private:                                                                       \
    handler_sptr h##_handler_;

#endif // handler_h_20150715
