# ifndef PREPROCESSOR_VARIADIC_TO_TUPLE_HPP
# define PREPROCESSOR_VARIADIC_TO_TUPLE_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# /* PP_VARIADIC_TO_TUPLE */
#
# if PP_VARIADICS
#    define PP_VARIADIC_TO_TUPLE(...) (__VA_ARGS__)
# endif
#
# endif
