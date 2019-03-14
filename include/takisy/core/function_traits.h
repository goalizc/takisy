#ifndef function_traits_h_20170509
#define function_traits_h_20170509

#include <tuple>
#include <type_traits>

template <typename T>
struct function_traits;

template <typename R, typename... Args>
struct function_traits<R(Args...)> {
private:
    template <typename T>
    using remove_cvref_t = typename std::remove_cv<
                           typename std::remove_reference<T>::type>::type;

public:
    static constexpr unsigned int arity = sizeof...(Args);
    typedef R result;
    typedef std::tuple<Args...> args;
    typedef std::tuple<remove_cvref_t<Args>...> pure_args;

    template <unsigned int I>
    struct arg {
        static_assert(I < arity, "no such index.");
        typedef typename std::tuple_element<I, args>::type type;
    };

    template <unsigned int I>
    struct pure_arg {
        static_assert(I < arity, "no such index.");
        typedef typename std::tuple_element<I, pure_args>::type type;
    };
};

template <typename R>
struct function_traits<R(void)> {
    static constexpr unsigned int arity = 0;
    typedef R result;
};

template <typename R, typename... Args>
struct function_traits<R(*)(Args...)>
    : public function_traits<R(Args...)>
{};

template <typename R>
struct function_traits<R(*)(void)>
    : public function_traits<R(void)>
{};

template <typename T, typename R, typename... Args>
struct function_traits<R(T::*)(Args...)>
    : public function_traits<R(Args...)>
{};

template <typename T, typename R>
struct function_traits<R(T::*)(void)>
    : public function_traits<R(void)>
{};

template <typename T, typename R, typename... Args>
struct function_traits<R(T::*)(Args...) const>
    : public function_traits<R(Args...)>
{};

template <typename T, typename R>
struct function_traits<R(T::*)(void) const>
    : public function_traits<R(void)>
{};

template <typename T>
struct function_traits
    : public function_traits<decltype(&T::operator())>
{};

template <typename T>
struct function_traits<T&>
    : public function_traits<decltype(&T::operator())>
{};

#endif // function_traits_h_20170509
