# ifndef PREPROCESSOR_CONTROL_IF_HPP
# define PREPROCESSOR_CONTROL_IF_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/iif.hpp>
# include <takisy/preprocessor/logical/bool.hpp>
#
# /* PP_IF */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_IF(cond, t, f) PP_IIF(PP_BOOL(cond), t, f)
# else
#    define PP_IF(cond, t, f) PP_IF_I(cond, t, f)
#    define PP_IF_I(cond, t, f) PP_IIF(PP_BOOL(cond), t, f)
# endif
#
# endif
