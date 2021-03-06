# ifndef PREPROCESSOR_LIST_FOR_EACH_PRODUCT_HPP
# define PREPROCESSOR_LIST_FOR_EACH_PRODUCT_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/if.hpp>
# include <takisy/preprocessor/list/adt.hpp>
# include <takisy/preprocessor/list/to_tuple.hpp>
# include <takisy/preprocessor/repetition/for.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
# include <takisy/preprocessor/tuple/to_list.hpp>
# include <takisy/preprocessor/tuple/rem.hpp>
# include <takisy/preprocessor/tuple/reverse.hpp>
#
# /* PP_LIST_FOR_EACH_PRODUCT */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_FOR_EACH_PRODUCT(macro, size, tuple) PP_LIST_FOR_EACH_PRODUCT_E(PP_FOR, macro, size, PP_TUPLE_TO_LIST(size, tuple))
# else
#    define PP_LIST_FOR_EACH_PRODUCT(macro, size, tuple) PP_LIST_FOR_EACH_PRODUCT_Q(macro, size, tuple)
#    define PP_LIST_FOR_EACH_PRODUCT_Q(macro, size, tuple) PP_LIST_FOR_EACH_PRODUCT_E(PP_FOR, macro, size, PP_TUPLE_TO_LIST(size, tuple))
# endif
#
# /* PP_LIST_FOR_EACH_PRODUCT_R */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_FOR_EACH_PRODUCT_R(r, macro, size, tuple) PP_LIST_FOR_EACH_PRODUCT_E(PP_FOR_ ## r, macro, size, PP_TUPLE_TO_LIST(size, tuple))
# else
#    define PP_LIST_FOR_EACH_PRODUCT_R(r, macro, size, tuple) PP_LIST_FOR_EACH_PRODUCT_R_Q(r, macro, size, tuple)
#    define PP_LIST_FOR_EACH_PRODUCT_R_Q(r, macro, size, tuple) PP_LIST_FOR_EACH_PRODUCT_E(PP_FOR_ ## r, macro, size, PP_TUPLE_TO_LIST(size, tuple))
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_FOR_EACH_PRODUCT_E(impl, macro, size, lists) impl((PP_LIST_FIRST(lists), PP_LIST_REST(lists), PP_NIL, macro, size), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_0)
# else
#    define PP_LIST_FOR_EACH_PRODUCT_E(impl, macro, size, lists) PP_LIST_FOR_EACH_PRODUCT_E_D(impl, macro, size, lists)
#    define PP_LIST_FOR_EACH_PRODUCT_E_D(impl, macro, size, lists) impl((PP_LIST_FIRST(lists), PP_LIST_REST(lists), PP_NIL, macro, size), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_0)
# endif
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_STRICT()
#    define PP_LIST_FOR_EACH_PRODUCT_P(r, data) PP_LIST_FOR_EACH_PRODUCT_P_I data
#    define PP_LIST_FOR_EACH_PRODUCT_P_I(a, b, res, macro, size) PP_LIST_IS_CONS(a)
# else
#    define PP_LIST_FOR_EACH_PRODUCT_P(r, data) PP_LIST_IS_CONS(PP_TUPLE_ELEM(5, 0, data))
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_LIST_FOR_EACH_PRODUCT_O(r, data) PP_LIST_FOR_EACH_PRODUCT_O_I data
#    define PP_LIST_FOR_EACH_PRODUCT_O_I(a, b, res, macro, size) (PP_LIST_REST(a), b, res, macro, size)
# else
#    define PP_LIST_FOR_EACH_PRODUCT_O(r, data) (PP_LIST_REST(PP_TUPLE_ELEM(5, 0, data)), PP_TUPLE_ELEM(5, 1, data), PP_TUPLE_ELEM(5, 2, data), PP_TUPLE_ELEM(5, 3, data), PP_TUPLE_ELEM(5, 4, data))
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_FOR_EACH_PRODUCT_I(r, data) PP_LIST_FOR_EACH_PRODUCT_I_I(r, PP_TUPLE_ELEM(5, 0, data), PP_TUPLE_ELEM(5, 1, data), PP_TUPLE_ELEM(5, 2, data), PP_TUPLE_ELEM(5, 3, data), PP_TUPLE_ELEM(5, 4, data))
# else
#    define PP_LIST_FOR_EACH_PRODUCT_I(r, data) PP_LIST_FOR_EACH_PRODUCT_I_D(r, PP_TUPLE_REM_5 data)
#    define PP_LIST_FOR_EACH_PRODUCT_I_D(r, data_e) PP_LIST_FOR_EACH_PRODUCT_I_I(r, data_e)
# endif
#
# define PP_LIST_FOR_EACH_PRODUCT_I_I(r, a, b, res, macro, size) PP_LIST_FOR_EACH_PRODUCT_I_II(r, macro, PP_LIST_TO_TUPLE_R(r, (PP_LIST_FIRST(a), res)), size)
# define PP_LIST_FOR_EACH_PRODUCT_I_II(r, macro, args, size) PP_LIST_FOR_EACH_PRODUCT_I_III(r, macro, args, size)
# define PP_LIST_FOR_EACH_PRODUCT_I_III(r, macro, args, size) macro(r, PP_TUPLE_REVERSE(size, args))
#
# define PP_LIST_FOR_EACH_PRODUCT_C(data, i) PP_IF(PP_LIST_IS_CONS(PP_TUPLE_ELEM(5, 1, data)), PP_LIST_FOR_EACH_PRODUCT_N_ ## i, PP_LIST_FOR_EACH_PRODUCT_I)
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_LIST_FOR_EACH_PRODUCT_H(data) PP_LIST_FOR_EACH_PRODUCT_H_I data
# else
#    define PP_LIST_FOR_EACH_PRODUCT_H(data) PP_LIST_FOR_EACH_PRODUCT_H_I(PP_TUPLE_ELEM(5, 0, data), PP_TUPLE_ELEM(5, 1, data), PP_TUPLE_ELEM(5, 2, data), PP_TUPLE_ELEM(5, 3, data), PP_TUPLE_ELEM(5, 4, data))
# endif
#
# define PP_LIST_FOR_EACH_PRODUCT_H_I(a, b, res, macro, size) (PP_LIST_FIRST(b), PP_LIST_REST(b), (PP_LIST_FIRST(a), res), macro, size)
#
# define PP_LIST_FOR_EACH_PRODUCT_M_0(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 0)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_1(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 1)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_2(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 2)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_3(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 3)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_4(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 4)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_5(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 5)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_6(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 6)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_7(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 7)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_8(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 8)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_9(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 9)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_10(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 10)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_11(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 11)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_12(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 12)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_13(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 13)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_14(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 14)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_15(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 15)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_16(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 16)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_17(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 17)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_18(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 18)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_19(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 19)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_20(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 20)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_21(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 21)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_22(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 22)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_23(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 23)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_24(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 24)(r, data)
# define PP_LIST_FOR_EACH_PRODUCT_M_25(r, data) PP_LIST_FOR_EACH_PRODUCT_C(data, 25)(r, data)
#
# define PP_LIST_FOR_EACH_PRODUCT_N_0(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_1)
# define PP_LIST_FOR_EACH_PRODUCT_N_1(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_2)
# define PP_LIST_FOR_EACH_PRODUCT_N_2(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_3)
# define PP_LIST_FOR_EACH_PRODUCT_N_3(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_4)
# define PP_LIST_FOR_EACH_PRODUCT_N_4(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_5)
# define PP_LIST_FOR_EACH_PRODUCT_N_5(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_6)
# define PP_LIST_FOR_EACH_PRODUCT_N_6(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_7)
# define PP_LIST_FOR_EACH_PRODUCT_N_7(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_8)
# define PP_LIST_FOR_EACH_PRODUCT_N_8(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_9)
# define PP_LIST_FOR_EACH_PRODUCT_N_9(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_10)
# define PP_LIST_FOR_EACH_PRODUCT_N_10(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_11)
# define PP_LIST_FOR_EACH_PRODUCT_N_11(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_12)
# define PP_LIST_FOR_EACH_PRODUCT_N_12(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_13)
# define PP_LIST_FOR_EACH_PRODUCT_N_13(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_14)
# define PP_LIST_FOR_EACH_PRODUCT_N_14(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_15)
# define PP_LIST_FOR_EACH_PRODUCT_N_15(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_16)
# define PP_LIST_FOR_EACH_PRODUCT_N_16(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_17)
# define PP_LIST_FOR_EACH_PRODUCT_N_17(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_18)
# define PP_LIST_FOR_EACH_PRODUCT_N_18(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_19)
# define PP_LIST_FOR_EACH_PRODUCT_N_19(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_20)
# define PP_LIST_FOR_EACH_PRODUCT_N_20(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_21)
# define PP_LIST_FOR_EACH_PRODUCT_N_21(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_22)
# define PP_LIST_FOR_EACH_PRODUCT_N_22(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_23)
# define PP_LIST_FOR_EACH_PRODUCT_N_23(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_24)
# define PP_LIST_FOR_EACH_PRODUCT_N_24(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_25)
# define PP_LIST_FOR_EACH_PRODUCT_N_25(r, data) PP_FOR_ ## r(PP_LIST_FOR_EACH_PRODUCT_H(data), PP_LIST_FOR_EACH_PRODUCT_P, PP_LIST_FOR_EACH_PRODUCT_O, PP_LIST_FOR_EACH_PRODUCT_M_26)
#
# endif
