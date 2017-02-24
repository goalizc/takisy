# ifndef PREPROCESSOR_SEQ_REVERSE_HPP
# define PREPROCESSOR_SEQ_REVERSE_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/facilities/empty.hpp>
# include <takisy/preprocessor/seq/fold_left.hpp>
#
# /* PP_SEQ_REVERSE */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SEQ_REVERSE(seq) PP_SEQ_FOLD_LEFT(PP_SEQ_REVERSE_O, PP_EMPTY, seq)()
# else
#    define PP_SEQ_REVERSE(seq) PP_SEQ_REVERSE_I(seq)
#    define PP_SEQ_REVERSE_I(seq) PP_SEQ_FOLD_LEFT(PP_SEQ_REVERSE_O, PP_EMPTY, seq)()
# endif
#
# define PP_SEQ_REVERSE_O(s, state, elem) (elem) state
#
# /* PP_SEQ_REVERSE_S */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SEQ_REVERSE_S(s, seq) PP_SEQ_FOLD_LEFT_ ## s(PP_SEQ_REVERSE_O, PP_EMPTY, seq)()
# else
#    define PP_SEQ_REVERSE_S(s, seq) PP_SEQ_REVERSE_S_I(s, seq)
#    define PP_SEQ_REVERSE_S_I(s, seq) PP_SEQ_FOLD_LEFT_ ## s(PP_SEQ_REVERSE_O, PP_EMPTY, seq)()
# endif
#
# endif
