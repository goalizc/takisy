# ifndef PREPROCESSOR_FACILITIES_IS_EMPTY_VARIADIC_HPP
# define PREPROCESSOR_FACILITIES_IS_EMPTY_VARIADIC_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# if PP_VARIADICS
#
# include <takisy/preprocessor/punctuation/is_begin_parens.hpp>
# include <takisy/preprocessor/facilities/detail/is_empty.hpp>
#
#if PP_VARIADICS_MSVC && _MSC_VER <= 1400
#
#define PP_IS_EMPTY(param) \
    PP_DETAIL_IS_EMPTY_IIF \
      ( \
      PP_IS_BEGIN_PARENS \
        ( \
        param \
        ) \
      ) \
      ( \
      PP_IS_EMPTY_ZERO, \
      PP_DETAIL_IS_EMPTY_PROCESS \
      ) \
    (param) \
/**/
#define PP_IS_EMPTY_ZERO(param) 0
# else
#define PP_IS_EMPTY(...) \
    PP_DETAIL_IS_EMPTY_IIF \
      ( \
      PP_IS_BEGIN_PARENS \
        ( \
        __VA_ARGS__ \
        ) \
      ) \
      ( \
      PP_IS_EMPTY_ZERO, \
      PP_DETAIL_IS_EMPTY_PROCESS \
      ) \
    (__VA_ARGS__) \
/**/
#define PP_IS_EMPTY_ZERO(...) 0
# endif /* PP_VARIADICS_MSVC && _MSC_VER <= 1400 */
# endif /* PP_VARIADICS */
# endif /* PREPROCESSOR_FACILITIES_IS_EMPTY_VARIADIC_HPP */
