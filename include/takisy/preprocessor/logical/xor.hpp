# ifndef PREPROCESSOR_LOGICAL_XOR_HPP
# define PREPROCESSOR_LOGICAL_XOR_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/logical/bool.hpp>
# include <takisy/preprocessor/logical/bitxor.hpp>
#
# /* PP_XOR */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_XOR(p, q) PP_BITXOR(PP_BOOL(p), PP_BOOL(q))
# else
#    define PP_XOR(p, q) PP_XOR_I(p, q)
#    define PP_XOR_I(p, q) PP_BITXOR(PP_BOOL(p), PP_BOOL(q))
# endif
#
# endif
