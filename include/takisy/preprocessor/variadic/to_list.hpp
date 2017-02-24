# ifndef PREPROCESSOR_VARIADIC_TO_LIST_HPP
# define PREPROCESSOR_VARIADIC_TO_LIST_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/tuple/to_list.hpp>
#
# /* PP_VARIADIC_TO_LIST */
#
# if PP_VARIADICS
#    define PP_VARIADIC_TO_LIST(...) PP_TUPLE_TO_LIST((__VA_ARGS__))
# endif
#
# endif
