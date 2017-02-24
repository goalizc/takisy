# ifndef PREPROCESSOR_SEQ_VARIADIC_SEQ_TO_SEQ_HPP
# define PREPROCESSOR_SEQ_VARIADIC_SEQ_TO_SEQ_HPP
#
# include <takisy/preprocessor/cat.hpp>
# include <takisy/preprocessor/config/config.hpp>
#
# /* PP_VARIADIC_SEQ_TO_SEQ */
#
# if PP_VARIADICS
#    define PP_VARIADIC_SEQ_TO_SEQ(vseq) PP_CAT(PP_VARIADIC_SEQ_TO_SEQ_A vseq, 0)
#    define PP_VARIADIC_SEQ_TO_SEQ_A(...) ((__VA_ARGS__)) PP_VARIADIC_SEQ_TO_SEQ_B
#    define PP_VARIADIC_SEQ_TO_SEQ_B(...) ((__VA_ARGS__)) PP_VARIADIC_SEQ_TO_SEQ_A
#    define PP_VARIADIC_SEQ_TO_SEQ_A0
#    define PP_VARIADIC_SEQ_TO_SEQ_B0
# endif
#
# endif
