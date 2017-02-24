# ifndef PREPROCESSOR_COMPARISON_GREATER_HPP
# define PREPROCESSOR_COMPARISON_GREATER_HPP
#
# include <takisy/preprocessor/comparison/less.hpp>
# include <takisy/preprocessor/config/config.hpp>
#
# /* PP_GREATER */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_GREATER(x, y) PP_LESS(y, x)
# else
#    define PP_GREATER(x, y) PP_GREATER_I(x, y)
#    define PP_GREATER_I(x, y) PP_LESS(y, x)
# endif
#
# /* PP_GREATER_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_GREATER_D(d, x, y) PP_LESS_D(d, y, x)
# else
#    define PP_GREATER_D(d, x, y) PP_GREATER_D_I(d, x, y)
#    define PP_GREATER_D_I(d, x, y) PP_LESS_D(d, y, x)
# endif
#
# endif
