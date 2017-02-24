# ifndef PREPROCESSOR_IS_BEGIN_PARENS_HPP
# define PREPROCESSOR_IS_BEGIN_PARENS_HPP

# include <takisy/preprocessor/config/config.hpp>

#if PP_VARIADICS

#include <takisy/preprocessor/punctuation/detail/is_begin_parens.hpp>

#if PP_VARIADICS_MSVC && _MSC_VER <= 1400

#define PP_IS_BEGIN_PARENS(param) \
    PP_DETAIL_IBP_SPLIT \
      ( \
      0, \
      PP_DETAIL_IBP_CAT \
        ( \
        PP_DETAIL_IBP_IS_VARIADIC_R_, \
        PP_DETAIL_IBP_IS_VARIADIC_C param \
        ) \
      ) \
/**/

#else

#define PP_IS_BEGIN_PARENS(...) \
    PP_DETAIL_IBP_SPLIT \
      ( \
      0, \
      PP_DETAIL_IBP_CAT \
        ( \
        PP_DETAIL_IBP_IS_VARIADIC_R_, \
        PP_DETAIL_IBP_IS_VARIADIC_C __VA_ARGS__ \
        ) \
      ) \
/**/

#endif /* PP_VARIADICS_MSVC && _MSC_VER <= 1400 */
#endif /* PP_VARIADICS */
#endif /* PREPROCESSOR_IS_BEGIN_PARENS_HPP */
