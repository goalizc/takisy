# ifndef PREPROCESSOR_LOGICAL_BITXOR_HPP
# define PREPROCESSOR_LOGICAL_BITXOR_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# /* PP_BITXOR */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_BITXOR(x, y) PP_BITXOR_I(x, y)
# else
#    define PP_BITXOR(x, y) PP_BITXOR_OO((x, y))
#    define PP_BITXOR_OO(par) PP_BITXOR_I ## par
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MSVC()
#    define PP_BITXOR_I(x, y) PP_BITXOR_ ## x ## y
# else
#    define PP_BITXOR_I(x, y) PP_BITXOR_ID(PP_BITXOR_ ## x ## y)
#    define PP_BITXOR_ID(id) id
# endif
#
# define PP_BITXOR_00 0
# define PP_BITXOR_01 1
# define PP_BITXOR_10 1
# define PP_BITXOR_11 0
#
# endif
