# ifndef PREPROCESSOR_ARRAY_DATA_HPP
# define PREPROCESSOR_ARRAY_DATA_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
#
# /* PP_ARRAY_DATA */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_ARRAY_DATA(array) PP_TUPLE_ELEM(2, 1, array)
# else
#    define PP_ARRAY_DATA(array) PP_ARRAY_DATA_I(array)
#    define PP_ARRAY_DATA_I(array) PP_ARRAY_DATA_II array
#    define PP_ARRAY_DATA_II(size, data) data
# endif
#
# endif
