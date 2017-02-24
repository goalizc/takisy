# ifndef PREPROCESSOR_SEQ_TO_LIST_HPP
# define PREPROCESSOR_SEQ_TO_LIST_HPP
#
# include <takisy/preprocessor/punctuation/comma.hpp>
# include <takisy/preprocessor/punctuation/paren.hpp>
# include <takisy/preprocessor/seq/detail/binary_transform.hpp>
#
# /* PP_SEQ_TO_LIST */
#
# define PP_SEQ_TO_LIST(seq) PP_SEQ_TO_LIST_I(PP_SEQ_BINARY_TRANSFORM(seq))
# define PP_SEQ_TO_LIST_I(bseq) PP_SEQ_TO_LIST_A bseq PP_NIL PP_SEQ_TO_LIST_B bseq
# define PP_SEQ_TO_LIST_A(m, e) m(PP_LPAREN() e PP_COMMA() PP_SEQ_TO_LIST_A_ID)
# define PP_SEQ_TO_LIST_A_ID() PP_SEQ_TO_LIST_A
# define PP_SEQ_TO_LIST_B(m, e) m(PP_RPAREN() PP_SEQ_TO_LIST_B_ID)
# define PP_SEQ_TO_LIST_B_ID() PP_SEQ_TO_LIST_B
#
# endif
