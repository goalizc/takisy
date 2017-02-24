#ifndef PREPROCESSOR_DETAIL_IS_EMPTY_HPP
#define PREPROCESSOR_DETAIL_IS_EMPTY_HPP

#include <takisy/preprocessor/punctuation/is_begin_parens.hpp>

#if PP_VARIADICS_MSVC

# pragma warning(once:4002)

#define PP_DETAIL_IS_EMPTY_IIF_0(t, b) b
#define PP_DETAIL_IS_EMPTY_IIF_1(t, b) t

#else

#define PP_DETAIL_IS_EMPTY_IIF_0(t, ...) __VA_ARGS__
#define PP_DETAIL_IS_EMPTY_IIF_1(t, ...) t

#endif

#if PP_VARIADICS_MSVC && _MSC_VER <= 1400

#define PP_DETAIL_IS_EMPTY_PROCESS(param) \
	PP_IS_BEGIN_PARENS \
    	( \
        PP_DETAIL_IS_EMPTY_NON_FUNCTION_C param () \
        ) \
/**/

#else

#define PP_DETAIL_IS_EMPTY_PROCESS(...) \
	PP_IS_BEGIN_PARENS \
        ( \
        PP_DETAIL_IS_EMPTY_NON_FUNCTION_C __VA_ARGS__ () \
        ) \
/**/

#endif

#define PP_DETAIL_IS_EMPTY_PRIMITIVE_CAT(a, b) a ## b
#define PP_DETAIL_IS_EMPTY_IIF(bit) PP_DETAIL_IS_EMPTY_PRIMITIVE_CAT(PP_DETAIL_IS_EMPTY_IIF_,bit)
#define PP_DETAIL_IS_EMPTY_NON_FUNCTION_C(...) ()

#endif /* PREPROCESSOR_DETAIL_IS_EMPTY_HPP */
