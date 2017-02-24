# ifndef PREPROCESSOR_TUPLE_REMOVE_HPP
# define PREPROCESSOR_TUPLE_REMOVE_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# if PP_VARIADICS
#
# include <takisy/preprocessor/array/remove.hpp>
# include <takisy/preprocessor/array/to_tuple.hpp>
# include <takisy/preprocessor/comparison/greater.hpp>
# include <takisy/preprocessor/control/iif.hpp>
# include <takisy/preprocessor/tuple/size.hpp>
# include <takisy/preprocessor/tuple/to_array.hpp>
#
# /* PP_TUPLE_REMOVE */
#
# define PP_TUPLE_REMOVE(tuple, i) \
	PP_IIF \
		( \
		PP_GREATER(PP_TUPLE_SIZE(tuple),1), \
		PP_TUPLE_REMOVE_EXEC, \
		PP_TUPLE_REMOVE_RETURN \
		) \
	(tuple, i) \
/**/
#
# define PP_TUPLE_REMOVE_EXEC(tuple, i) \
	PP_ARRAY_TO_TUPLE(PP_ARRAY_REMOVE(PP_TUPLE_TO_ARRAY(tuple), i)) \
/**/
#
# define PP_TUPLE_REMOVE_RETURN(tuple, i) tuple
#
# /* PP_TUPLE_REMOVE_D */
#
# define PP_TUPLE_REMOVE_D(d, tuple, i) \
	PP_IIF \
		( \
		PP_GREATER_D(d, PP_TUPLE_SIZE(tuple), 1), \
		PP_TUPLE_REMOVE_D_EXEC, \
		PP_TUPLE_REMOVE_D_RETURN \
		) \
	(d, tuple, i) \
/**/
#
# define PP_TUPLE_REMOVE_D_EXEC(d, tuple, i) \
	PP_ARRAY_TO_TUPLE(PP_ARRAY_REMOVE_D(d, PP_TUPLE_TO_ARRAY(tuple), i)) \
/**/
#
# define PP_TUPLE_REMOVE_D_RETURN(d, tuple, i) tuple
#
# endif // PP_VARIADICS
#
# endif // PREPROCESSOR_TUPLE_REMOVE_HPP
