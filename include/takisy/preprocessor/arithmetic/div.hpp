# ifndef PREPROCESSOR_ARITHMETIC_DIV_HPP
# define PREPROCESSOR_ARITHMETIC_DIV_HPP
#
# include <takisy/preprocessor/arithmetic/detail/div_base.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
#
# /* PP_DIV */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_DIV(x, y) PP_TUPLE_ELEM(3, 0, PP_DIV_BASE(x, y))
# else
#    define PP_DIV(x, y) PP_DIV_I(x, y)
#    define PP_DIV_I(x, y) PP_TUPLE_ELEM(3, 0, PP_DIV_BASE(x, y))
# endif
#
# /* PP_DIV_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_DIV_D(d, x, y) PP_TUPLE_ELEM(3, 0, PP_DIV_BASE_D(d, x, y))
# else
#    define PP_DIV_D(d, x, y) PP_DIV_D_I(d, x, y)
#    define PP_DIV_D_I(d, x, y) PP_TUPLE_ELEM(3, 0, PP_DIV_BASE_D(d, x, y))
# endif
#
# endif
