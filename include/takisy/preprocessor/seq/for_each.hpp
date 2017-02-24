# ifndef PREPROCESSOR_SEQ_FOR_EACH_HPP
# define PREPROCESSOR_SEQ_FOR_EACH_HPP
#
# include <takisy/preprocessor/arithmetic/dec.hpp>
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
# /* PP_SEQ_FOR_EACH */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SEQ_FOR_EACH(macro, data, seq) PP_SEQ_FOR_EACH_DETAIL_CHECK(macro, data, seq)
# else
#    define PP_SEQ_FOR_EACH(macro, data, seq) PP_SEQ_FOR_EACH_D(macro, data, seq)
#    define PP_SEQ_FOR_EACH_D(macro, data, seq) PP_SEQ_FOR_EACH_DETAIL_CHECK(macro, data, seq)
# endif
#
#    define PP_SEQ_FOR_EACH_DETAIL_CHECK_EXEC(macro, data, seq) PP_FOR((macro, data, seq, PP_SEQ_SIZE(seq)), PP_SEQ_FOR_EACH_P, PP_SEQ_FOR_EACH_O, PP_SEQ_FOR_EACH_M)
#    define PP_SEQ_FOR_EACH_DETAIL_CHECK_EMPTY(macro, data, seq)
#
#    define PP_SEQ_FOR_EACH_DETAIL_CHECK(macro, data, seq) \
		PP_IIF \
			( \
			PP_SEQ_DETAIL_IS_NOT_EMPTY(seq), \
			PP_SEQ_FOR_EACH_DETAIL_CHECK_EXEC, \
			PP_SEQ_FOR_EACH_DETAIL_CHECK_EMPTY \
			) \
		(macro, data, seq) \
/**/
#
# define PP_SEQ_FOR_EACH_P(r, x) PP_TUPLE_ELEM(4, 3, x)
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_STRICT()
#    define PP_SEQ_FOR_EACH_O(r, x) PP_SEQ_FOR_EACH_O_I x
# else
#    define PP_SEQ_FOR_EACH_O(r, x) PP_SEQ_FOR_EACH_O_I(PP_TUPLE_ELEM(4, 0, x), PP_TUPLE_ELEM(4, 1, x), PP_TUPLE_ELEM(4, 2, x), PP_TUPLE_ELEM(4, 3, x))
# endif
#
# define PP_SEQ_FOR_EACH_O_I(macro, data, seq, sz) \
	PP_SEQ_FOR_EACH_O_I_DEC(macro, data, seq, PP_DEC(sz)) \
/**/
# define PP_SEQ_FOR_EACH_O_I_DEC(macro, data, seq, sz) \
	( \
	macro, \
	data, \
	PP_IF \
		( \
		sz, \
		PP_SEQ_FOR_EACH_O_I_TAIL, \
		PP_SEQ_FOR_EACH_O_I_NIL \
		) \
	(seq), \
	sz \
	) \
/**/
# define PP_SEQ_FOR_EACH_O_I_TAIL(seq) PP_SEQ_TAIL(seq)
# define PP_SEQ_FOR_EACH_O_I_NIL(seq) PP_NIL
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_STRICT()
#    define PP_SEQ_FOR_EACH_M(r, x) PP_SEQ_FOR_EACH_M_IM(r, PP_TUPLE_REM_4 x)
#    define PP_SEQ_FOR_EACH_M_IM(r, im) PP_SEQ_FOR_EACH_M_I(r, im)
# else
#    define PP_SEQ_FOR_EACH_M(r, x) PP_SEQ_FOR_EACH_M_I(r, PP_TUPLE_ELEM(4, 0, x), PP_TUPLE_ELEM(4, 1, x), PP_TUPLE_ELEM(4, 2, x), PP_TUPLE_ELEM(4, 3, x))
# endif
#
# define PP_SEQ_FOR_EACH_M_I(r, macro, data, seq, sz) macro(r, data, PP_SEQ_HEAD(seq))
#
# /* PP_SEQ_FOR_EACH_R */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_SEQ_FOR_EACH_R(r, macro, data, seq) PP_SEQ_FOR_EACH_DETAIL_CHECK_R(r, macro, data, seq)
# else
#    define PP_SEQ_FOR_EACH_R(r, macro, data, seq) PP_SEQ_FOR_EACH_R_I(r, macro, data, seq)
#    define PP_SEQ_FOR_EACH_R_I(r, macro, data, seq) PP_SEQ_FOR_EACH_DETAIL_CHECK_R(r, macro, data, seq)
# endif
#
#    define PP_SEQ_FOR_EACH_DETAIL_CHECK_EXEC_R(r, macro, data, seq) PP_FOR_ ## r((macro, data, seq, PP_SEQ_SIZE(seq)), PP_SEQ_FOR_EACH_P, PP_SEQ_FOR_EACH_O, PP_SEQ_FOR_EACH_M)
#    define PP_SEQ_FOR_EACH_DETAIL_CHECK_EMPTY_R(r, macro, data, seq)
#
#    define PP_SEQ_FOR_EACH_DETAIL_CHECK_R(r, macro, data, seq) \
		PP_IIF \
			( \
			PP_SEQ_DETAIL_IS_NOT_EMPTY(seq), \
			PP_SEQ_FOR_EACH_DETAIL_CHECK_EXEC_R, \
			PP_SEQ_FOR_EACH_DETAIL_CHECK_EMPTY_R \
			) \
		(r, macro, data, seq) \
/**/
#
# endif
