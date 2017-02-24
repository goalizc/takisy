# ifndef PREPROCESSOR_TUPLE_POP_FRONT_HPP
# define PREPROCESSOR_TUPLE_POP_FRONT_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# if PP_VARIADICS
#
# include <takisy/preprocessor/array/pop_front.hpp>
# include <takisy/preprocessor/array/to_tuple.hpp>
# include <takisy/preprocessor/comparison/greater.hpp>
# include <takisy/preprocessor/control/iif.hpp>
# include <takisy/preprocessor/tuple/size.hpp>
# include <takisy/preprocessor/tuple/to_array.hpp>
#
#
# /* PP_TUPLE_POP_FRONT */
#
# define PP_TUPLE_POP_FRONT(tuple) \
	PP_IIF \
		( \
		PP_GREATER(PP_TUPLE_SIZE(tuple),1), \
		PP_TUPLE_POP_FRONT_EXEC, \
		PP_TUPLE_POP_FRONT_RETURN \
		) \
	(tuple) \
/**/
#
# define PP_TUPLE_POP_FRONT_EXEC(tuple) \
	PP_ARRAY_TO_TUPLE(PP_ARRAY_POP_FRONT(PP_TUPLE_TO_ARRAY(tuple))) \
/**/
#
# define PP_TUPLE_POP_FRONT_RETURN(tuple) tuple
#
# /* PP_TUPLE_POP_FRONT_Z */
#
# define PP_TUPLE_POP_FRONT_Z(z, tuple) \
	PP_IIF \
		( \
		PP_GREATER(PP_TUPLE_SIZE(tuple),1), \
		PP_TUPLE_POP_FRONT_Z_EXEC, \
		PP_TUPLE_POP_FRONT_Z_RETURN \
		) \
	(z, tuple) \
/**/
#
# define PP_TUPLE_POP_FRONT_Z_EXEC(z, tuple) \
	PP_ARRAY_TO_TUPLE(PP_ARRAY_POP_FRONT_Z(z, PP_TUPLE_TO_ARRAY(tuple))) \
/**/
#
# define PP_TUPLE_POP_FRONT_Z_RETURN(z, tuple) tuple
#
# endif // PP_VARIADICS
#
# endif // PREPROCESSOR_TUPLE_POP_FRONT_HPP
