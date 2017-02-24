# ifndef PREPROCESSOR_VARIADIC_TO_ARRAY_HPP
# define PREPROCESSOR_VARIADIC_TO_ARRAY_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/tuple/to_array.hpp>
# if PP_VARIADICS_MSVC
#    include <takisy/preprocessor/variadic/size.hpp>
# endif
#
# /* PP_VARIADIC_TO_ARRAY */
#
# if PP_VARIADICS
#    if PP_VARIADICS_MSVC
#        define PP_VARIADIC_TO_ARRAY(...) PP_TUPLE_TO_ARRAY_2(PP_VARIADIC_SIZE(__VA_ARGS__),(__VA_ARGS__))
#    else
#        define PP_VARIADIC_TO_ARRAY(...) PP_TUPLE_TO_ARRAY((__VA_ARGS__))
#    endif
# endif
#
# endif
