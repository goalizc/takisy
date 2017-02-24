# ifndef PREPROCESSOR_LOGICAL_OR_HPP
# define PREPROCESSOR_LOGICAL_OR_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/logical/bool.hpp>
# include <takisy/preprocessor/logical/bitor.hpp>
#
# /* PP_OR */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_OR(p, q) PP_BITOR(PP_BOOL(p), PP_BOOL(q))
# else
#    define PP_OR(p, q) PP_OR_I(p, q)
#    define PP_OR_I(p, q) PP_BITOR(PP_BOOL(p), PP_BOOL(q))
# endif
#
# endif
