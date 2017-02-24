# ifndef PREPROCESSOR_ARRAY_TO_TUPLE_HPP
# define PREPROCESSOR_ARRAY_TO_TUPLE_HPP
#
# include <takisy/preprocessor/array/data.hpp>
# include <takisy/preprocessor/array/size.hpp>
# include <takisy/preprocessor/control/if.hpp>
#
# /* PP_ARRAY_TO_TUPLE */
#
#    define PP_ARRAY_TO_TUPLE(array) \
		PP_IF \
			( \
			PP_ARRAY_SIZE(array), \
			PP_ARRAY_DATA, \
			PP_ARRAY_TO_TUPLE_EMPTY \
			) \
		(array) \
/**/
#    define PP_ARRAY_TO_TUPLE_EMPTY(array)
#
# endif
