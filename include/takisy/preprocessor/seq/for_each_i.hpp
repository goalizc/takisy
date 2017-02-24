# ifndef PREPROCESSOR_SEQ_FOR_EACH_I_HPP
# define PREPROCESSOR_SEQ_FOR_EACH_I_HPP
#
# include <takisy/preprocessor/arithmetic/dec.hpp>
# include <takisy/preprocessor/arithmetic/inc.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/if.hpp>
# include <takisy/preprocessor/control/iif.hpp>
# include <takisy/preprocessor/repetition/for.hpp>
# include <takisy/preprocessor/seq/seq.hpp>
# include <takisy/preprocessor/seq/size.hpp>
# include <takisy/preprocessor/seq/detail/is_empty.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
# include <takisy/preprocessor/tuple/rem.hpp>
#
# /* PP_SEQ_FOR_EACH_I */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SEQ_FOR_EACH_I(macro, data, seq) PP_SEQ_FOR_EACH_I_DETAIL_CHECK(macro, data, seq)
# else
#    define PP_SEQ_FOR_EACH_I(macro, data, seq) PP_SEQ_FOR_EACH_I_I(macro, data, seq)
#    define PP_SEQ_FOR_EACH_I_I(macro, data, seq) PP_SEQ_FOR_EACH_I_DETAIL_CHECK(macro, data, seq)
# endif
#
#    define PP_SEQ_FOR_EACH_I_DETAIL_CHECK_EXEC(macro, data, seq) PP_FOR((macro, data, seq, 0, PP_SEQ_SIZE(seq)), PP_SEQ_FOR_EACH_I_P, PP_SEQ_FOR_EACH_I_O, PP_SEQ_FOR_EACH_I_M)
#    define PP_SEQ_FOR_EACH_I_DETAIL_CHECK_EMPTY(macro, data, seq)
#
#    define PP_SEQ_FOR_EACH_I_DETAIL_CHECK(macro, data, seq) \
		PP_IIF \
			( \
			PP_SEQ_DETAIL_IS_NOT_EMPTY(seq), \
			PP_SEQ_FOR_EACH_I_DETAIL_CHECK_EXEC, \
			PP_SEQ_FOR_EACH_I_DETAIL_CHECK_EMPTY \
			) \
		(macro, data, seq) \
/**/
#
# define PP_SEQ_FOR_EACH_I_P(r, x) PP_TUPLE_ELEM(5, 4, x)
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_STRICT()
#    define PP_SEQ_FOR_EACH_I_O(r, x) PP_SEQ_FOR_EACH_I_O_I x
# else
#    define PP_SEQ_FOR_EACH_I_O(r, x) PP_SEQ_FOR_EACH_I_O_I(PP_TUPLE_ELEM(5, 0, x), PP_TUPLE_ELEM(5, 1, x), PP_TUPLE_ELEM(5, 2, x), PP_TUPLE_ELEM(5, 3, x), PP_TUPLE_ELEM(5, 4, x))
# endif
#
# define PP_SEQ_FOR_EACH_I_O_I(macro, data, seq, i, sz) \
	PP_SEQ_FOR_EACH_I_O_I_DEC(macro, data, seq, i, PP_DEC(sz)) \
/**/
# define PP_SEQ_FOR_EACH_I_O_I_DEC(macro, data, seq, i, sz) \
	( \
	macro, \
	data, \
	PP_IF \
		( \
		sz, \
		PP_SEQ_FOR_EACH_I_O_I_TAIL, \
		PP_SEQ_FOR_EACH_I_O_I_NIL \
		) \
	(seq), \
	PP_INC(i), \
	sz \
	) \
/**/
# define PP_SEQ_FOR_EACH_I_O_I_TAIL(seq) PP_SEQ_TAIL(seq)
# define PP_SEQ_FOR_EACH_I_O_I_NIL(seq) PP_NIL
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_STRICT()
#    define PP_SEQ_FOR_EACH_I_M(r, x) PP_SEQ_FOR_EACH_I_M_IM(r, PP_TUPLE_REM_5 x)
#    define PP_SEQ_FOR_EACH_I_M_IM(r, im) PP_SEQ_FOR_EACH_I_M_I(r, im)
# else
#    define PP_SEQ_FOR_EACH_I_M(r, x) PP_SEQ_FOR_EACH_I_M_I(r, PP_TUPLE_ELEM(5, 0, x), PP_TUPLE_ELEM(5, 1, x), PP_TUPLE_ELEM(5, 2, x), PP_TUPLE_ELEM(5, 3, x), PP_TUPLE_ELEM(5, 4, x))
# endif
#
# define PP_SEQ_FOR_EACH_I_M_I(r, macro, data, seq, i, sz) macro(r, data, i, PP_SEQ_HEAD(seq))
#
# /* PP_SEQ_FOR_EACH_I_R */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SEQ_FOR_EACH_I_R(r, macro, data, seq) PP_SEQ_FOR_EACH_I_R_DETAIL_CHECK(r, macro, data, seq)
# else
#    define PP_SEQ_FOR_EACH_I_R(r, macro, data, seq) PP_SEQ_FOR_EACH_I_R_I(r, macro, data, seq)
#    define PP_SEQ_FOR_EACH_I_R_I(r, macro, data, seq) PP_SEQ_FOR_EACH_I_R_DETAIL_CHECK(r, macro, data, seq)
# endif
#
#    define PP_SEQ_FOR_EACH_I_R_DETAIL_CHECK_EXEC(r, macro, data, seq) PP_FOR_ ## r((macro, data, seq, 0, PP_SEQ_SIZE(seq)), PP_SEQ_FOR_EACH_I_P, PP_SEQ_FOR_EACH_I_O, PP_SEQ_FOR_EACH_I_M)
#    define PP_SEQ_FOR_EACH_I_R_DETAIL_CHECK_EMPTY(r, macro, data, seq)
#
#    define PP_SEQ_FOR_EACH_I_R_DETAIL_CHECK(r, macro, data, seq) \
		PP_IIF \
			( \
			PP_SEQ_DETAIL_IS_NOT_EMPTY(seq), \
			PP_SEQ_FOR_EACH_I_R_DETAIL_CHECK_EXEC, \
			PP_SEQ_FOR_EACH_I_R_DETAIL_CHECK_EMPTY \
			) \
		(r, macro, data, seq) \
/**/
#
# endif
