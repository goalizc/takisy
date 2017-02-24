# ifndef PREPROCESSOR_ARRAY_ENUM_HPP
# define PREPROCESSOR_ARRAY_ENUM_HPP
#
# include <takisy/preprocessor/cat.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/tuple/rem.hpp>
#
# /* PP_ARRAY_ENUM */
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_MSVC()
#    define PP_ARRAY_ENUM(array) PP_ARRAY_ENUM_I(PP_TUPLE_REM_CTOR, array)
#    define PP_ARRAY_ENUM_I(m, args) PP_ARRAY_ENUM_II(m, args)
#    define PP_ARRAY_ENUM_II(m, args) PP_CAT(m ## args,)
# elif PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_ARRAY_ENUM(array) PP_ARRAY_ENUM_I(array)
#    define PP_ARRAY_ENUM_I(array) PP_TUPLE_REM_CTOR ## array
# else
#    define PP_ARRAY_ENUM(array) PP_TUPLE_REM_CTOR array
# endif
#
# endif
