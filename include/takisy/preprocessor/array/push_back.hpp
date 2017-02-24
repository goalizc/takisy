# ifndef PREPROCESSOR_ARRAY_PUSH_BACK_HPP
# define PREPROCESSOR_ARRAY_PUSH_BACK_HPP
#
# include <takisy/preprocessor/arithmetic/inc.hpp>
# include <takisy/preprocessor/array/data.hpp>
# include <takisy/preprocessor/array/size.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/punctuation/comma_if.hpp>
# include <takisy/preprocessor/tuple/rem.hpp>
# include <takisy/preprocessor/array/detail/get_data.hpp>
#
# /* PP_ARRAY_PUSH_BACK */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_ARRAY_PUSH_BACK(array, elem) PP_ARRAY_PUSH_BACK_I(PP_ARRAY_SIZE(array), PP_ARRAY_DATA(array), elem)
# else
#    define PP_ARRAY_PUSH_BACK(array, elem) PP_ARRAY_PUSH_BACK_D(array, elem)
#    define PP_ARRAY_PUSH_BACK_D(array, elem) PP_ARRAY_PUSH_BACK_I(PP_ARRAY_SIZE(array), PP_ARRAY_DATA(array), elem)
# endif
#
# define PP_ARRAY_PUSH_BACK_I(size, data, elem) (PP_INC(size), (PP_ARRAY_DETAIL_GET_DATA(size,data) PP_COMMA_IF(size) elem))
#
# endif
