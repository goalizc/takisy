# ifndef PREPROCESSOR_LIST_TRANSFORM_HPP
# define PREPROCESSOR_LIST_TRANSFORM_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/list/fold_right.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
# include <takisy/preprocessor/tuple/rem.hpp>
#
# /* PP_LIST_TRANSFORM */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_TRANSFORM(op, data, list) PP_TUPLE_ELEM(3, 2, PP_LIST_FOLD_RIGHT(PP_LIST_TRANSFORM_O, (op, data, PP_NIL), list))
# else
#    define PP_LIST_TRANSFORM(op, data, list) PP_LIST_TRANSFORM_I(op, data, list)
#    define PP_LIST_TRANSFORM_I(op, data, list) PP_TUPLE_ELEM(3, 2, PP_LIST_FOLD_RIGHT(PP_LIST_TRANSFORM_O, (op, data, PP_NIL), list))
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_TRANSFORM_O(d, odr, elem) PP_LIST_TRANSFORM_O_D(d, PP_TUPLE_ELEM(3, 0, odr), PP_TUPLE_ELEM(3, 1, odr), PP_TUPLE_ELEM(3, 2, odr), elem)
# else
#    define PP_LIST_TRANSFORM_O(d, odr, elem) PP_LIST_TRANSFORM_O_I(d, PP_TUPLE_REM_3 odr, elem)
#    define PP_LIST_TRANSFORM_O_I(d, im, elem) PP_LIST_TRANSFORM_O_D(d, im, elem)
# endif
#
# define PP_LIST_TRANSFORM_O_D(d, op, data, res, elem) (op, data, (op(d, data, elem), res))
#
# /* PP_LIST_TRANSFORM_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_TRANSFORM_D(d, op, data, list) PP_TUPLE_ELEM(3, 2, PP_LIST_FOLD_RIGHT_ ## d(PP_LIST_TRANSFORM_O, (op, data, PP_NIL), list))
# else
#    define PP_LIST_TRANSFORM_D(d, op, data, list) PP_LIST_TRANSFORM_D_I(d, op, data, list)
#    define PP_LIST_TRANSFORM_D_I(d, op, data, list) PP_TUPLE_ELEM(3, 2, PP_LIST_FOLD_RIGHT_ ## d(PP_LIST_TRANSFORM_O, (op, data, PP_NIL), list))
# endif
#
# endif
