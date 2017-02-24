# ifndef PREPROCESSOR_ARITHMETIC_DETAIL_DIV_BASE_HPP
# define PREPROCESSOR_ARITHMETIC_DETAIL_DIV_BASE_HPP
#
# include <takisy/preprocessor/arithmetic/inc.hpp>
# include <takisy/preprocessor/arithmetic/sub.hpp>
# include <takisy/preprocessor/comparison/less_equal.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/while.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
# include <takisy/preprocessor/tuple/rem.hpp>
#
# /* PP_DIV_BASE */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_DIV_BASE(x, y) PP_WHILE(PP_DIV_BASE_P, PP_DIV_BASE_O, (0, x, y))
# else
#    define PP_DIV_BASE(x, y) PP_DIV_BASE_I(x, y)
#    define PP_DIV_BASE_I(x, y) PP_WHILE(PP_DIV_BASE_P, PP_DIV_BASE_O, (0, x, y))
# endif
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_STRICT()
#    define PP_DIV_BASE_P(d, rxy) PP_DIV_BASE_P_IM(d, PP_TUPLE_REM_3 rxy)
#    define PP_DIV_BASE_P_IM(d, im) PP_DIV_BASE_P_I(d, im)
# else
#    define PP_DIV_BASE_P(d, rxy) PP_DIV_BASE_P_I(d, PP_TUPLE_ELEM(3, 0, rxy), PP_TUPLE_ELEM(3, 1, rxy), PP_TUPLE_ELEM(3, 2, rxy))
# endif
#
# define PP_DIV_BASE_P_I(d, r, x, y) PP_LESS_EQUAL_D(d, y, x)
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_STRICT()
#    define PP_DIV_BASE_O(d, rxy) PP_DIV_BASE_O_IM(d, PP_TUPLE_REM_3 rxy)
#    define PP_DIV_BASE_O_IM(d, im) PP_DIV_BASE_O_I(d, im)
# else
#    define PP_DIV_BASE_O(d, rxy) PP_DIV_BASE_O_I(d, PP_TUPLE_ELEM(3, 0, rxy), PP_TUPLE_ELEM(3, 1, rxy), PP_TUPLE_ELEM(3, 2, rxy))
# endif
#
# define PP_DIV_BASE_O_I(d, r, x, y) (PP_INC(r), PP_SUB_D(d, x, y), y)
#
# /* PP_DIV_BASE_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_DIV_BASE_D(d, x, y) PP_WHILE_ ## d(PP_DIV_BASE_P, PP_DIV_BASE_O, (0, x, y))
# else
#    define PP_DIV_BASE_D(d, x, y) PP_DIV_BASE_D_I(d, x, y)
#    define PP_DIV_BASE_D_I(d, x, y) PP_WHILE_ ## d(PP_DIV_BASE_P, PP_DIV_BASE_O, (0, x, y))
# endif
#
# endif
