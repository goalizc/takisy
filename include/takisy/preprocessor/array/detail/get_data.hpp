# ifndef PREPROCESSOR_ARRAY_DETAIL_GET_DATA_HPP
# define PREPROCESSOR_ARRAY_DETAIL_GET_DATA_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/tuple/rem.hpp>
# include <takisy/preprocessor/control/if.hpp>
# include <takisy/preprocessor/control/iif.hpp>
# include <takisy/preprocessor/facilities/is_1.hpp>
#
# /* PP_ARRAY_DETAIL_GET_DATA */
#
# define PP_ARRAY_DETAIL_GET_DATA_NONE(size, data)

# if PP_VARIADICS && !(PP_VARIADICS_MSVC && _MSC_VER <= 1400)
# 	 if PP_VARIADICS_MSVC
# 		define PP_ARRAY_DETAIL_GET_DATA_ANY_VC_DEFAULT(size, data) PP_TUPLE_REM(size) data
# 		define PP_ARRAY_DETAIL_GET_DATA_ANY_VC_CAT(size, data) PP_TUPLE_REM_CAT(size) data
# 		define PP_ARRAY_DETAIL_GET_DATA_ANY(size, data) \
			PP_IIF \
				( \
				PP_IS_1(size), \
				PP_ARRAY_DETAIL_GET_DATA_ANY_VC_CAT, \
				PP_ARRAY_DETAIL_GET_DATA_ANY_VC_DEFAULT \
				) \
			(size,data) \
/**/
#    else
# 		define PP_ARRAY_DETAIL_GET_DATA_ANY(size, data) PP_TUPLE_REM(size) data
#    endif
# else
# 	 define PP_ARRAY_DETAIL_GET_DATA_ANY(size, data) PP_TUPLE_REM(size) data
# endif

# define PP_ARRAY_DETAIL_GET_DATA(size, data) \
	PP_IF \
		( \
		size, \
		PP_ARRAY_DETAIL_GET_DATA_ANY, \
		PP_ARRAY_DETAIL_GET_DATA_NONE \
		) \
	(size,data) \
/**/
#
# endif /* PREPROCESSOR_ARRAY_DETAIL_GET_DATA_HPP */
