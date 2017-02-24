# ifndef PREPROCESSOR_CAT_HPP
# define PREPROCESSOR_CAT_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# /* PP_CAT */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_CAT(a, b) PP_CAT_I(a, b)
# else
#    define PP_CAT(a, b) PP_CAT_OO((a, b))
#    define PP_CAT_OO(par) PP_CAT_I ## par
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MSVC()
#    define PP_CAT_I(a, b) a ## b
# else
#    define PP_CAT_I(a, b) PP_CAT_II(~, a ## b)
#    define PP_CAT_II(p, res) res
# endif
#
# endif
