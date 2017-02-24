# ifndef PREPROCESSOR_SEQ_FILTER_HPP
# define PREPROCESSOR_SEQ_FILTER_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/expr_if.hpp>
# include <takisy/preprocessor/facilities/empty.hpp>
# include <takisy/preprocessor/seq/fold_left.hpp>
# include <takisy/preprocessor/seq/seq.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
# include <takisy/preprocessor/tuple/rem.hpp>
#
# /* PP_SEQ_FILTER */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SEQ_FILTER(pred, data, seq) PP_SEQ_TAIL(PP_TUPLE_ELEM(3, 2, PP_SEQ_FOLD_LEFT(PP_SEQ_FILTER_O, (pred, data, (nil)), seq)))
# else
#    define PP_SEQ_FILTER(pred, data, seq) PP_SEQ_FILTER_I(pred, data, seq)
#    define PP_SEQ_FILTER_I(pred, data, seq) PP_SEQ_TAIL(PP_TUPLE_ELEM(3, 2, PP_SEQ_FOLD_LEFT(PP_SEQ_FILTER_O, (pred, data, (nil)), seq)))
# endif
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_STRICT()
#    define PP_SEQ_FILTER_O(s, st, elem) PP_SEQ_FILTER_O_IM(s, PP_TUPLE_REM_3 st, elem)
#    define PP_SEQ_FILTER_O_IM(s, im, elem) PP_SEQ_FILTER_O_I(s, im, elem)
# else
#    define PP_SEQ_FILTER_O(s, st, elem) PP_SEQ_FILTER_O_I(s, PP_TUPLE_ELEM(3, 0, st), PP_TUPLE_ELEM(3, 1, st), PP_TUPLE_ELEM(3, 2, st), elem)
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_DMC()
#   define PP_SEQ_FILTER_O_I(s, pred, data, res, elem) (pred, data, res PP_EXPR_IF(pred(s, data, elem), (elem)))
# else
#   define PP_SEQ_FILTER_O_I(s, pred, data, res, elem) (pred, data, res PP_EXPR_IF(pred##(s, data, elem), (elem)))
# endif
#
# /* PP_SEQ_FILTER_S */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SEQ_FILTER_S(s, pred, data, seq) PP_SEQ_TAIL(PP_TUPLE_ELEM(3, 2, PP_SEQ_FOLD_LEFT_ ## s(PP_SEQ_FILTER_O, (pred, data, (nil)), seq)))
# else
#    define PP_SEQ_FILTER_S(s, pred, data, seq) PP_SEQ_FILTER_S_I(s, pred, data, seq)
#    define PP_SEQ_FILTER_S_I(s, pred, data, seq) PP_SEQ_TAIL(PP_TUPLE_ELEM(3, 2, PP_SEQ_FOLD_LEFT_ ## s(PP_SEQ_FILTER_O, (pred, data, (nil)), seq)))
# endif
#
# endif
