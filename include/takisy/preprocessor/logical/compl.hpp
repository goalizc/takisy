# ifndef PREPROCESSOR_LOGICAL_COMPL_HPP
# define PREPROCESSOR_LOGICAL_COMPL_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# /* PP_COMPL */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_COMPL(x) PP_COMPL_I(x)
# else
#    define PP_COMPL(x) PP_COMPL_OO((x))
#    define PP_COMPL_OO(par) PP_COMPL_I ## par
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MSVC()
#    define PP_COMPL_I(x) PP_COMPL_ ## x
# else
#    define PP_COMPL_I(x) PP_COMPL_ID(PP_COMPL_ ## x)
#    define PP_COMPL_ID(id) id
# endif
#
# define PP_COMPL_0 1
# define PP_COMPL_1 0
#
# endif
