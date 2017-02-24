# ifndef PREPROCESSOR_SEQ_REST_N_HPP
# define PREPROCESSOR_SEQ_REST_N_HPP
#
# include <takisy/preprocessor/arithmetic/inc.hpp>
# include <takisy/preprocessor/comparison/not_equal.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/expr_iif.hpp>
# include <takisy/preprocessor/facilities/identity.hpp>
# include <takisy/preprocessor/logical/bitand.hpp>
# include <takisy/preprocessor/seq/detail/is_empty.hpp>
# include <takisy/preprocessor/seq/detail/split.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
#
# /* PP_SEQ_REST_N */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SEQ_REST_N(n, seq) PP_SEQ_REST_N_DETAIL_EXEC(n, seq, PP_SEQ_DETAIL_EMPTY_SIZE(seq))
# else
#    define PP_SEQ_REST_N(n, seq) PP_SEQ_REST_N_I(n, seq)
#    define PP_SEQ_REST_N_I(n, seq) PP_SEQ_REST_N_DETAIL_EXEC(n, seq, PP_SEQ_DETAIL_EMPTY_SIZE(seq))
# endif
#
#    define PP_SEQ_REST_N_DETAIL_EXEC(n, seq, size) \
		PP_EXPR_IIF \
			( \
			PP_BITAND \
				( \
				PP_SEQ_DETAIL_IS_NOT_EMPTY_SIZE(size), \
				PP_NOT_EQUAL(n,size) \
				), \
			PP_TUPLE_ELEM(2, 1, PP_SEQ_SPLIT(PP_INC(n), PP_IDENTITY( (nil) seq )))() \
			) \
/**/
#
# endif
