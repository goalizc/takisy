# ifndef PREPROCESSOR_SEQ_TRANSFORM_HPP
# define PREPROCESSOR_SEQ_TRANSFORM_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/seq/fold_left.hpp>
# include <takisy/preprocessor/seq/seq.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
# include <takisy/preprocessor/tuple/rem.hpp>
#
# /* PP_SEQ_TRANSFORM */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SEQ_TRANSFORM(op, data, seq) PP_SEQ_TAIL(PP_TUPLE_ELEM(3, 2, PP_SEQ_FOLD_LEFT(PP_SEQ_TRANSFORM_O, (op, data, (nil)), seq)))
# else
#    define PP_SEQ_TRANSFORM(op, data, seq) PP_SEQ_TRANSFORM_I(op, data, seq)
#    define PP_SEQ_TRANSFORM_I(op, data, seq) PP_SEQ_TAIL(PP_TUPLE_ELEM(3, 2, PP_SEQ_FOLD_LEFT(PP_SEQ_TRANSFORM_O, (op, data, (nil)), seq)))
# endif
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_STRICT()
#    define PP_SEQ_TRANSFORM_O(s, state, elem) PP_SEQ_TRANSFORM_O_IM(s, PP_TUPLE_REM_3 state, elem)
#    define PP_SEQ_TRANSFORM_O_IM(s, im, elem) PP_SEQ_TRANSFORM_O_I(s, im, elem)
# else
#    define PP_SEQ_TRANSFORM_O(s, state, elem) PP_SEQ_TRANSFORM_O_I(s, PP_TUPLE_ELEM(3, 0, state), PP_TUPLE_ELEM(3, 1, state), PP_TUPLE_ELEM(3, 2, state), elem)
# endif
#
# define PP_SEQ_TRANSFORM_O_I(s, op, data, res, elem) (op, data, res (op(s, data, elem)))
#
# /* PP_SEQ_TRANSFORM_S */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SEQ_TRANSFORM_S(s, op, data, seq) PP_SEQ_TAIL(PP_TUPLE_ELEM(3, 2, PP_SEQ_FOLD_LEFT_ ## s(PP_SEQ_TRANSFORM_O, (op, data, (nil)), seq)))
# else
#    define PP_SEQ_TRANSFORM_S(s, op, data, seq) PP_SEQ_TRANSFORM_S_I(s, op, data, seq)
#    define PP_SEQ_TRANSFORM_S_I(s, op, data, seq) PP_SEQ_TAIL(PP_TUPLE_ELEM(3, 2, PP_SEQ_FOLD_LEFT_ ## s(PP_SEQ_TRANSFORM_O, (op, data, (nil)), seq)))
# endif
#
# endif
