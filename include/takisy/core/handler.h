#ifndef handler_h_20150715
#define handler_h_20150715

#include <set>
#include <tuple>
#include <memory>

class handler
{
protected:
    template <unsigned int N>
    struct Apply
    {
        template <typename F, typename T, typename... A>
        static inline void apply(F& f, T& t, A&... a)
        {
            Apply<N - 1>::apply(f, t, std::get<N - 1>(t), a...);
        }
    };

public:
    typedef std::shared_ptr<handler> sptr;

    struct handlers : std::set<sptr>
    {
        handlers(void) : std::set<sptr>() {}
        handlers(const handlers&) {}
    };

public:
    virtual ~handler(void) {}
    virtual void handle(void* tuple) = 0;

public:
    template <typename... Args, typename Lambda>
    static sptr make(Lambda lambda)
    {
        class lambda_handler : public handler
        {
        public:
            inline lambda_handler(Lambda lambda)
                : lambda_(lambda)
            {}

        public:
            inline void handle(void* tuple) override
            {
                Apply<sizeof...(Args)>::apply
                    (lambda_, *reinterpret_cast<std::tuple<Args...>*>(tuple));
            }

        private:
            Lambda lambda_;
        };

        return sptr(new lambda_handler(lambda));
    }
};

template <>
struct handler::Apply<0>
{
    template <typename F, typename T, typename... A>
    static inline void apply(F& f, T&, A&... a)
    {
        f(a...);
    }
};

#define DECLARE_HANDLER(h, ...)                                         \
    template <typename Lambda>                                          \
    inline handler::sptr h(Lambda lambda)                               \
    {                                                                   \
        return h(handler::make<decltype(this), ##__VA_ARGS__>(lambda)); \
    }                                                                   \
                                                                        \
    inline handler::sptr h(const handler::sptr& handler)                \
    {                                                                   \
        return h##_.insert(handler), handler;                           \
    }                                                                   \
                                                                        \
    inline void h##Remove(const handler::sptr& handler)                 \
    {                                                                   \
        h##_.erase(handler);                                            \
    }                                                                   \
                                                                        \
    inline void h##Reset(void)                                          \
    {                                                                   \
        h##_.clear();                                                   \
    }                                                                   \
                                                                        \
    template <typename... Args>                                         \
    inline void h##Handle(Args&&... args)                               \
    {                                                                   \
        std::tuple<decltype(this), ##__VA_ARGS__> tuple(this, args...); \
                                                                        \
        for (const handler::sptr& handler : h##_)                       \
            handler->handle(&tuple);                                    \
    }                                                                   \
                                                                        \
    handler::handlers h##_;

#endif // handler_h_20150715
