# ifndef PREPROCESSOR_SEQ_DETAIL_BINARY_TRANSFORM_HPP
# define PREPROCESSOR_SEQ_DETAIL_BINARY_TRANSFORM_HPP
#
# include <takisy/preprocessor/cat.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/tuple/eat.hpp>
# include <takisy/preprocessor/tuple/rem.hpp>
# include <takisy/preprocessor/variadic/detail/is_single_return.hpp>
#
# /* PP_SEQ_BINARY_TRANSFORM */
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_MSVC()
#    define PP_SEQ_BINARY_TRANSFORM(seq) PP_SEQ_BINARY_TRANSFORM_I(, seq)
#    define PP_SEQ_BINARY_TRANSFORM_I(p, seq) PP_SEQ_BINARY_TRANSFORM_II(p ## seq)
#    define PP_SEQ_BINARY_TRANSFORM_II(seq) PP_SEQ_BINARY_TRANSFORM_III(seq)
#    define PP_SEQ_BINARY_TRANSFORM_III(seq) PP_CAT(PP_SEQ_BINARY_TRANSFORM_A seq, 0)
# else
#    define PP_SEQ_BINARY_TRANSFORM(seq) PP_CAT(PP_SEQ_BINARY_TRANSFORM_A seq, 0)
# endif
# if PP_VARIADICS
#    if PP_VARIADICS_MSVC
#		define PP_SEQ_BINARY_TRANSFORM_GET_REM(...) \
			PP_VARIADIC_IS_SINGLE_RETURN(PP_REM_CAT,PP_REM,__VA_ARGS__) \
		/**/
#	 else
#		define PP_SEQ_BINARY_TRANSFORM_GET_REM(...) PP_REM
#	 endif
#    define PP_SEQ_BINARY_TRANSFORM_A(...) (PP_SEQ_BINARY_TRANSFORM_GET_REM(__VA_ARGS__), __VA_ARGS__)() PP_SEQ_BINARY_TRANSFORM_B
#    define PP_SEQ_BINARY_TRANSFORM_B(...) (PP_SEQ_BINARY_TRANSFORM_GET_REM(__VA_ARGS__), __VA_ARGS__)() PP_SEQ_BINARY_TRANSFORM_A
# else
#    define PP_SEQ_BINARY_TRANSFORM_A(e) (PP_REM, e)() PP_SEQ_BINARY_TRANSFORM_B
#    define PP_SEQ_BINARY_TRANSFORM_B(e) (PP_REM, e)() PP_SEQ_BINARY_TRANSFORM_A
# endif
# define PP_SEQ_BINARY_TRANSFORM_A0 (PP_EAT, ?)
# define PP_SEQ_BINARY_TRANSFORM_B0 (PP_EAT, ?)
#
# endif
