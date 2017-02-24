# ifndef PREPROCESSOR_PUNCTUATION_PAREN_IF_HPP
# define PREPROCESSOR_PUNCTUATION_PAREN_IF_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/if.hpp>
# include <takisy/preprocessor/facilities/empty.hpp>
# include <takisy/preprocessor/punctuation/paren.hpp>
#
# /* PP_LPAREN_IF */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LPAREN_IF(cond) PP_IF(cond, PP_LPAREN, PP_EMPTY)()
# else
#    define PP_LPAREN_IF(cond) PP_LPAREN_IF_I(cond)
#    define PP_LPAREN_IF_I(cond) PP_IF(cond, PP_LPAREN, PP_EMPTY)()
# endif
#
# /* PP_RPAREN_IF */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_RPAREN_IF(cond) PP_IF(cond, PP_RPAREN, PP_EMPTY)()
# else
#    define PP_RPAREN_IF(cond) PP_RPAREN_IF_I(cond)
#    define PP_RPAREN_IF_I(cond) PP_IF(cond, PP_RPAREN, PP_EMPTY)()
# endif
#
# endif
