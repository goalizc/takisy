# ifndef PREPROCESSOR_LIST_FIRST_N_HPP
# define PREPROCESSOR_LIST_FIRST_N_HPP
#
# include <takisy/preprocessor/arithmetic/dec.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/while.hpp>
# include <takisy/preprocessor/list/adt.hpp>
# include <takisy/preprocessor/list/reverse.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
# include <takisy/preprocessor/tuple/rem.hpp>
#
# /* PP_LIST_FIRST_N */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_FIRST_N(count, list) PP_LIST_REVERSE(PP_TUPLE_ELEM(3, 2, PP_WHILE(PP_LIST_FIRST_N_P, PP_LIST_FIRST_N_O, (count, list, PP_NIL))))
# else
#    define PP_LIST_FIRST_N(count, list) PP_LIST_FIRST_N_I(count, list)
#    define PP_LIST_FIRST_N_I(count, list) PP_LIST_REVERSE(PP_TUPLE_ELEM(3, 2, PP_WHILE(PP_LIST_FIRST_N_P, PP_LIST_FIRST_N_O, (count, list, PP_NIL))))
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_FIRST_N_P(d, data) PP_TUPLE_ELEM(3, 0, data)
# else
#    define PP_LIST_FIRST_N_P(d, data) PP_LIST_FIRST_N_P_I data
#    define PP_LIST_FIRST_N_P_I(c, l, nl) c
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_LIST_FIRST_N_O(d, data) PP_LIST_FIRST_N_O_D data
# else
#    define PP_LIST_FIRST_N_O(d, data) PP_LIST_FIRST_N_O_D(PP_TUPLE_ELEM(3, 0, data), PP_TUPLE_ELEM(3, 1, data), PP_TUPLE_ELEM(3, 2, data))
# endif
#
# define PP_LIST_FIRST_N_O_D(c, l, nl) (PP_DEC(c), PP_LIST_REST(l), (PP_LIST_FIRST(l), nl))
#
# /* PP_LIST_FIRST_N_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_FIRST_N_D(d, count, list) PP_LIST_REVERSE_D(d, PP_TUPLE_ELEM(3, 2, PP_WHILE_ ## d(PP_LIST_FIRST_N_P, PP_LIST_FIRST_N_O, (count, list, PP_NIL))))
# else
#    define PP_LIST_FIRST_N_D(d, count, list) PP_LIST_FIRST_N_D_I(d, count, list)
#    define PP_LIST_FIRST_N_D_I(d, count, list) PP_LIST_REVERSE_D(d, PP_TUPLE_ELEM(3, 2, PP_WHILE_ ## d(PP_LIST_FIRST_N_P, PP_LIST_FIRST_N_O, (count, list, PP_NIL))))
# endif
#
# endif
