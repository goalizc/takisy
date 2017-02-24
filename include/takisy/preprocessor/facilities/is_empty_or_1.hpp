# ifndef PREPROCESSOR_FACILITIES_IS_EMPTY_OR_1_HPP
# define PREPROCESSOR_FACILITIES_IS_EMPTY_OR_1_HPP
#
# include <takisy/preprocessor/control/iif.hpp>
# include <takisy/preprocessor/facilities/empty.hpp>
# include <takisy/preprocessor/facilities/identity.hpp>
# include <takisy/preprocessor/facilities/is_1.hpp>
# include <takisy/preprocessor/facilities/is_empty.hpp>
#
# /* PP_IS_EMPTY_OR_1 */
#
# define PP_IS_EMPTY_OR_1(x) \
    PP_IIF( \
        PP_IS_EMPTY(x PP_EMPTY()), \
        PP_IDENTITY(1), \
        PP_IS_1 \
    )(x) \
    /**/
#
# endif
