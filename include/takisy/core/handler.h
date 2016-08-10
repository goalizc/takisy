#ifndef handler_h_20150715
#define handler_h_20150715

#include <vector>
#include <memory>

class handler
{
public:
    virtual ~handler(void) {}
    virtual void handle(void) = 0;
};

typedef std::shared_ptr<handler> handler_sptr;

template <typename This, typename Lambda>
inline handler_sptr make_lambda_handler(This _this, Lambda lambda)
{
    class lambda_handler : public handler
    {
    public:
        inline lambda_handler(This _this, Lambda lambda)
            : this_(_this), lambda_(lambda)
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

    return handler_sptr(new lambda_handler(_this, lambda));
}

#define DECLARE_HANDLER(h)                                          \
public:                                                             \
    template <typename Lambda>                                      \
    inline void h(Lambda lambda)                                    \
    {                                                               \
        h##_handlers_.push_back(make_lambda_handler(this, lambda)); \
    }                                                               \
                                                                    \
    inline void h(void)                                             \
    {                                                               \
        for (handler_sptr& handler : h##_handlers_)                 \
            handler->handle();                                      \
    }                                                               \
                                                                    \
private:                                                            \
    std::vector<handler_sptr> h##_handlers_;

#endif // handler_h_20150715
