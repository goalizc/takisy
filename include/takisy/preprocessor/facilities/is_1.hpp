# ifndef PREPROCESSOR_FACILITIES_IS_1_HPP
# define PREPROCESSOR_FACILITIES_IS_1_HPP
#
# include <takisy/preprocessor/cat.hpp>
# include <takisy/preprocessor/facilities/is_empty.hpp>
#
# /* PP_IS_1 */
#
# define PP_IS_1(x) PP_IS_EMPTY(PP_CAT(PP_IS_1_HELPER_, x))
# define PP_IS_1_HELPER_1
#
# endif
