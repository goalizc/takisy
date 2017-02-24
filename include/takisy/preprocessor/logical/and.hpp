# ifndef PREPROCESSOR_LOGICAL_AND_HPP
# define PREPROCESSOR_LOGICAL_AND_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/logical/bool.hpp>
# include <takisy/preprocessor/logical/bitand.hpp>
#
# /* PP_AND */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_AND(p, q) PP_BITAND(PP_BOOL(p), PP_BOOL(q))
# else
#    define PP_AND(p, q) PP_AND_I(p, q)
#    define PP_AND_I(p, q) PP_BITAND(PP_BOOL(p), PP_BOOL(q))
# endif
#
# endif
