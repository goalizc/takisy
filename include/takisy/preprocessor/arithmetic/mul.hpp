# ifndef PREPROCESSOR_ARITHMETIC_MUL_HPP
# define PREPROCESSOR_ARITHMETIC_MUL_HPP
#
# include <takisy/preprocessor/arithmetic/add.hpp>
# include <takisy/preprocessor/arithmetic/dec.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/while.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
# include <takisy/preprocessor/tuple/rem.hpp>
#
# /* PP_MUL */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_MUL(x, y) PP_TUPLE_ELEM(3, 0, PP_WHILE(PP_MUL_P, PP_MUL_O, (0, x, y)))
# else
#    define PP_MUL(x, y) PP_MUL_I(x, y)
#    define PP_MUL_I(x, y) PP_TUPLE_ELEM(3, 0, PP_WHILE(PP_MUL_P, PP_MUL_O, (0, x, y)))
# endif
#
# define PP_MUL_P(d, rxy) PP_TUPLE_ELEM(3, 2, rxy)
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_STRICT()
#    define PP_MUL_O(d, rxy) PP_MUL_O_IM(d, PP_TUPLE_REM_3 rxy)
#    define PP_MUL_O_IM(d, im) PP_MUL_O_I(d, im)
# else
#    define PP_MUL_O(d, rxy) PP_MUL_O_I(d, PP_TUPLE_ELEM(3, 0, rxy), PP_TUPLE_ELEM(3, 1, rxy), PP_TUPLE_ELEM(3, 2, rxy))
# endif
#
# define PP_MUL_O_I(d, r, x, y) (PP_ADD_D(d, r, x), x, PP_DEC(y))
#
# /* PP_MUL_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_MUL_D(d, x, y) PP_TUPLE_ELEM(3, 0, PP_WHILE_ ## d(PP_MUL_P, PP_MUL_O, (0, x, y)))
# else
#    define PP_MUL_D(d, x, y) PP_MUL_D_I(d, x, y)
#    define PP_MUL_D_I(d, x, y) PP_TUPLE_ELEM(3, 0, PP_WHILE_ ## d(PP_MUL_P, PP_MUL_O, (0, x, y)))
# endif
#
# endif
