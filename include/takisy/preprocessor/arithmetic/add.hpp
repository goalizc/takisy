# ifndef PREPROCESSOR_ARITHMETIC_ADD_HPP
# define PREPROCESSOR_ARITHMETIC_ADD_HPP
#
# include <takisy/preprocessor/arithmetic/dec.hpp>
# include <takisy/preprocessor/arithmetic/inc.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/while.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
#
# /* PP_ADD */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_ADD(x, y) PP_TUPLE_ELEM(2, 0, PP_WHILE(PP_ADD_P, PP_ADD_O, (x, y)))
# else
#    define PP_ADD(x, y) PP_ADD_I(x, y)
#    define PP_ADD_I(x, y) PP_TUPLE_ELEM(2, 0, PP_WHILE(PP_ADD_P, PP_ADD_O, (x, y)))
# endif
#
# define PP_ADD_P(d, xy) PP_TUPLE_ELEM(2, 1, xy)
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_ADD_O(d, xy) PP_ADD_O_I xy
# else
#    define PP_ADD_O(d, xy) PP_ADD_O_I(PP_TUPLE_ELEM(2, 0, xy), PP_TUPLE_ELEM(2, 1, xy))
# endif
#
# define PP_ADD_O_I(x, y) (PP_INC(x), PP_DEC(y))
#
# /* PP_ADD_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_ADD_D(d, x, y) PP_TUPLE_ELEM(2, 0, PP_WHILE_ ## d(PP_ADD_P, PP_ADD_O, (x, y)))
# else
#    define PP_ADD_D(d, x, y) PP_ADD_D_I(d, x, y)
#    define PP_ADD_D_I(d, x, y) PP_TUPLE_ELEM(2, 0, PP_WHILE_ ## d(PP_ADD_P, PP_ADD_O, (x, y)))
# endif
#
# endif
