# ifndef PREPROCESSOR_TUPLE_REPLACE_HPP
# define PREPROCESSOR_TUPLE_REPLACE_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# if PP_VARIADICS
#
# include <takisy/preprocessor/array/replace.hpp>
# include <takisy/preprocessor/array/to_tuple.hpp>
# include <takisy/preprocessor/tuple/to_array.hpp>
#
# /* PP_TUPLE_REPLACE */
#
# define PP_TUPLE_REPLACE(tuple, i, elem) \
	PP_ARRAY_TO_TUPLE(PP_ARRAY_REPLACE(PP_TUPLE_TO_ARRAY(tuple), i, elem)) \
/**/
#
# /* PP_TUPLE_REPLACE_D */
#
# define PP_TUPLE_REPLACE_D(d, tuple, i, elem) \
	PP_ARRAY_TO_TUPLE(PP_ARRAY_REPLACE_D(d, PP_TUPLE_TO_ARRAY(tuple), i, elem)) \
/**/
#
# endif // PP_VARIADICS
#
# endif // PREPROCESSOR_TUPLE_REPLACE_HPP
