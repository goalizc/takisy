# ifndef PREPROCESSOR_ARRAY_TO_LIST_HPP
# define PREPROCESSOR_ARRAY_TO_LIST_HPP
#
# include <takisy/preprocessor/cat.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/array/size.hpp>
# include <takisy/preprocessor/control/if.hpp>
# include <takisy/preprocessor/tuple/to_list.hpp>
#
# /* PP_ARRAY_TO_LIST */
#
#    define PP_ARRAY_TO_LIST(array) \
		PP_IF \
			( \
			PP_ARRAY_SIZE(array), \
			PP_ARRAY_TO_LIST_DO, \
			PP_ARRAY_TO_LIST_EMPTY \
			) \
		(array) \
/**/
#
#    define PP_ARRAY_TO_LIST_EMPTY(array) PP_NIL
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_MSVC()
#    define PP_ARRAY_TO_LIST_DO(array) PP_ARRAY_TO_LIST_I(PP_TUPLE_TO_LIST, array)
#    define PP_ARRAY_TO_LIST_I(m, args) PP_ARRAY_TO_LIST_II(m, args)
#    define PP_ARRAY_TO_LIST_II(m, args) PP_CAT(m ## args,)
# elif PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_ARRAY_TO_LIST_DO(array) PP_ARRAY_TO_LIST_I(array)
#    define PP_ARRAY_TO_LIST_I(array) PP_TUPLE_TO_LIST ## array
# else
#    define PP_ARRAY_TO_LIST_DO(array) PP_TUPLE_TO_LIST array
# endif
#
# endif
