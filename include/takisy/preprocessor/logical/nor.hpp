# ifndef PREPROCESSOR_LOGICAL_NOR_HPP
# define PREPROCESSOR_LOGICAL_NOR_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/logical/bool.hpp>
# include <takisy/preprocessor/logical/bitnor.hpp>
#
# /* PP_NOR */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_NOR(p, q) PP_BITNOR(PP_BOOL(p), PP_BOOL(q))
# else
#    define PP_NOR(p, q) PP_NOR_I(p, q)
#    define PP_NOR_I(p, q) PP_BITNOR(PP_BOOL(p), PP_BOOL(q))
# endif
#
# endif
