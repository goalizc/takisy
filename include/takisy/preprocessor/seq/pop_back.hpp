# ifndef PREPROCESSOR_SEQ_POP_BACK_HPP
# define PREPROCESSOR_SEQ_POP_BACK_HPP
#
# include <takisy/preprocessor/arithmetic/dec.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/seq/first_n.hpp>
# include <takisy/preprocessor/seq/size.hpp>
#
# /* PP_SEQ_POP_BACK */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SEQ_POP_BACK(seq) PP_SEQ_FIRST_N(PP_DEC(PP_SEQ_SIZE(seq)), seq)
# else
#    define PP_SEQ_POP_BACK(seq) PP_SEQ_POP_BACK_I(seq)
#    define PP_SEQ_POP_BACK_I(seq) PP_SEQ_FIRST_N(PP_DEC(PP_SEQ_SIZE(seq)), seq)
# endif
#
# endif
