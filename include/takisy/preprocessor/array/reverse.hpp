# ifndef PREPROCESSOR_ARRAY_REVERSE_HPP
# define PREPROCESSOR_ARRAY_REVERSE_HPP
#
# include <takisy/preprocessor/array/data.hpp>
# include <takisy/preprocessor/array/size.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/tuple/reverse.hpp>
#
# /* PP_ARRAY_REVERSE */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_ARRAY_REVERSE(array) (PP_ARRAY_SIZE(array), PP_TUPLE_REVERSE(PP_ARRAY_SIZE(array), PP_ARRAY_DATA(array)))
# else
#    define PP_ARRAY_REVERSE(array) PP_ARRAY_REVERSE_I(array)
#    define PP_ARRAY_REVERSE_I(array) (PP_ARRAY_SIZE(array), PP_TUPLE_REVERSE(PP_ARRAY_SIZE(array), PP_ARRAY_DATA(array)))
# endif
#
# endif
