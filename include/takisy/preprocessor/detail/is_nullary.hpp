# ifndef PREPROCESSOR_DETAIL_IS_NULLARY_HPP
# define PREPROCESSOR_DETAIL_IS_NULLARY_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/detail/check.hpp>
#
# /* PP_IS_NULLARY */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_IS_NULLARY(x) PP_CHECK(x, PP_IS_NULLARY_CHECK)
# else
#    define PP_IS_NULLARY(x) PP_IS_NULLARY_I(x)
#    define PP_IS_NULLARY_I(x) PP_CHECK(x, PP_IS_NULLARY_CHECK)
# endif
#
# define PP_IS_NULLARY_CHECK() 1
# define PP_CHECK_RESULT_PP_IS_NULLARY_CHECK 0, PP_NIL
#
# endif
