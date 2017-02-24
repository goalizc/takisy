# ifndef PREPROCESSOR_TUPLE_INSERT_HPP
# define PREPROCESSOR_TUPLE_INSERT_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# if PP_VARIADICS
#
# include <takisy/preprocessor/array/insert.hpp>
# include <takisy/preprocessor/array/to_tuple.hpp>
# include <takisy/preprocessor/tuple/to_array.hpp>
#
# /* PP_TUPLE_INSERT */
#
# define PP_TUPLE_INSERT(tuple, i, elem) \
	PP_ARRAY_TO_TUPLE(PP_ARRAY_INSERT(PP_TUPLE_TO_ARRAY(tuple), i, elem)) \
/**/
#
# /* PP_TUPLE_INSERT_D */
#
# define PP_TUPLE_INSERT_D(d, tuple, i, elem) \
	PP_ARRAY_TO_TUPLE(PP_ARRAY_INSERT_D(d, PP_TUPLE_TO_ARRAY(tuple), i, elem)) \
/**/
#
# endif // PP_VARIADICS
#
# endif // PREPROCESSOR_TUPLE_INSERT_HPP
