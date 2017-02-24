# ifndef PREPROCESSOR_SEQ_POP_FRONT_HPP
# define PREPROCESSOR_SEQ_POP_FRONT_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/seq/seq.hpp>
#
# /* PP_SEQ_POP_FRONT */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SEQ_POP_FRONT(seq) PP_SEQ_TAIL(seq)
# else
#    define PP_SEQ_POP_FRONT(seq) PP_SEQ_POP_FRONT_I(seq)
#    define PP_SEQ_POP_FRONT_I(seq) PP_SEQ_TAIL(seq)
# endif
#
# endif
