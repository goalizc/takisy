# ifndef PREPROCESSOR_DETAIL_SPLIT_HPP
# define PREPROCESSOR_DETAIL_SPLIT_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# /* PP_SPLIT */
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_SPLIT(n, im) PP_SPLIT_I((n, im))
#    define PP_SPLIT_I(par) PP_SPLIT_II ## par
#    define PP_SPLIT_II(n, a, b) PP_SPLIT_ ## n(a, b)
# elif PP_CONFIG_FLAGS() & PP_CONFIG_MSVC()
#    define PP_SPLIT(n, im) PP_SPLIT_I(n((im)))
#    define PP_SPLIT_I(n) PP_SPLIT_ID(PP_SPLIT_II_ ## n)
#    define PP_SPLIT_II_0(s) PP_SPLIT_ID(PP_SPLIT_0 s)
#    define PP_SPLIT_II_1(s) PP_SPLIT_ID(PP_SPLIT_1 s)
#    define PP_SPLIT_ID(id) id
# else
#    define PP_SPLIT(n, im) PP_SPLIT_I(n)(im)
#    define PP_SPLIT_I(n) PP_SPLIT_ ## n
# endif
#
# define PP_SPLIT_0(a, b) a
# define PP_SPLIT_1(a, b) b
#
# endif
