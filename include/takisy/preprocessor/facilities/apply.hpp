# ifndef PREPROCESSOR_FACILITIES_APPLY_HPP
# define PREPROCESSOR_FACILITIES_APPLY_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/expr_iif.hpp>
# include <takisy/preprocessor/detail/is_unary.hpp>
# include <takisy/preprocessor/tuple/rem.hpp>
#
# /* PP_APPLY */
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_APPLY(x) PP_APPLY_I(x)
#    define PP_APPLY_I(x) PP_EXPR_IIF(PP_IS_UNARY(x), PP_TUPLE_REM_1 x)
# elif PP_CONFIG_FLAGS() & PP_CONFIG_BCC()
#    define PP_APPLY(x) PP_APPLY_I(x)
#    define PP_APPLY_I(x) PP_APPLY_ ## x
#    define PP_APPLY_(x) x
#    define PP_APPLY_PP_NIL
# else
#    define PP_APPLY(x) PP_EXPR_IIF(PP_IS_UNARY(x), PP_TUPLE_REM_1 x)
# endif
#
# endif
