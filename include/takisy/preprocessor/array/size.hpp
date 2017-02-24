# ifndef PREPROCESSOR_ARRAY_SIZE_HPP
# define PREPROCESSOR_ARRAY_SIZE_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
#
# /* PP_ARRAY_SIZE */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_ARRAY_SIZE(array) PP_TUPLE_ELEM(2, 0, array)
# else
#    define PP_ARRAY_SIZE(array) PP_ARRAY_SIZE_I(array)
#    define PP_ARRAY_SIZE_I(array) PP_ARRAY_SIZE_II array
#    define PP_ARRAY_SIZE_II(size, data) size
# endif
#
# endif
