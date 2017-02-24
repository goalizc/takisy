# ifndef PREPROCESSOR_LIST_SIZE_HPP
# define PREPROCESSOR_LIST_SIZE_HPP
#
# include <takisy/preprocessor/arithmetic/inc.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/while.hpp>
# include <takisy/preprocessor/list/adt.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
# include <takisy/preprocessor/tuple/rem.hpp>
#
# /* PP_LIST_SIZE */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_SIZE(list) PP_TUPLE_ELEM(2, 0, PP_WHILE(PP_LIST_SIZE_P, PP_LIST_SIZE_O, (0, list)))
# else
#    define PP_LIST_SIZE(list) PP_LIST_SIZE_I(list)
#    define PP_LIST_SIZE_I(list) PP_TUPLE_ELEM(2, 0, PP_WHILE(PP_LIST_SIZE_P, PP_LIST_SIZE_O, (0, list)))
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_SIZE_P(d, rl) PP_LIST_IS_CONS(PP_TUPLE_ELEM(2, 1, rl))
# else
#    define PP_LIST_SIZE_P(d, rl) PP_LIST_SIZE_P_I(PP_TUPLE_REM_2 rl)
#    define PP_LIST_SIZE_P_I(im) PP_LIST_SIZE_P_II(im)
#    define PP_LIST_SIZE_P_II(r, l) PP_LIST_IS_CONS(l)
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_SIZE_O(d, rl) (PP_INC(PP_TUPLE_ELEM(2, 0, rl)), PP_LIST_REST(PP_TUPLE_ELEM(2, 1, rl)))
# else
#    define PP_LIST_SIZE_O(d, rl) PP_LIST_SIZE_O_I(PP_TUPLE_REM_2 rl)
#    define PP_LIST_SIZE_O_I(im) PP_LIST_SIZE_O_II(im)
#    define PP_LIST_SIZE_O_II(r, l) (PP_INC(r), PP_LIST_REST(l))
# endif
#
# /* PP_LIST_SIZE_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_SIZE_D(d, list) PP_TUPLE_ELEM(2, 0, PP_WHILE_ ## d(PP_LIST_SIZE_P, PP_LIST_SIZE_O, (0, list)))
# else
#    define PP_LIST_SIZE_D(d, list) PP_LIST_SIZE_D_I(d, list)
#    define PP_LIST_SIZE_D_I(d, list) PP_TUPLE_ELEM(2, 0, PP_WHILE_ ## d(PP_LIST_SIZE_P, PP_LIST_SIZE_O, (0, list)))
# endif
#
# endif
