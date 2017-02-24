# ifndef PREPROCESSOR_ARRAY_REPLACE_HPP
# define PREPROCESSOR_ARRAY_REPLACE_HPP
#
# include <takisy/preprocessor/arithmetic/inc.hpp>
# include <takisy/preprocessor/array/elem.hpp>
# include <takisy/preprocessor/array/push_back.hpp>
# include <takisy/preprocessor/comparison/not_equal.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/deduce_d.hpp>
# include <takisy/preprocessor/control/iif.hpp>
# include <takisy/preprocessor/control/while.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
#
# /* PP_ARRAY_REPLACE */
#
# define PP_ARRAY_REPLACE(array, i, elem) PP_ARRAY_REPLACE_I(PP_DEDUCE_D(), array, i, elem)
# define PP_ARRAY_REPLACE_I(d, array, i, elem) PP_ARRAY_REPLACE_D(d, array, i, elem)
#
# /* PP_ARRAY_REPLACE_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_ARRAY_REPLACE_D(d, array, i, elem) PP_TUPLE_ELEM(5, 3, PP_WHILE_ ## d(PP_ARRAY_REPLACE_P, PP_ARRAY_REPLACE_O, (0, i, elem, (0, ()), array)))
# else
#    define PP_ARRAY_REPLACE_D(d, array, i, elem) PP_ARRAY_REPLACE_D_I(d, array, i, elem)
#    define PP_ARRAY_REPLACE_D_I(d, array, i, elem) PP_TUPLE_ELEM(5, 3, PP_WHILE_ ## d(PP_ARRAY_REPLACE_P, PP_ARRAY_REPLACE_O, (0, i, elem, (0, ()), array)))
# endif
#
# define PP_ARRAY_REPLACE_P(d, state) PP_NOT_EQUAL(PP_TUPLE_ELEM(5, 0, state), PP_ARRAY_SIZE(PP_TUPLE_ELEM(5, 4, state)))
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_STRICT()
#    define PP_ARRAY_REPLACE_O(d, state) PP_ARRAY_REPLACE_O_I state
# else
#    define PP_ARRAY_REPLACE_O(d, state) PP_ARRAY_REPLACE_O_I(PP_TUPLE_ELEM(5, 0, state), PP_TUPLE_ELEM(5, 1, state), PP_TUPLE_ELEM(5, 2, state), PP_TUPLE_ELEM(5, 3, state), PP_TUPLE_ELEM(5, 4, state))
# endif
#
# define PP_ARRAY_REPLACE_O_I(n, i, elem, res, arr) (PP_INC(n), i, elem, PP_ARRAY_PUSH_BACK(res, PP_IIF(PP_NOT_EQUAL(n, i), PP_ARRAY_ELEM(n, arr), elem)), arr)
#
# endif
