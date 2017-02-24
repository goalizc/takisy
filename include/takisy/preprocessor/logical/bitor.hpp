# ifndef PREPROCESSOR_LOGICAL_BITOR_HPP
# define PREPROCESSOR_LOGICAL_BITOR_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# /* PP_BITOR */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_BITOR(x, y) PP_BITOR_I(x, y)
# else
#    define PP_BITOR(x, y) PP_BITOR_OO((x, y))
#    define PP_BITOR_OO(par) PP_BITOR_I ## par
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MSVC()
#    define PP_BITOR_I(x, y) PP_BITOR_ ## x ## y
# else
#    define PP_BITOR_I(x, y) PP_BITOR_ID(PP_BITOR_ ## x ## y)
#    define PP_BITOR_ID(id) id
# endif
#
# define PP_BITOR_00 0
# define PP_BITOR_01 1
# define PP_BITOR_10 1
# define PP_BITOR_11 1
#
# endif
