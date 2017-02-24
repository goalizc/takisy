# ifndef PREPROCESSOR_LIST_APPEND_HPP
# define PREPROCESSOR_LIST_APPEND_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/list/fold_right.hpp>
#
# /* PP_LIST_APPEND */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_APPEND(a, b) PP_LIST_FOLD_RIGHT(PP_LIST_APPEND_O, b, a)
# else
#    define PP_LIST_APPEND(a, b) PP_LIST_APPEND_I(a, b)
#    define PP_LIST_APPEND_I(a, b) PP_LIST_FOLD_RIGHT(PP_LIST_APPEND_O, b, a)
# endif
#
# define PP_LIST_APPEND_O(d, s, x) (x, s)
#
# /* PP_LIST_APPEND_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_APPEND_D(d, a, b) PP_LIST_FOLD_RIGHT_ ## d(PP_LIST_APPEND_O, b, a)
# else
#    define PP_LIST_APPEND_D(d, a, b) PP_LIST_APPEND_D_I(d, a, b)
#    define PP_LIST_APPEND_D_I(d, a, b) PP_LIST_FOLD_RIGHT_ ## d(PP_LIST_APPEND_O, b, a)
# endif
#
# endif
