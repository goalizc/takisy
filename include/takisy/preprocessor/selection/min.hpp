# ifndef PREPROCESSOR_SELECTION_MIN_HPP
# define PREPROCESSOR_SELECTION_MIN_HPP
#
# include <takisy/preprocessor/comparison/less_equal.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/iif.hpp>
#
# /* PP_MIN */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_MIN(x, y) PP_IIF(PP_LESS_EQUAL(y, x), y, x)
# else
#    define PP_MIN(x, y) PP_MIN_I(x, y)
#    define PP_MIN_I(x, y) PP_IIF(PP_LESS_EQUAL(y, x), y, x)
# endif
#
# /* PP_MIN_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_MIN_D(d, x, y) PP_IIF(PP_LESS_EQUAL_D(d, y, x), y, x)
# else
#    define PP_MIN_D(d, x, y) PP_MIN_D_I(d, x, y)
#    define PP_MIN_D_I(d, x, y) PP_IIF(PP_LESS_EQUAL_D(d, y, x), y, x)
# endif
#
# endif
