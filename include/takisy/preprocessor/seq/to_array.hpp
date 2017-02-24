# ifndef PREPROCESSOR_SEQ_TO_ARRAY_HPP
# define PREPROCESSOR_SEQ_TO_ARRAY_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/seq/enum.hpp>
# include <takisy/preprocessor/seq/size.hpp>
#
# /* PP_SEQ_TO_ARRAY */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SEQ_TO_ARRAY(seq) (PP_SEQ_SIZE(seq), (PP_SEQ_ENUM(seq)))
# else
#    define PP_SEQ_TO_ARRAY(seq) PP_SEQ_TO_ARRAY_I(seq)
#    define PP_SEQ_TO_ARRAY_I(seq) (PP_SEQ_SIZE(seq), (PP_SEQ_ENUM(seq)))
# endif
#
# endif
