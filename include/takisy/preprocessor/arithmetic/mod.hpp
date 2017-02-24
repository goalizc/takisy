# ifndef PREPROCESSOR_ARITHMETIC_MOD_HPP
# define PREPROCESSOR_ARITHMETIC_MOD_HPP
#
# include <takisy/preprocessor/arithmetic/detail/div_base.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
#
# /* PP_MOD */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_MOD(x, y) PP_TUPLE_ELEM(3, 1, PP_DIV_BASE(x, y))
# else
#    define PP_MOD(x, y) PP_MOD_I(x, y)
#    define PP_MOD_I(x, y) PP_TUPLE_ELEM(3, 1, PP_DIV_BASE(x, y))
# endif
#
# /* PP_MOD_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_MOD_D(d, x, y) PP_TUPLE_ELEM(3, 1, PP_DIV_BASE_D(d, x, y))
# else
#    define PP_MOD_D(d, x, y) PP_MOD_D_I(d, x, y)
#    define PP_MOD_D_I(d, x, y) PP_TUPLE_ELEM(3, 1, PP_DIV_BASE_D(d, x, y))
# endif
#
# endif
