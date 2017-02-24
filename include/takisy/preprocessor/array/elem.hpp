# ifndef PREPROCESSOR_ARRAY_ELEM_HPP
# define PREPROCESSOR_ARRAY_ELEM_HPP
#
# include <takisy/preprocessor/array/data.hpp>
# include <takisy/preprocessor/array/size.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
#
# /* PP_ARRAY_ELEM */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_ARRAY_ELEM(i, array) PP_TUPLE_ELEM(PP_ARRAY_SIZE(array), i, PP_ARRAY_DATA(array))
# else
#    define PP_ARRAY_ELEM(i, array) PP_ARRAY_ELEM_I(i, array)
#    define PP_ARRAY_ELEM_I(i, array) PP_TUPLE_ELEM(PP_ARRAY_SIZE(array), i, PP_ARRAY_DATA(array))
# endif
#
# endif
