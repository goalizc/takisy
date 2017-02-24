# ifndef PREPROCESSOR_SEQ_TO_TUPLE_HPP
# define PREPROCESSOR_SEQ_TO_TUPLE_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/seq/enum.hpp>
#
# /* PP_SEQ_TO_TUPLE */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SEQ_TO_TUPLE(seq) (PP_SEQ_ENUM(seq))
# else
#    define PP_SEQ_TO_TUPLE(seq) PP_SEQ_TO_TUPLE_I(seq)
#    define PP_SEQ_TO_TUPLE_I(seq) (PP_SEQ_ENUM(seq))
# endif
#
# endif
