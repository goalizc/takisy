# ifndef PREPROCESSOR_CONTROL_EXPR_IIF_HPP
# define PREPROCESSOR_CONTROL_EXPR_IIF_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# /* PP_EXPR_IIF */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_EXPR_IIF(bit, expr) PP_EXPR_IIF_I(bit, expr)
# else
#    define PP_EXPR_IIF(bit, expr) PP_EXPR_IIF_OO((bit, expr))
#    define PP_EXPR_IIF_OO(par) PP_EXPR_IIF_I ## par
# endif
#
# define PP_EXPR_IIF_I(bit, expr) PP_EXPR_IIF_ ## bit(expr)
#
# define PP_EXPR_IIF_0(expr)
# define PP_EXPR_IIF_1(expr) expr
#
# endif
