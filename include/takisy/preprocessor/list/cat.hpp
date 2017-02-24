# ifndef PREPROCESSOR_LIST_CAT_HPP
# define PREPROCESSOR_LIST_CAT_HPP
#
# include <takisy/preprocessor/cat.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/list/adt.hpp>
# include <takisy/preprocessor/list/fold_left.hpp>
#
# /* PP_LIST_CAT */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_CAT(list) PP_LIST_FOLD_LEFT(PP_LIST_CAT_O, PP_LIST_FIRST(list), PP_LIST_REST(list))
# else
#    define PP_LIST_CAT(list) PP_LIST_CAT_I(list)
#    define PP_LIST_CAT_I(list) PP_LIST_FOLD_LEFT(PP_LIST_CAT_O, PP_LIST_FIRST(list), PP_LIST_REST(list))
# endif
#
# define PP_LIST_CAT_O(d, s, x) PP_CAT(s, x)
#
# /* PP_LIST_CAT_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_CAT_D(d, list) PP_LIST_FOLD_LEFT_ ## d(PP_LIST_CAT_O, PP_LIST_FIRST(list), PP_LIST_REST(list))
# else
#    define PP_LIST_CAT_D(d, list) PP_LIST_CAT_D_I(d, list)
#    define PP_LIST_CAT_D_I(d, list) PP_LIST_FOLD_LEFT_ ## d(PP_LIST_CAT_O, PP_LIST_FIRST(list), PP_LIST_REST(list))
# endif
#
# endif
