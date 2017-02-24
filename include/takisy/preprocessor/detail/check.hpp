# ifndef PREPROCESSOR_DETAIL_CHECK_HPP
# define PREPROCESSOR_DETAIL_CHECK_HPP
#
# include <takisy/preprocessor/cat.hpp>
# include <takisy/preprocessor/config/config.hpp>
#
# /* PP_CHECK */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_CHECK(x, type) PP_CHECK_D(x, type)
# else
#    define PP_CHECK(x, type) PP_CHECK_OO((x, type))
#    define PP_CHECK_OO(par) PP_CHECK_D ## par
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MSVC() && ~PP_CONFIG_FLAGS() & PP_CONFIG_DMC()
#    define PP_CHECK_D(x, type) PP_CHECK_1(PP_CAT(PP_CHECK_RESULT_, type x))
#    define PP_CHECK_1(chk) PP_CHECK_2(chk)
#    define PP_CHECK_2(res, _) res
# elif PP_CONFIG_FLAGS() & PP_CONFIG_MSVC()
#    define PP_CHECK_D(x, type) PP_CHECK_1(type x)
#    define PP_CHECK_1(chk) PP_CHECK_2(chk)
#    define PP_CHECK_2(chk) PP_CHECK_3((PP_CHECK_RESULT_ ## chk))
#    define PP_CHECK_3(im) PP_CHECK_5(PP_CHECK_4 im)
#    define PP_CHECK_4(res, _) res
#    define PP_CHECK_5(res) res
# else /* DMC */
#    define PP_CHECK_D(x, type) PP_CHECK_OO((type x))
#    define PP_CHECK_OO(par) PP_CHECK_0 ## par
#    define PP_CHECK_0(chk) PP_CHECK_1(PP_CAT(PP_CHECK_RESULT_, chk))
#    define PP_CHECK_1(chk) PP_CHECK_2(chk)
#    define PP_CHECK_2(res, _) res
# endif
#
# define PP_CHECK_RESULT_1 1, PP_NIL
#
# endif
