# ifndef PREPROCESSOR_SEQ_REMOVE_HPP
# define PREPROCESSOR_SEQ_REMOVE_HPP
#
# include <takisy/preprocessor/arithmetic/inc.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/seq/first_n.hpp>
# include <takisy/preprocessor/seq/rest_n.hpp>
#
# /* PP_SEQ_REMOVE */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SEQ_REMOVE(seq, i) PP_SEQ_FIRST_N(i, seq) PP_SEQ_REST_N(PP_INC(i), seq)
# else
#    define PP_SEQ_REMOVE(seq, i) PP_SEQ_REMOVE_I(seq, i)
#    define PP_SEQ_REMOVE_I(seq, i) PP_SEQ_FIRST_N(i, seq) PP_SEQ_REST_N(PP_INC(i), seq)
# endif
#
# endif
