#ifndef reflect_h_20170111
#define reflect_h_20170111

#include <tuple>
#include <type_traits>
#include <takisy/preprocessor/arithmetic/dec.hpp>
#include <takisy/preprocessor/seq/enum.hpp>
#include <takisy/preprocessor/seq/for_each.hpp>
#include <takisy/preprocessor/seq/transform.hpp>
#include <takisy/preprocessor/seq/variadic_seq_to_seq.hpp>
#include <takisy/preprocessor/stringize.hpp>
#include <takisy/preprocessor/tuple/enum.hpp>
#include <takisy/preprocessor/tuple/pop_back.hpp>
#include <takisy/preprocessor/tuple/push_front.hpp>
#include <takisy/preprocessor/tuple/size.hpp>
#include <takisy/preprocessor/variadic/size.hpp>
#include <takisy/preprocessor/variadic/to_seq.hpp>

#define REFLECT_DEFINE_STRUCT(NAME, BODY, CODE...) \
    struct NAME {                                  \
        PP_SEQ_FOR_EACH(REFLECT_MEMBER_OP, , BODY) \
        CODE                                       \
    };                                             \
    REFLECT_ADAPT_STRUCT(                          \
        NAME,                                      \
        REFLECT_BODY_TO_MEMBERS(BODY)              \
    )

#define REFLECT_ADAPT_STRUCT(NAME, MEMBERS...)                              \
    template <>                                                             \
    struct reflect::Info<NAME> {                                            \
        static constexpr bool reflexible = true;                            \
        static constexpr unsigned size = PP_VARIADIC_SIZE(MEMBERS);         \
        static constexpr const char* name = #NAME;                          \
        static constexpr const char* members[] = { REFLECT_STRS(MEMBERS) }; \
        template <unsigned I>                                               \
        struct member {                                                     \
            static constexpr unsigned index = I;                            \
            static constexpr const char* name = members[I];                 \
        };                                                                  \
    };                                                                      \
    template <unsigned I, typename R>                                       \
    struct reflect::Member<I, R, NAME> : public Info<NAME>::member<I> {     \
        R& holder;                                                          \
        typedef decltype(ref<I>(REFLECT_CALL(holder, MEMBERS))) value_type; \
        value_type& value;                                                  \
    public:                                                                 \
        inline Member(R& holder)                                            \
            : holder(holder), value(ref<I>(REFLECT_CALL(holder, MEMBERS)))  \
        {}                                                                  \
    };

#define REFLECT_BODY(body) \
    PP_SEQ_TRANSFORM(REFLECT_BODY_OP, , PP_VARIADIC_SEQ_TO_SEQ(body))
#define REFLECT_BODY_OP(r, data, tuple) \
    PP_TUPLE_PUSH_FRONT(PP_TUPLE_POP_BACK(tuple), \
                        PP_TUPLE_ELEM(PP_DEC(PP_TUPLE_SIZE(tuple)), tuple))

#define REFLECT_MEMBER_OP(r, data, elem) REFLECT_MEMBER_EXPAND elem
#define REFLECT_MEMBER_EXPAND(name, type...) type name;
#define REFLECT_BODY_TO_MEMBERS(body) \
    REFLECT_ENUM_TRANSFROM(REFLECT_BODY_TO_MEMBERS_OP, , body)
#define REFLECT_BODY_TO_MEMBERS_OP(r, data, elem) PP_TUPLE_ELEM(0, elem)
#define REFLECT_STRS(args...) \
    REFLECT_ENUM_TRANSFROM(REFLECT_STRS_OP, , PP_VARIADIC_TO_SEQ(args))
#define REFLECT_STRS_OP(r, data, elem) PP_STRINGIZE(elem)
#define REFLECT_CALL(object, members...) \
    REFLECT_ENUM_TRANSFROM(REFLECT_CALL_OP, object, PP_VARIADIC_TO_SEQ(members))
#define REFLECT_CALL_OP(r, object, member) object.member
#define REFLECT_ENUM_TRANSFROM(op, data, seq) \
    PP_SEQ_ENUM(PP_SEQ_TRANSFORM(op, data, seq))

struct reflect {
    /* ===============================================================
     * remove const, volatile, reference attributes
     * ============================================================ */
    template <typename T>
    struct remove_cvref {
        typedef typename std::remove_cv
               <typename std::remove_reference<T>::type>::type type;
    };

    /* ===============================================================
     * reflect type information
     * ============================================================ */
    template <typename R>
    struct Info {
        static constexpr bool reflexible = false;
    };

    template <typename R>
    using info = Info<typename remove_cvref<R>::type>;

    /* ===============================================================
     * reflect object's member
     * ============================================================ */
    template <unsigned I, typename R, typename R2>
    struct Member;

    template <unsigned I, typename R>
    using member = Member<I, R, typename remove_cvref<R>::type>;

    /* ===============================================================
     * for_each helper
     * ============================================================ */
    template <unsigned I>
    struct ForEach {
        template <typename R, typename OP>
        static inline void apply(R& r, OP&& op) {
            ForEach<I - 1>::apply(r, op), op(member<I, R>(r));
        }
    };

public:
    /* ===============================================================
     * Ird's reference of args
     * ============================================================ */
    template <unsigned I, typename... T>
    static inline typename std::tuple_element<I, std::tuple<T&...>>::type
            ref(T&... args) {
        return std::get<I>(std::tuple<T&...>(args...));
    }

public:
    /* ===============================================================
     * reflect object's member value
     * ============================================================ */
    template <unsigned I, typename R>
    static inline typename member<I, R>::value_type& value(R& r) {
        return member<I, R>(r).value;
    }

    template <unsigned I, typename R>
    static inline typename member<I, R>::value_type& value(R&& r) {
        return member<I, R>(r).value;
    }

    /* ===============================================================
     * execute op(member) on each reflect object's members
     * ============================================================ */
    template <typename R, typename OP>
    static inline OP& for_each(R& r, OP& op) {
        return ForEach<info<R>::size - 1>::apply(r, op), op;
    }

    template <typename R, typename OP>
    static inline OP&& for_each(R& r, OP&& op) {
        return ForEach<info<R>::size - 1>::apply(r, op), std::move(op);
    }

    template <typename R, typename OP>
    static inline OP& for_each(R&& r, OP& op) {
        return ForEach<info<R>::size - 1>::apply(r, op), op;
    }

    template <typename R, typename OP>
    static inline OP&& for_each(R&& r, OP&& op) {
        return ForEach<info<R>::size - 1>::apply(r, op), std::move(op);
    }
};

template <>
struct reflect::ForEach<0> {
    template <typename R, typename OP>
    static inline void apply(R& r, OP&& op) {
        op(member<0, R>(r));
    }
};

#endif // reflect_h_20170111
