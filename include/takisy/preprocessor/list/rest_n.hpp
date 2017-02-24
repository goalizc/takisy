# ifndef PREPROCESSOR_LIST_REST_N_HPP
# define PREPROCESSOR_LIST_REST_N_HPP
#
# include <takisy/preprocessor/arithmetic/dec.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/while.hpp>
# include <takisy/preprocessor/list/adt.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
#
# /* PP_LIST_REST_N */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_REST_N(count, list) PP_TUPLE_ELEM(2, 0, PP_WHILE(PP_LIST_REST_N_P, PP_LIST_REST_N_O, (list, count)))
# else
#    define PP_LIST_REST_N(count, list) PP_LIST_REST_N_I(count, list)
#    define PP_LIST_REST_N_I(count, list) PP_TUPLE_ELEM(2, 0, PP_WHILE(PP_LIST_REST_N_P, PP_LIST_REST_N_O, (list, count)))
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_REST_N_P(d, lc) PP_TUPLE_ELEM(2, 1, lc)
# else
#    define PP_LIST_REST_N_P(d, lc) PP_LIST_REST_N_P_I lc
#    define PP_LIST_REST_N_P_I(list, count) count
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_REST_N_O(d, lc) (PP_LIST_REST(PP_TUPLE_ELEM(2, 0, lc)), PP_DEC(PP_TUPLE_ELEM(2, 1, lc)))
# else
#    define PP_LIST_REST_N_O(d, lc) PP_LIST_REST_N_O_I lc
#    define PP_LIST_REST_N_O_I(list, count) (PP_LIST_REST(list), PP_DEC(count))
# endif
#
# /* PP_LIST_REST_N_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_REST_N_D(d, count, list) PP_TUPLE_ELEM(2, 0, PP_WHILE_ ## d(PP_LIST_REST_N_P, PP_LIST_REST_N_O, (list, count)))
# else
#    define PP_LIST_REST_N_D(d, count, list) PP_LIST_REST_N_D_I(d, count, list)
#    define PP_LIST_REST_N_D_I(d, count, list) PP_TUPLE_ELEM(2, 0, PP_WHILE_ ## d(PP_LIST_REST_N_P, PP_LIST_REST_N_O, (list, count)))
# endif
#
# endif
