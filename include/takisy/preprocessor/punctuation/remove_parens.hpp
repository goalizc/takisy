#ifndef PREPROCESSOR_REMOVE_PARENS_HPP
#define PREPROCESSOR_REMOVE_PARENS_HPP

#include <takisy/preprocessor/config/config.hpp>

#if PP_VARIADICS

#include <takisy/preprocessor/control/iif.hpp>
#include <takisy/preprocessor/facilities/identity.hpp>
#include <takisy/preprocessor/punctuation/is_begin_parens.hpp>
#include <takisy/preprocessor/tuple/enum.hpp>

#define PP_REMOVE_PARENS(param) \
    PP_IIF \
      ( \
      PP_IS_BEGIN_PARENS(param), \
      PP_REMOVE_PARENS_DO, \
      PP_IDENTITY \
      ) \
    (param)() \
/**/

#define PP_REMOVE_PARENS_DO(param) \
  PP_IDENTITY(PP_TUPLE_ENUM(param)) \
/**/

#endif /* PP_VARIADICS */
#endif /* PREPROCESSOR_REMOVE_PARENS_HPP */
