# ifndef PREPROCESSOR_STRINGIZE_HPP
# define PREPROCESSOR_STRINGIZE_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# /* PP_STRINGIZE */
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_MSVC()
#    define PP_STRINGIZE(text) PP_STRINGIZE_A((text))
#    define PP_STRINGIZE_A(arg) PP_STRINGIZE_I arg
# elif PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_STRINGIZE(text) PP_STRINGIZE_OO((text))
#    define PP_STRINGIZE_OO(par) PP_STRINGIZE_I ## par
# else
#    define PP_STRINGIZE(text) PP_STRINGIZE_I(text)
# endif
#
# define PP_STRINGIZE_I(text) #text
#
# endif
