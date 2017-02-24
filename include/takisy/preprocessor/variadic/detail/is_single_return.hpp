# ifndef PREPROCESSOR_VARIADIC_DETAIL_IS_SINGLE_RETURN_HPP
# define PREPROCESSOR_VARIADIC_DETAIL_IS_SINGLE_RETURN_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# /* PP_VARIADIC_IS_SINGLE_RETURN */
#
# if PP_VARIADICS && PP_VARIADICS_MSVC
# include <takisy/preprocessor/control/iif.hpp>
# include <takisy/preprocessor/facilities/is_1.hpp>
# include <takisy/preprocessor/variadic/size.hpp>
# define PP_VARIADIC_IS_SINGLE_RETURN(sr,nsr,...)	\
	PP_IIF(PP_IS_1(PP_VARIADIC_SIZE(__VA_ARGS__)),sr,nsr) \
	/**/
# endif /* PP_VARIADICS && PP_VARIADICS_MSVC */
#
# endif /* PREPROCESSOR_VARIADIC_DETAIL_IS_SINGLE_RETURN_HPP */
