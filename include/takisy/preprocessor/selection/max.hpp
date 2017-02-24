# ifndef PREPROCESSOR_SELECTION_MAX_HPP
# define PREPROCESSOR_SELECTION_MAX_HPP
#
# include <takisy/preprocessor/comparison/less_equal.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/iif.hpp>
#
# /* PP_MAX */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_MAX(x, y) PP_IIF(PP_LESS_EQUAL(x, y), y, x)
# else
#    define PP_MAX(x, y) PP_MAX_I(x, y)
#    define PP_MAX_I(x, y) PP_IIF(PP_LESS_EQUAL(x, y), y, x)
# endif
#
# /* PP_MAX_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_MAX_D(d, x, y) PP_IIF(PP_LESS_EQUAL_D(d, x, y), y, x)
# else
#    define PP_MAX_D(d, x, y) PP_MAX_D_I(d, x, y)
#    define PP_MAX_D_I(d, x, y) PP_IIF(PP_LESS_EQUAL_D(d, x, y), y, x)
# endif
#
# endif
