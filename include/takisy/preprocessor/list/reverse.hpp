# ifndef PREPROCESSOR_LIST_REVERSE_HPP
# define PREPROCESSOR_LIST_REVERSE_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/list/fold_left.hpp>
#
# /* PP_LIST_REVERSE */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_REVERSE(list) PP_LIST_FOLD_LEFT(PP_LIST_REVERSE_O, PP_NIL, list)
# else
#    define PP_LIST_REVERSE(list) PP_LIST_REVERSE_I(list)
#    define PP_LIST_REVERSE_I(list) PP_LIST_FOLD_LEFT(PP_LIST_REVERSE_O, PP_NIL, list)
# endif
#
# define PP_LIST_REVERSE_O(d, s, x) (x, s)
#
# /* PP_LIST_REVERSE_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_REVERSE_D(d, list) PP_LIST_FOLD_LEFT_ ## d(PP_LIST_REVERSE_O, PP_NIL, list)
# else
#    define PP_LIST_REVERSE_D(d, list) PP_LIST_REVERSE_D_I(d, list)
#    define PP_LIST_REVERSE_D_I(d, list) PP_LIST_FOLD_LEFT_ ## d(PP_LIST_REVERSE_O, PP_NIL, list)
# endif
#
# endif
