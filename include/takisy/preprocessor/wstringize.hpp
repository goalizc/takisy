# ifndef PREPROCESSOR_WSTRINGIZE_HPP
# define PREPROCESSOR_WSTRINGIZE_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# /* PP_WSTRINGIZE */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_WSTRINGIZE(text) PP_WSTRINGIZE_I(text)
# else
#    define PP_WSTRINGIZE(text) PP_WSTRINGIZE_OO((text))
#    define PP_WSTRINGIZE_OO(par) PP_WSTRINGIZE_I ## par
# endif
#
# define PP_WSTRINGIZE_I(text) PP_WSTRINGIZE_II(#text)
# define PP_WSTRINGIZE_II(str) L ## str
#
# endif
