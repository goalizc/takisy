# ifndef PREPROCESSOR_ARRAY_PUSH_FRONT_HPP
# define PREPROCESSOR_ARRAY_PUSH_FRONT_HPP
#
# include <takisy/preprocessor/arithmetic/inc.hpp>
# include <takisy/preprocessor/array/data.hpp>
# include <takisy/preprocessor/array/size.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/punctuation/comma_if.hpp>
# include <takisy/preprocessor/tuple/rem.hpp>
# include <takisy/preprocessor/array/detail/get_data.hpp>
#
# /* PP_ARRAY_PUSH_FRONT */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_ARRAY_PUSH_FRONT(array, elem) PP_ARRAY_PUSH_FRONT_I(PP_ARRAY_SIZE(array), PP_ARRAY_DATA(array), elem)
# else
#    define PP_ARRAY_PUSH_FRONT(array, elem) PP_ARRAY_PUSH_FRONT_D(array, elem)
#    define PP_ARRAY_PUSH_FRONT_D(array, elem) PP_ARRAY_PUSH_FRONT_I(PP_ARRAY_SIZE(array), PP_ARRAY_DATA(array), elem)
# endif
#
# define PP_ARRAY_PUSH_FRONT_I(size, data, elem) (PP_INC(size), (elem PP_COMMA_IF(size) PP_ARRAY_DETAIL_GET_DATA(size,data)))
#
# endif
