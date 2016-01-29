#ifndef handler_h_20150119
#define handler_h_20150119

#include <memory>

struct Handler
{
    virtual ~Handler(void);
    virtual void process(void* user_data) = 0;
};

template <typename Lambda, typename UserData>
class LambdaHandler : public Handler
{
public:
    LambdaHandler(Lambda lambda)
        : lambda_(lambda)
    {}

public:
    inline void process(void* user_data) override
    {
        lambda_(*reinterpret_cast<UserData*>(user_data));
    }

private:
    Lambda lambda_;
};

template <typename Lambda, typename UserData>
inline std::shared_ptr<Handler> get_lambda_handler(Lambda lambda)
{
    return std::shared_ptr<Handler>(
                new LambdaHandler<Lambda, UserData>(lambda));
}

#endif // handler_h_20150119
