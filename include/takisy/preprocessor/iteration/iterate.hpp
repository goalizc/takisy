# ifndef PREPROCESSOR_ITERATION_ITERATE_HPP
# define PREPROCESSOR_ITERATION_ITERATE_HPP
#
# include <takisy/preprocessor/arithmetic/dec.hpp>
# include <takisy/preprocessor/arithmetic/inc.hpp>
# include <takisy/preprocessor/array/elem.hpp>
# include <takisy/preprocessor/array/size.hpp>
# include <takisy/preprocessor/cat.hpp>
# include <takisy/preprocessor/slot/slot.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
#
# /* PP_ITERATION_DEPTH */
#
# define PP_ITERATION_DEPTH() 0
#
# /* PP_ITERATION */
#
# define PP_ITERATION() PP_CAT(PP_ITERATION_, PP_ITERATION_DEPTH())
#
# /* PP_ITERATION_START && PP_ITERATION_FINISH */
#
# define PP_ITERATION_START() PP_CAT(PP_ITERATION_START_, PP_ITERATION_DEPTH())
# define PP_ITERATION_FINISH() PP_CAT(PP_ITERATION_FINISH_, PP_ITERATION_DEPTH())
#
# /* PP_ITERATION_FLAGS */
#
# define PP_ITERATION_FLAGS() (PP_CAT(PP_ITERATION_FLAGS_, PP_ITERATION_DEPTH())())
#
# /* PP_FRAME_ITERATION */
#
# define PP_FRAME_ITERATION(i) PP_CAT(PP_ITERATION_, i)
#
# /* PP_FRAME_START && PP_FRAME_FINISH */
#
# define PP_FRAME_START(i) PP_CAT(PP_ITERATION_START_, i)
# define PP_FRAME_FINISH(i) PP_CAT(PP_ITERATION_FINISH_, i)
#
# /* PP_FRAME_FLAGS */
#
# define PP_FRAME_FLAGS(i) (PP_CAT(PP_ITERATION_FLAGS_, i)())
#
# /* PP_RELATIVE_ITERATION */
#
# define PP_RELATIVE_ITERATION(i) PP_CAT(PP_RELATIVE_, i)(PP_ITERATION_)
#
# define PP_RELATIVE_0(m) PP_CAT(m, PP_ITERATION_DEPTH())
# define PP_RELATIVE_1(m) PP_CAT(m, PP_DEC(PP_ITERATION_DEPTH()))
# define PP_RELATIVE_2(m) PP_CAT(m, PP_DEC(PP_DEC(PP_ITERATION_DEPTH())))
# define PP_RELATIVE_3(m) PP_CAT(m, PP_DEC(PP_DEC(PP_DEC(PP_ITERATION_DEPTH()))))
# define PP_RELATIVE_4(m) PP_CAT(m, PP_DEC(PP_DEC(PP_DEC(PP_DEC(PP_ITERATION_DEPTH())))))
#
# /* PP_RELATIVE_START && PP_RELATIVE_FINISH */
#
# define PP_RELATIVE_START(i) PP_CAT(PP_RELATIVE_, i)(PP_ITERATION_START_)
# define PP_RELATIVE_FINISH(i) PP_CAT(PP_RELATIVE_, i)(PP_ITERATION_FINISH_)
#
# /* PP_RELATIVE_FLAGS */
#
# define PP_RELATIVE_FLAGS(i) (PP_CAT(PP_RELATIVE_, i)(PP_ITERATION_FLAGS_)())
#
# /* PP_ITERATE */
#
# define PP_ITERATE() PP_CAT(PP_ITERATE_, PP_INC(PP_ITERATION_DEPTH()))
#
# define PP_ITERATE_1 <takisy/preprocessor/iteration/detail/iter/forward1.hpp>
# define PP_ITERATE_2 <takisy/preprocessor/iteration/detail/iter/forward2.hpp>
# define PP_ITERATE_3 <takisy/preprocessor/iteration/detail/iter/forward3.hpp>
# define PP_ITERATE_4 <takisy/preprocessor/iteration/detail/iter/forward4.hpp>
# define PP_ITERATE_5 <takisy/preprocessor/iteration/detail/iter/forward5.hpp>
#
# endif
