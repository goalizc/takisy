# ifndef PREPROCESSOR_ARRAY_TO_SEQ_HPP
# define PREPROCESSOR_ARRAY_TO_SEQ_HPP
#
# include <takisy/preprocessor/cat.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/array/size.hpp>
# include <takisy/preprocessor/control/if.hpp>
# include <takisy/preprocessor/tuple/to_seq.hpp>
#
# /* PP_ARRAY_TO_SEQ */
#
#    define PP_ARRAY_TO_SEQ(array) \
		PP_IF \
			( \
			PP_ARRAY_SIZE(array), \
			PP_ARRAY_TO_SEQ_DO, \
			PP_ARRAY_TO_SEQ_EMPTY \
			) \
		(array) \
/**/
#    define PP_ARRAY_TO_SEQ_EMPTY(array)
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_MSVC()
#    define PP_ARRAY_TO_SEQ_DO(array) PP_ARRAY_TO_SEQ_I(PP_TUPLE_TO_SEQ, array)
#    define PP_ARRAY_TO_SEQ_I(m, args) PP_ARRAY_TO_SEQ_II(m, args)
#    define PP_ARRAY_TO_SEQ_II(m, args) PP_CAT(m ## args,)
# elif PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_ARRAY_TO_SEQ_DO(array) PP_ARRAY_TO_SEQ_I(array)
#    define PP_ARRAY_TO_SEQ_I(array) PP_TUPLE_TO_SEQ ## array
# else
#    define PP_ARRAY_TO_SEQ_DO(array) PP_TUPLE_TO_SEQ array
# endif
#
# endif
