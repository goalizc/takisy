# ifndef PREPROCESSOR_CONTROL_EXPR_IF_HPP
# define PREPROCESSOR_CONTROL_EXPR_IF_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/expr_iif.hpp>
# include <takisy/preprocessor/logical/bool.hpp>
#
# /* PP_EXPR_IF */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_EXPR_IF(cond, expr) PP_EXPR_IIF(PP_BOOL(cond), expr)
# else
#    define PP_EXPR_IF(cond, expr) PP_EXPR_IF_I(cond, expr)
#    define PP_EXPR_IF_I(cond, expr) PP_EXPR_IIF(PP_BOOL(cond), expr)
# endif
#
# endif
