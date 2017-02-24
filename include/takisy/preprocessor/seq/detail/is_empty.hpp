# ifndef PREPROCESSOR_SEQ_DETAIL_IS_EMPTY_HPP
# define PREPROCESSOR_SEQ_DETAIL_IS_EMPTY_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/arithmetic/dec.hpp>
# include <takisy/preprocessor/logical/bool.hpp>
# include <takisy/preprocessor/logical/compl.hpp>
# include <takisy/preprocessor/seq/size.hpp>
#
/* An empty seq is one that is just PP_SEQ_NIL */
#
# define PP_SEQ_DETAIL_IS_EMPTY(seq) \
	PP_COMPL \
		( \
		PP_SEQ_DETAIL_IS_NOT_EMPTY(seq) \
		) \
/**/
#
# define PP_SEQ_DETAIL_IS_EMPTY_SIZE(size) \
	PP_COMPL \
		( \
		PP_SEQ_DETAIL_IS_NOT_EMPTY_SIZE(size) \
		) \
/**/
#
# define PP_SEQ_DETAIL_IS_NOT_EMPTY(seq) \
	PP_SEQ_DETAIL_IS_NOT_EMPTY_SIZE(PP_SEQ_DETAIL_EMPTY_SIZE(seq)) \
/**/
#
# define PP_SEQ_DETAIL_IS_NOT_EMPTY_SIZE(size) \
	PP_BOOL(size) \
/**/
#
# define PP_SEQ_DETAIL_EMPTY_SIZE(seq) \
	PP_DEC(PP_SEQ_SIZE(seq (nil))) \
/**/
#
# endif
