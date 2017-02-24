# ifndef PREPROCESSOR_COMPARISON_LESS_HPP
# define PREPROCESSOR_COMPARISON_LESS_HPP
#
# include <takisy/preprocessor/comparison/less_equal.hpp>
# include <takisy/preprocessor/comparison/not_equal.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/iif.hpp>
# include <takisy/preprocessor/logical/bitand.hpp>
# include <takisy/preprocessor/tuple/eat.hpp>
#
# /* PP_LESS */
#
# if PP_CONFIG_FLAGS() & (PP_CONFIG_MWCC() | PP_CONFIG_DMC())
#    define PP_LESS(x, y) PP_BITAND(PP_NOT_EQUAL(x, y), PP_LESS_EQUAL(x, y))
# elif ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LESS(x, y) PP_IIF(PP_NOT_EQUAL(x, y), PP_LESS_EQUAL, 0 PP_TUPLE_EAT_2)(x, y)
# else
#    define PP_LESS(x, y) PP_LESS_I(x, y)
#    define PP_LESS_I(x, y) PP_IIF(PP_NOT_EQUAL(x, y), PP_LESS_EQUAL, 0 PP_TUPLE_EAT_2)(x, y)
# endif
#
# /* PP_LESS_D */
#
# if PP_CONFIG_FLAGS() & (PP_CONFIG_MWCC() | PP_CONFIG_DMC())
#    define PP_LESS_D(d, x, y) PP_BITAND(PP_NOT_EQUAL(x, y), PP_LESS_EQUAL_D(d, x, y))
# elif ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LESS_D(d, x, y) PP_IIF(PP_NOT_EQUAL(x, y), PP_LESS_EQUAL_D, 0 PP_TUPLE_EAT_3)(d, x, y)
# else
#    define PP_LESS_D(d, x, y) PP_LESS_D_I(d, x, y)
#    define PP_LESS_D_I(d, x, y) PP_IIF(PP_NOT_EQUAL(x, y), PP_LESS_EQUAL_D, 0 PP_TUPLE_EAT_3)(d, x, y)
# endif
#
# endif
