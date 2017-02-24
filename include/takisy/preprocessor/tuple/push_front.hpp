# ifndef PREPROCESSOR_TUPLE_PUSH_FRONT_HPP
# define PREPROCESSOR_TUPLE_PUSH_FRONT_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# if PP_VARIADICS
#
# include <takisy/preprocessor/array/push_front.hpp>
# include <takisy/preprocessor/array/to_tuple.hpp>
# include <takisy/preprocessor/tuple/to_array.hpp>
#
#
# /* PP_TUPLE_PUSH_FRONT */
#
# define PP_TUPLE_PUSH_FRONT(tuple, elem) \
	PP_ARRAY_TO_TUPLE(PP_ARRAY_PUSH_FRONT(PP_TUPLE_TO_ARRAY(tuple), elem)) \
/**/
#
# endif // PP_VARIADICS
#
# endif // PREPROCESSOR_TUPLE_PUSH_FRONT_HPP
