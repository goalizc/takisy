# ifndef PREPROCESSOR_COMPARISON_GREATER_EQUAL_HPP
# define PREPROCESSOR_COMPARISON_GREATER_EQUAL_HPP
#
# include <takisy/preprocessor/comparison/less_equal.hpp>
# include <takisy/preprocessor/config/config.hpp>
#
# /* PP_GREATER_EQUAL */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_GREATER_EQUAL(x, y) PP_LESS_EQUAL(y, x)
# else
#    define PP_GREATER_EQUAL(x, y) PP_GREATER_EQUAL_I(x, y)
#    define PP_GREATER_EQUAL_I(x, y) PP_LESS_EQUAL(y, x)
# endif
#
# /* PP_GREATER_EQUAL_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_GREATER_EQUAL_D(d, x, y) PP_LESS_EQUAL_D(d, y, x)
# else
#    define PP_GREATER_EQUAL_D(d, x, y) PP_GREATER_EQUAL_D_I(d, x, y)
#    define PP_GREATER_EQUAL_D_I(d, x, y) PP_LESS_EQUAL_D(d, y, x)
# endif
#
# endif
