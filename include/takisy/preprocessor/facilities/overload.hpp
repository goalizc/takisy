# ifndef PREPROCESSOR_FACILITIES_OVERLOAD_HPP
# define PREPROCESSOR_FACILITIES_OVERLOAD_HPP
#
# include <takisy/preprocessor/cat.hpp>
# include <takisy/preprocessor/variadic/size.hpp>
#
# /* PP_OVERLOAD */
#
# if PP_VARIADICS
#    define PP_OVERLOAD(prefix, ...) PP_CAT(prefix, PP_VARIADIC_SIZE(__VA_ARGS__))
# endif
#
# endif
