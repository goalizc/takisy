# ifndef PREPROCESSOR_ARITHMETIC_SUB_HPP
# define PREPROCESSOR_ARITHMETIC_SUB_HPP
#
# include <takisy/preprocessor/arithmetic/dec.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/while.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
#
# /* PP_SUB */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SUB(x, y) PP_TUPLE_ELEM(2, 0, PP_WHILE(PP_SUB_P, PP_SUB_O, (x, y)))
# else
#    define PP_SUB(x, y) PP_SUB_I(x, y)
#    define PP_SUB_I(x, y) PP_TUPLE_ELEM(2, 0, PP_WHILE(PP_SUB_P, PP_SUB_O, (x, y)))
# endif
#
# define PP_SUB_P(d, xy) PP_TUPLE_ELEM(2, 1, xy)
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_SUB_O(d, xy) PP_SUB_O_I xy
# else
#    define PP_SUB_O(d, xy) PP_SUB_O_I(PP_TUPLE_ELEM(2, 0, xy), PP_TUPLE_ELEM(2, 1, xy))
# endif
#
# define PP_SUB_O_I(x, y) (PP_DEC(x), PP_DEC(y))
#
# /* PP_SUB_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SUB_D(d, x, y) PP_TUPLE_ELEM(2, 0, PP_WHILE_ ## d(PP_SUB_P, PP_SUB_O, (x, y)))
# else
#    define PP_SUB_D(d, x, y) PP_SUB_D_I(d, x, y)
#    define PP_SUB_D_I(d, x, y) PP_TUPLE_ELEM(2, 0, PP_WHILE_ ## d(PP_SUB_P, PP_SUB_O, (x, y)))
# endif
#
# endif
