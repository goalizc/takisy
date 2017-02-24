# ifndef PREPROCESSOR_ARRAY_REMOVE_HPP
# define PREPROCESSOR_ARRAY_REMOVE_HPP
#
# include <takisy/preprocessor/arithmetic/inc.hpp>
# include <takisy/preprocessor/array/elem.hpp>
# include <takisy/preprocessor/array/push_back.hpp>
# include <takisy/preprocessor/array/size.hpp>
# include <takisy/preprocessor/comparison/not_equal.hpp>
# include <takisy/preprocessor/control/deduce_d.hpp>
# include <takisy/preprocessor/control/iif.hpp>
# include <takisy/preprocessor/control/while.hpp>
# include <takisy/preprocessor/tuple/eat.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
#
# /* PP_ARRAY_REMOVE */
#
# define PP_ARRAY_REMOVE(array, i) PP_ARRAY_REMOVE_I(PP_DEDUCE_D(), array, i)
# define PP_ARRAY_REMOVE_I(d, array, i) PP_ARRAY_REMOVE_D(d, array, i)
#
# /* PP_ARRAY_REMOVE_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_ARRAY_REMOVE_D(d, array, i) PP_TUPLE_ELEM(4, 2, PP_WHILE_ ## d(PP_ARRAY_REMOVE_P, PP_ARRAY_REMOVE_O, (0, i, (0, ()), array)))
# else
#    define PP_ARRAY_REMOVE_D(d, array, i) PP_ARRAY_REMOVE_D_I(d, array, i)
#    define PP_ARRAY_REMOVE_D_I(d, array, i) PP_TUPLE_ELEM(4, 2, PP_WHILE_ ## d(PP_ARRAY_REMOVE_P, PP_ARRAY_REMOVE_O, (0, i, (0, ()), array)))
# endif
#
# define PP_ARRAY_REMOVE_P(d, st) PP_NOT_EQUAL(PP_TUPLE_ELEM(4, 0, st), PP_ARRAY_SIZE(PP_TUPLE_ELEM(4, 3, st)))
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_STRICT()
#    define PP_ARRAY_REMOVE_O(d, st) PP_ARRAY_REMOVE_O_I st
# else
#    define PP_ARRAY_REMOVE_O(d, st) PP_ARRAY_REMOVE_O_I(PP_TUPLE_ELEM(4, 0, st), PP_TUPLE_ELEM(4, 1, st), PP_TUPLE_ELEM(4, 2, st), PP_TUPLE_ELEM(4, 3, st))
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_DMC()
#    define PP_ARRAY_REMOVE_O_I(n, i, res, arr) (PP_INC(n), i, PP_IIF(PP_NOT_EQUAL(n, i), PP_ARRAY_PUSH_BACK, res PP_TUPLE_EAT_2)(res, PP_ARRAY_ELEM(n, arr)), arr)
# else
#    define PP_ARRAY_REMOVE_O_I(n, i, res, arr) (PP_INC(n), i, PP_IIF(PP_NOT_EQUAL(n, i), PP_ARRAY_PUSH_BACK, PP_TUPLE_ELEM_2_0)(res, PP_ARRAY_ELEM(n, arr)), arr)
# endif
#
# endif
