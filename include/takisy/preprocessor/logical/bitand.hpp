# ifndef PREPROCESSOR_LOGICAL_BITAND_HPP
# define PREPROCESSOR_LOGICAL_BITAND_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# /* PP_BITAND */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_BITAND(x, y) PP_BITAND_I(x, y)
# else
#    define PP_BITAND(x, y) PP_BITAND_OO((x, y))
#    define PP_BITAND_OO(par) PP_BITAND_I ## par
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MSVC()
#    define PP_BITAND_I(x, y) PP_BITAND_ ## x ## y
# else
#    define PP_BITAND_I(x, y) PP_BITAND_ID(PP_BITAND_ ## x ## y)
#    define PP_BITAND_ID(res) res
# endif
#
# define PP_BITAND_00 0
# define PP_BITAND_01 0
# define PP_BITAND_10 0
# define PP_BITAND_11 1
#
# endif
