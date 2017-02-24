# ifndef PREPROCESSOR_VARIADIC_TO_SEQ_HPP
# define PREPROCESSOR_VARIADIC_TO_SEQ_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/tuple/to_seq.hpp>
#
# /* PP_VARIADIC_TO_SEQ */
#
# if PP_VARIADICS
#    define PP_VARIADIC_TO_SEQ(...) PP_TUPLE_TO_SEQ((__VA_ARGS__))
# endif
#
# endif
