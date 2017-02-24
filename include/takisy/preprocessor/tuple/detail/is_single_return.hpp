# ifndef PREPROCESSOR_TUPLE_DETAIL_IS_SINGLE_RETURN_HPP
# define PREPROCESSOR_TUPLE_DETAIL_IS_SINGLE_RETURN_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# /* PP_TUPLE_IS_SINGLE_RETURN */
#
# if PP_VARIADICS && PP_VARIADICS_MSVC
# include <takisy/preprocessor/control/iif.hpp>
# include <takisy/preprocessor/facilities/is_1.hpp>
# include <takisy/preprocessor/tuple/size.hpp>
# define PP_TUPLE_IS_SINGLE_RETURN(sr,nsr,tuple)	\
	PP_IIF(PP_IS_1(PP_TUPLE_SIZE(tuple)),sr,nsr) \
	/**/
# endif /* PP_VARIADICS && PP_VARIADICS_MSVC */
#
# endif /* PREPROCESSOR_TUPLE_DETAIL_IS_SINGLE_RETURN_HPP */
