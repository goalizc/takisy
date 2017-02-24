# ifndef PREPROCESSOR_SEQ_FIRST_N_HPP
# define PREPROCESSOR_SEQ_FIRST_N_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/if.hpp>
# include <takisy/preprocessor/seq/detail/split.hpp>
# include <takisy/preprocessor/tuple/eat.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
#
# /* PP_SEQ_FIRST_N */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SEQ_FIRST_N(n, seq) PP_IF(n, PP_TUPLE_ELEM, PP_TUPLE_EAT_3)(2, 0, PP_SEQ_SPLIT(n, seq (nil)))
# else
#    define PP_SEQ_FIRST_N(n, seq) PP_SEQ_FIRST_N_I(n, seq)
#    define PP_SEQ_FIRST_N_I(n, seq) PP_IF(n, PP_TUPLE_ELEM, PP_TUPLE_EAT_3)(2, 0, PP_SEQ_SPLIT(n, seq (nil)))
# endif
#
# endif
