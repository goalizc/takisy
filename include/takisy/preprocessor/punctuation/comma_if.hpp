# ifndef PREPROCESSOR_PUNCTUATION_COMMA_IF_HPP
# define PREPROCESSOR_PUNCTUATION_COMMA_IF_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/if.hpp>
# include <takisy/preprocessor/facilities/empty.hpp>
# include <takisy/preprocessor/punctuation/comma.hpp>
#
# /* PP_COMMA_IF */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_COMMA_IF(cond) PP_IF(cond, PP_COMMA, PP_EMPTY)()
# else
#    define PP_COMMA_IF(cond) PP_COMMA_IF_I(cond)
#    define PP_COMMA_IF_I(cond) PP_IF(cond, PP_COMMA, PP_EMPTY)()
# endif
#
# endif
