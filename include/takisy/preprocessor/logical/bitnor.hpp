# ifndef PREPROCESSOR_LOGICAL_BITNOR_HPP
# define PREPROCESSOR_LOGICAL_BITNOR_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# /* PP_BITNOR */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_BITNOR(x, y) PP_BITNOR_I(x, y)
# else
#    define PP_BITNOR(x, y) PP_BITNOR_OO((x, y))
#    define PP_BITNOR_OO(par) PP_BITNOR_I ## par
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MSVC()
#    define PP_BITNOR_I(x, y) PP_BITNOR_ ## x ## y
# else
#    define PP_BITNOR_I(x, y) PP_BITNOR_ID(PP_BITNOR_ ## x ## y)
#    define PP_BITNOR_ID(id) id
# endif
#
# define PP_BITNOR_00 1
# define PP_BITNOR_01 0
# define PP_BITNOR_10 0
# define PP_BITNOR_11 0
#
# endif
