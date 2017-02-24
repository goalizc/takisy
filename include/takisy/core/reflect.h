#ifndef reflect_h_20170111
#define reflect_h_20170111

#include <tuple>
#include <type_traits>
#include <takisy/preprocessor/arithmetic/dec.hpp>
#include <takisy/preprocessor/control/if.hpp>
#include <takisy/preprocessor/seq/enum.hpp>
#include <takisy/preprocessor/seq/for_each.hpp>
#include <takisy/preprocessor/seq/transform.hpp>
#include <takisy/preprocessor/seq/variadic_seq_to_seq.hpp>
#include <takisy/preprocessor/tuple/elem.hpp>
#include <takisy/preprocessor/tuple/enum.hpp>
#include <takisy/preprocessor/tuple/pop_back.hpp>
#include <takisy/preprocessor/tuple/pop_front.hpp>
#include <takisy/preprocessor/tuple/push_front.hpp>
#include <takisy/preprocessor/tuple/size.hpp>
#include <takisy/preprocessor/tuple/to_seq.hpp>
#include <takisy/preprocessor/variadic/to_seq.hpp>

#define reflect(refname, body, code...)                                        \
    struct refname {                                                           \
        PP_SEQ_FOR_EACH(reflect_op_member, , body) code                        \
    };                                                                         \
    template <>                                                                \
    struct reflect::Info<refname> {                                            \
        static constexpr bool reflexible = true;                               \
        static constexpr unsigned size = PP_SEQ_SIZE(body);                    \
        static constexpr const char* name = #refname;                          \
        static constexpr const char* member_types[] = { reflect_types(body) }; \
        static constexpr const char* member_names[] = { reflect_names(body) }; \
        template <unsigned I>                                                  \
        struct member {                                                        \
            static constexpr unsigned index = I;                               \
            static constexpr const char* type = member_types[index];           \
            static constexpr const char* name = member_names[index];           \
        };                                                                     \
    };                                                                         \
    template <unsigned I, typename R>                                          \
    struct reflect::Member<I, R, refname> : public Info<refname>::member<I> {  \
        typedef R holder_type;                                                 \
        holder_type& holder;                                                   \
        typedef decltype(ref<I>(reflect_call(holder, body))) value_type;       \
        value_type& value;                                                     \
    public:                                                                    \
        inline Member(holder_type& holder)                                     \
            : holder(holder), value(ref<I>(reflect_call(holder, body)))        \
        {}                                                                     \
    };

#define reflect_members(args...)            \
    PP_SEQ_TRANSFORM(reflect_op_members2, , \
        PP_VARIADIC_SEQ_TO_SEQ(             \
            PP_SEQ_FOR_EACH(reflect_op_members1, , PP_VARIADIC_TO_SEQ(args))))
#define reflect_op_members1(r, data, elem) elem
#define reflect_op_members2(r, data, elem)  \
    PP_TUPLE_PUSH_FRONT(                    \
        PP_TUPLE_POP_BACK(elem),            \
            PP_TUPLE_ELEM(PP_DEC(PP_TUPLE_SIZE(elem)), elem))

#define reflect_op_member(r, ignore, elem) reflect_expand_member elem
#define reflect_expand_member(name, type...) type name;

#define reflect_types(body) \
        reflect_transform(reflect_op_types1, reflect_op_types2, , body)
#define reflect_op_types1(r, data, elem) PP_TUPLE_POP_FRONT(elem)
#define reflect_op_types2(r, data, elem) reflect_expand_types(elem)
#define reflect_expand_types(type) \
        reflect_str(PP_SEQ_ENUM(PP_TUPLE_TO_SEQ(type))),

#define reflect_names(body) \
        reflect_transform(reflect_op_names1, reflect_op_names2, , body)
#define reflect_op_names1(r, data, elem) PP_TUPLE_ELEM(0, elem)
#define reflect_op_names2(r, data, elem) reflect_str(elem),

#define reflect_call(object, body) \
        reflect_transform(reflect_op_call1, reflect_op_call2, object, body)
#define reflect_op_call1(r, object, elem) (object.PP_TUPLE_ELEM(0, elem))
#define reflect_op_call2(r, object, elem) reflect_expand elem,

#define reflect_transform(op1, op2, extra, body) \
    PP_TUPLE_ENUM(PP_TUPLE_POP_BACK(             \
        (PP_SEQ_FOR_EACH(op2, , PP_SEQ_TRANSFORM(op1, extra, body)))))

#define reflect_str(args...) reflect_stri(args)
#define reflect_stri(args...) #args
#define reflect_expand(args...) args

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
    static typename std::tuple_element<I, std::tuple<T&...>>::type
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
