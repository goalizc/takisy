# ifndef PREPROCESSOR_TUPLE_PUSH_BACK_HPP
# define PREPROCESSOR_TUPLE_PUSH_BACK_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# if PP_VARIADICS
#
# include <takisy/preprocessor/array/push_back.hpp>
# include <takisy/preprocessor/array/to_tuple.hpp>
# include <takisy/preprocessor/tuple/to_array.hpp>
#
# /* PP_TUPLE_PUSH_BACK */
#
# define PP_TUPLE_PUSH_BACK(tuple, elem) \
	PP_ARRAY_TO_TUPLE(PP_ARRAY_PUSH_BACK(PP_TUPLE_TO_ARRAY(tuple), elem)) \
/**/
#
# endif // PP_VARIADICS
#
# endif // PREPROCESSOR_TUPLE_PUSH_BACK_HPP
