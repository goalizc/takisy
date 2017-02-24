# ifndef PREPROCESSOR_SEQ_SUBSEQ_HPP
# define PREPROCESSOR_SEQ_SUBSEQ_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/seq/first_n.hpp>
# include <takisy/preprocessor/seq/rest_n.hpp>
#
# /* PP_SEQ_SUBSEQ */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SEQ_SUBSEQ(seq, i, len) PP_SEQ_FIRST_N(len, PP_SEQ_REST_N(i, seq))
# else
#    define PP_SEQ_SUBSEQ(seq, i, len) PP_SEQ_SUBSEQ_I(seq, i, len)
#    define PP_SEQ_SUBSEQ_I(seq, i, len) PP_SEQ_FIRST_N(len, PP_SEQ_REST_N(i, seq))
# endif
#
# endif
