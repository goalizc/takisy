# ifndef PREPROCESSOR_LIST_LIST_FOR_EACH_I_HPP
# define PREPROCESSOR_LIST_LIST_FOR_EACH_I_HPP
#
# include <takisy/preprocessor/arithmetic/inc.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/list/adt.hpp>
# include <takisy/preprocessor/repetition/for.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
# include <takisy/preprocessor/tuple/rem.hpp>
#
# /* PP_LIST_FOR_EACH_I */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG() && ~PP_CONFIG_FLAGS() & PP_CONFIG_MSVC()
#    define PP_LIST_FOR_EACH_I(macro, data, list) PP_FOR((macro, data, list, 0), PP_LIST_FOR_EACH_I_P, PP_LIST_FOR_EACH_I_O, PP_LIST_FOR_EACH_I_M)
# else
#    define PP_LIST_FOR_EACH_I(macro, data, list) PP_LIST_FOR_EACH_I_I(macro, data, list)
#    define PP_LIST_FOR_EACH_I_I(macro, data, list) PP_FOR((macro, data, list, 0), PP_LIST_FOR_EACH_I_P, PP_LIST_FOR_EACH_I_O, PP_LIST_FOR_EACH_I_M)
# endif
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_STRICT()
#    define PP_LIST_FOR_EACH_I_P(r, x) PP_LIST_FOR_EACH_I_P_D x
#    define PP_LIST_FOR_EACH_I_P_D(m, d, l, i) PP_LIST_IS_CONS(l)
# else
#    define PP_LIST_FOR_EACH_I_P(r, x) PP_LIST_IS_CONS(PP_TUPLE_ELEM(4, 2, x))
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_LIST_FOR_EACH_I_O(r, x) PP_LIST_FOR_EACH_I_O_D x
#    define PP_LIST_FOR_EACH_I_O_D(m, d, l, i) (m, d, PP_LIST_REST(l), PP_INC(i))
# else
#    define PP_LIST_FOR_EACH_I_O(r, x) (PP_TUPLE_ELEM(4, 0, x), PP_TUPLE_ELEM(4, 1, x), PP_LIST_REST(PP_TUPLE_ELEM(4, 2, x)), PP_INC(PP_TUPLE_ELEM(4, 3, x)))
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_FOR_EACH_I_M(r, x) PP_LIST_FOR_EACH_I_M_D(r, PP_TUPLE_ELEM(4, 0, x), PP_TUPLE_ELEM(4, 1, x), PP_TUPLE_ELEM(4, 2, x), PP_TUPLE_ELEM(4, 3, x))
# else
#    define PP_LIST_FOR_EACH_I_M(r, x) PP_LIST_FOR_EACH_I_M_I(r, PP_TUPLE_REM_4 x)
#    define PP_LIST_FOR_EACH_I_M_I(r, x_e) PP_LIST_FOR_EACH_I_M_D(r, x_e)
# endif
#
# define PP_LIST_FOR_EACH_I_M_D(r, m, d, l, i) m(r, d, i, PP_LIST_FIRST(l))
#
# /* PP_LIST_FOR_EACH_I_R */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_FOR_EACH_I_R(r, macro, data, list) PP_FOR_ ## r((macro, data, list, 0), PP_LIST_FOR_EACH_I_P, PP_LIST_FOR_EACH_I_O, PP_LIST_FOR_EACH_I_M)
# else
#    define PP_LIST_FOR_EACH_I_R(r, macro, data, list) PP_LIST_FOR_EACH_I_R_I(r, macro, data, list)
#    define PP_LIST_FOR_EACH_I_R_I(r, macro, data, list) PP_FOR_ ## r((macro, data, list, 0), PP_LIST_FOR_EACH_I_P, PP_LIST_FOR_EACH_I_O, PP_LIST_FOR_EACH_I_M)
# endif
#
# endif
