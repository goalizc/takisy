# ifndef PREPROCESSOR_DETAIL_IS_UNARY_HPP
# define PREPROCESSOR_DETAIL_IS_UNARY_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/detail/check.hpp>
#
# /* PP_IS_UNARY */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_IS_UNARY(x) PP_CHECK(x, PP_IS_UNARY_CHECK)
# else
#    define PP_IS_UNARY(x) PP_IS_UNARY_I(x)
#    define PP_IS_UNARY_I(x) PP_CHECK(x, PP_IS_UNARY_CHECK)
# endif
#
# define PP_IS_UNARY_CHECK(a) 1
# define PP_CHECK_RESULT_PP_IS_UNARY_CHECK 0, PP_NIL
#
# endif
