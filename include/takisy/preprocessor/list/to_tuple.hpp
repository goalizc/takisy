# ifndef PREPROCESSOR_LIST_TO_TUPLE_HPP
# define PREPROCESSOR_LIST_TO_TUPLE_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/list/enum.hpp>
# include <takisy/preprocessor/control/iif.hpp>
#
# /* PP_LIST_TO_TUPLE */
#
# define PP_LIST_TO_TUPLE(list) \
	PP_IIF \
		( \
		PP_LIST_IS_NIL(list), \
		PP_LIST_TO_TUPLE_EMPTY, \
		PP_LIST_TO_TUPLE_DO \
		) \
	(list) \
/**/
# define PP_LIST_TO_TUPLE_EMPTY(list)
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_TO_TUPLE_DO(list) (PP_LIST_ENUM(list))
# else
#    define PP_LIST_TO_TUPLE_DO(list) PP_LIST_TO_TUPLE_I(list)
#    define PP_LIST_TO_TUPLE_I(list) (PP_LIST_ENUM(list))
# endif
#
# /* PP_LIST_TO_TUPLE_R */
#
# define PP_LIST_TO_TUPLE_R(r, list) \
	PP_IIF \
		( \
		PP_LIST_IS_NIL(list), \
		PP_LIST_TO_TUPLE_R_EMPTY, \
		PP_LIST_TO_TUPLE_R_DO \
		) \
	(r, list) \
/**/
# define PP_LIST_TO_TUPLE_R_EMPTY(r,list)
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_TO_TUPLE_R_DO(r, list) (PP_LIST_ENUM_R(r, list))
# else
#    define PP_LIST_TO_TUPLE_R_DO(r, list) PP_LIST_TO_TUPLE_R_I(r, list)
#    define PP_LIST_TO_TUPLE_R_I(r, list) (PP_LIST_ENUM_R(r, list))
# endif
#
# endif
