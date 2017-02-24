# ifndef PREPROCESSOR_SEQ_CAT_HPP
# define PREPROCESSOR_SEQ_CAT_HPP
#
# include <takisy/preprocessor/arithmetic/dec.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/if.hpp>
# include <takisy/preprocessor/seq/fold_left.hpp>
# include <takisy/preprocessor/seq/seq.hpp>
# include <takisy/preprocessor/seq/size.hpp>
# include <takisy/preprocessor/tuple/eat.hpp>
#
# /* PP_SEQ_CAT */
#
# define PP_SEQ_CAT(seq) \
    PP_IF( \
        PP_DEC(PP_SEQ_SIZE(seq)), \
        PP_SEQ_CAT_I, \
        PP_SEQ_HEAD \
    )(seq) \
    /**/
# define PP_SEQ_CAT_I(seq) PP_SEQ_FOLD_LEFT(PP_SEQ_CAT_O, PP_SEQ_HEAD(seq), PP_SEQ_TAIL(seq))
#
# define PP_SEQ_CAT_O(s, st, elem) PP_SEQ_CAT_O_I(st, elem)
# define PP_SEQ_CAT_O_I(a, b) a ## b
#
# /* PP_SEQ_CAT_S */
#
# define PP_SEQ_CAT_S(s, seq) \
    PP_IF( \
        PP_DEC(PP_SEQ_SIZE(seq)), \
        PP_SEQ_CAT_S_I_A, \
        PP_SEQ_CAT_S_I_B \
    )(s, seq) \
    /**/
# define PP_SEQ_CAT_S_I_A(s, seq) PP_SEQ_FOLD_LEFT_ ## s(PP_SEQ_CAT_O, PP_SEQ_HEAD(seq), PP_SEQ_TAIL(seq))
# define PP_SEQ_CAT_S_I_B(s, seq) PP_SEQ_HEAD(seq)
#
# endif
