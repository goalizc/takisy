# ifndef PREPROCESSOR_LIST_FILTER_HPP
# define PREPROCESSOR_LIST_FILTER_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/if.hpp>
# include <takisy/preprocessor/list/fold_right.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
# include <takisy/preprocessor/tuple/rem.hpp>
#
# /* PP_LIST_FILTER */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_FILTER(pred, data, list) PP_TUPLE_ELEM(3, 2, PP_LIST_FOLD_RIGHT(PP_LIST_FILTER_O, (pred, data, PP_NIL), list))
# else
#    define PP_LIST_FILTER(pred, data, list) PP_LIST_FILTER_I(pred, data, list)
#    define PP_LIST_FILTER_I(pred, data, list) PP_TUPLE_ELEM(3, 2, PP_LIST_FOLD_RIGHT(PP_LIST_FILTER_O, (pred, data, PP_NIL), list))
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_FILTER_O(d, pdr, elem) PP_LIST_FILTER_O_D(d, PP_TUPLE_ELEM(3, 0, pdr), PP_TUPLE_ELEM(3, 1, pdr), PP_TUPLE_ELEM(3, 2, pdr), elem)
# else
#    define PP_LIST_FILTER_O(d, pdr, elem) PP_LIST_FILTER_O_I(d, PP_TUPLE_REM_3 pdr, elem)
#    define PP_LIST_FILTER_O_I(d, im, elem) PP_LIST_FILTER_O_D(d, im, elem)
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_DMC()
#    define PP_LIST_FILTER_O_D(d, pred, data, res, elem) (pred, data, PP_IF(pred(d, data, elem), (elem, res), res))
# else
#    define PP_LIST_FILTER_O_D(d, pred, data, res, elem) (pred, data, PP_IF(pred##(d, data, elem), (elem, res), res))
# endif
#
# /* PP_LIST_FILTER_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_FILTER_D(d, pred, data, list) PP_TUPLE_ELEM(3, 2, PP_LIST_FOLD_RIGHT_ ## d(PP_LIST_FILTER_O, (pred, data, PP_NIL), list))
# else
#    define PP_LIST_FILTER_D(d, pred, data, list) PP_LIST_FILTER_D_I(d, pred, data, list)
#    define PP_LIST_FILTER_D_I(d, pred, data, list) PP_TUPLE_ELEM(3, 2, PP_LIST_FOLD_RIGHT_ ## d(PP_LIST_FILTER_O, (pred, data, PP_NIL), list))
# endif
#
# endif
