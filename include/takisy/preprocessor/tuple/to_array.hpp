# ifndef PREPROCESSOR_TUPLE_TO_ARRAY_HPP
# define PREPROCESSOR_TUPLE_TO_ARRAY_HPP
#
# include <takisy/preprocessor/cat.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/facilities/overload.hpp>
# include <takisy/preprocessor/tuple/size.hpp>
# include <takisy/preprocessor/variadic/size.hpp>
#
# /* PP_TUPLE_TO_ARRAY */
#
# if PP_VARIADICS
#    if PP_VARIADICS_MSVC
#        define PP_TUPLE_TO_ARRAY(...) PP_TUPLE_TO_ARRAY_I(PP_OVERLOAD(PP_TUPLE_TO_ARRAY_, __VA_ARGS__), (__VA_ARGS__))
#        define PP_TUPLE_TO_ARRAY_I(m, args) PP_TUPLE_TO_ARRAY_II(m, args)
#        define PP_TUPLE_TO_ARRAY_II(m, args) PP_CAT(m ## args,)
#        define PP_TUPLE_TO_ARRAY_1(tuple) (PP_TUPLE_SIZE(tuple), tuple)
#    else
#        define PP_TUPLE_TO_ARRAY(...) PP_OVERLOAD(PP_TUPLE_TO_ARRAY_, __VA_ARGS__)(__VA_ARGS__)
#        define PP_TUPLE_TO_ARRAY_1(tuple) (PP_VARIADIC_SIZE tuple, tuple)
#    endif
#    define PP_TUPLE_TO_ARRAY_2(size, tuple) (size, tuple)
# else
#    define PP_TUPLE_TO_ARRAY(size, tuple) (size, tuple)
# endif
#
# endif
