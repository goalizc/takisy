# ifndef PREPROCESSOR_SEQ_INSERT_HPP
# define PREPROCESSOR_SEQ_INSERT_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/seq/first_n.hpp>
# include <takisy/preprocessor/seq/rest_n.hpp>
#
# /* PP_SEQ_INSERT */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SEQ_INSERT(seq, i, elem) PP_SEQ_FIRST_N(i, seq) (elem) PP_SEQ_REST_N(i, seq)
# else
#    define PP_SEQ_INSERT(seq, i, elem) PP_SEQ_INSERT_I(seq, i, elem)
#    define PP_SEQ_INSERT_I(seq, i, elem) PP_SEQ_FIRST_N(i, seq) (elem) PP_SEQ_REST_N(i, seq)
# endif
#
# endif
