# ifndef PREPROCESSOR_ARRAY_INSERT_HPP
# define PREPROCESSOR_ARRAY_INSERT_HPP
#
# include <takisy/preprocessor/arithmetic/inc.hpp>
# include <takisy/preprocessor/array/elem.hpp>
# include <takisy/preprocessor/array/push_back.hpp>
# include <takisy/preprocessor/array/size.hpp>
# include <takisy/preprocessor/comparison/not_equal.hpp>
# include <takisy/preprocessor/control/deduce_d.hpp>
# include <takisy/preprocessor/control/iif.hpp>
# include <takisy/preprocessor/control/while.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
#
# /* PP_ARRAY_INSERT */
#
# define PP_ARRAY_INSERT(array, i, elem) PP_ARRAY_INSERT_I(PP_DEDUCE_D(), array, i, elem)
# define PP_ARRAY_INSERT_I(d, array, i, elem) PP_ARRAY_INSERT_D(d, array, i, elem)
#
# /* PP_ARRAY_INSERT_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_ARRAY_INSERT_D(d, array, i, elem) PP_TUPLE_ELEM(5, 3, PP_WHILE_ ## d(PP_ARRAY_INSERT_P, PP_ARRAY_INSERT_O, (0, i, elem, (0, ()), array)))
# else
#    define PP_ARRAY_INSERT_D(d, array, i, elem) PP_ARRAY_INSERT_D_I(d, array, i, elem)
#    define PP_ARRAY_INSERT_D_I(d, array, i, elem) PP_TUPLE_ELEM(5, 3, PP_WHILE_ ## d(PP_ARRAY_INSERT_P, PP_ARRAY_INSERT_O, (0, i, elem, (0, ()), array)))
# endif
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_STRICT()
#    define PP_ARRAY_INSERT_P(d, state) PP_ARRAY_INSERT_P_I state
# else
#    define PP_ARRAY_INSERT_P(d, state) PP_ARRAY_INSERT_P_I(nil, nil, nil, PP_TUPLE_ELEM(5, 3, state), PP_TUPLE_ELEM(5, 4, state))
# endif
#
# define PP_ARRAY_INSERT_P_I(_i, _ii, _iii, res, arr) PP_NOT_EQUAL(PP_ARRAY_SIZE(res), PP_INC(PP_ARRAY_SIZE(arr)))
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_STRICT()
#    define PP_ARRAY_INSERT_O(d, state) PP_ARRAY_INSERT_O_I state
# else
#    define PP_ARRAY_INSERT_O(d, state) PP_ARRAY_INSERT_O_I(PP_TUPLE_ELEM(5, 0, state), PP_TUPLE_ELEM(5, 1, state), PP_TUPLE_ELEM(5, 2, state), PP_TUPLE_ELEM(5, 3, state), PP_TUPLE_ELEM(5, 4, state))
# endif
#
# define PP_ARRAY_INSERT_O_I(n, i, elem, res, arr) (PP_IIF(PP_NOT_EQUAL(PP_ARRAY_SIZE(res), i), PP_INC(n), n), i, elem, PP_ARRAY_PUSH_BACK(res, PP_IIF(PP_NOT_EQUAL(PP_ARRAY_SIZE(res), i), PP_ARRAY_ELEM(n, arr), elem)), arr)
#
# endif
