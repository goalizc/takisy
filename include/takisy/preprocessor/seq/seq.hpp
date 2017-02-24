# ifndef PREPROCESSOR_SEQ_SEQ_HPP
# define PREPROCESSOR_SEQ_SEQ_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/seq/elem.hpp>
#
# /* PP_SEQ_HEAD */
#
# define PP_SEQ_HEAD(seq) PP_SEQ_ELEM(0, seq)
#
# /* PP_SEQ_TAIL */
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_SEQ_TAIL(seq) PP_SEQ_TAIL_1((seq))
#    define PP_SEQ_TAIL_1(par) PP_SEQ_TAIL_2 ## par
#    define PP_SEQ_TAIL_2(seq) PP_SEQ_TAIL_I ## seq
# elif PP_CONFIG_FLAGS() & PP_CONFIG_MSVC()
#    define PP_SEQ_TAIL(seq) PP_SEQ_TAIL_ID(PP_SEQ_TAIL_I seq)
#    define PP_SEQ_TAIL_ID(id) id
# elif PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SEQ_TAIL(seq) PP_SEQ_TAIL_D(seq)
#    define PP_SEQ_TAIL_D(seq) PP_SEQ_TAIL_I seq
# else
#    define PP_SEQ_TAIL(seq) PP_SEQ_TAIL_I seq
# endif
#
# define PP_SEQ_TAIL_I(x)
#
# /* PP_SEQ_NIL */
#
# define PP_SEQ_NIL(x) (x)
#
# endif
