# ifndef PREPROCESSOR_SEQ_REPLACE_HPP
# define PREPROCESSOR_SEQ_REPLACE_HPP
#
# include <takisy/preprocessor/arithmetic/dec.hpp>
# include <takisy/preprocessor/arithmetic/inc.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/comparison/equal.hpp>
# include <takisy/preprocessor/control/iif.hpp>
# include <takisy/preprocessor/seq/first_n.hpp>
# include <takisy/preprocessor/seq/rest_n.hpp>
# include <takisy/preprocessor/seq/size.hpp>
#
# /* PP_SEQ_REPLACE */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SEQ_REPLACE(seq, i, elem) PP_SEQ_FIRST_N(i, seq) (elem) PP_SEQ_REPLACE_DETAIL_REST(seq, i)
# else
#    define PP_SEQ_REPLACE(seq, i, elem) PP_SEQ_REPLACE_I(seq, i, elem)
#    define PP_SEQ_REPLACE_I(seq, i, elem) PP_SEQ_FIRST_N(i, seq) (elem) PP_SEQ_REPLACE_DETAIL_REST(seq, i)
# endif
#
#    define PP_SEQ_REPLACE_DETAIL_REST_EMPTY(seq, i)
#    define PP_SEQ_REPLACE_DETAIL_REST_VALID(seq, i) PP_SEQ_REST_N(PP_INC(i), seq)
#    define PP_SEQ_REPLACE_DETAIL_REST(seq, i) \
		PP_IIF \
			( \
			PP_EQUAL(i,PP_DEC(PP_SEQ_SIZE(seq))), \
			PP_SEQ_REPLACE_DETAIL_REST_EMPTY, \
			PP_SEQ_REPLACE_DETAIL_REST_VALID \
			) \
		(seq, i) \
/**/
#
# endif
