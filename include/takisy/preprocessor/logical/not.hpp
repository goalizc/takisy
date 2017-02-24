# ifndef PREPROCESSOR_LOGICAL_NOT_HPP
# define PREPROCESSOR_LOGICAL_NOT_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/logical/bool.hpp>
# include <takisy/preprocessor/logical/compl.hpp>
#
# /* PP_NOT */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_NOT(x) PP_COMPL(PP_BOOL(x))
# else
#    define PP_NOT(x) PP_NOT_I(x)
#    define PP_NOT_I(x) PP_COMPL(PP_BOOL(x))
# endif
#
# endif
