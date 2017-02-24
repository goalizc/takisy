# ifndef PREPROCESSOR_DETAIL_IS_BINARY_HPP
# define PREPROCESSOR_DETAIL_IS_BINARY_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/detail/check.hpp>
#
# /* PP_IS_BINARY */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_IS_BINARY(x) PP_CHECK(x, PP_IS_BINARY_CHECK)
# else
#    define PP_IS_BINARY(x) PP_IS_BINARY_I(x)
#    define PP_IS_BINARY_I(x) PP_CHECK(x, PP_IS_BINARY_CHECK)
# endif
#
# define PP_IS_BINARY_CHECK(a, b) 1
# define PP_CHECK_RESULT_PP_IS_BINARY_CHECK 0, PP_NIL
#
# endif
