# ifndef PREPROCESSOR_COMPARISON_LESS_EQUAL_HPP
# define PREPROCESSOR_COMPARISON_LESS_EQUAL_HPP
#
# include <takisy/preprocessor/arithmetic/sub.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/logical/not.hpp>
#
# /* PP_LESS_EQUAL */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LESS_EQUAL(x, y) PP_NOT(PP_SUB(x, y))
# else
#    define PP_LESS_EQUAL(x, y) PP_LESS_EQUAL_I(x, y)
#    define PP_LESS_EQUAL_I(x, y) PP_NOT(PP_SUB(x, y))
# endif
#
# /* PP_LESS_EQUAL_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LESS_EQUAL_D(d, x, y) PP_NOT(PP_SUB_D(d, x, y))
# else
#    define PP_LESS_EQUAL_D(d, x, y) PP_LESS_EQUAL_D_I(d, x, y)
#    define PP_LESS_EQUAL_D_I(d, x, y) PP_NOT(PP_SUB_D(d, x, y))
# endif
#
# endif
