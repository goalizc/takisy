# ifndef PREPROCESSOR_FACILITIES_EXPAND_HPP
# define PREPROCESSOR_FACILITIES_EXPAND_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MWCC() && ~PP_CONFIG_FLAGS() & PP_CONFIG_DMC()
#    define PP_EXPAND(x) PP_EXPAND_I(x)
# else
#    define PP_EXPAND(x) PP_EXPAND_OO((x))
#    define PP_EXPAND_OO(par) PP_EXPAND_I ## par
# endif
#
# define PP_EXPAND_I(x) x
#
# endif
