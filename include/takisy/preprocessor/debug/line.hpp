# ifndef PREPROCESSOR_DEBUG_LINE_HPP
# define PREPROCESSOR_DEBUG_LINE_HPP
#
# include <takisy/preprocessor/cat.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/iteration/iterate.hpp>
# include <takisy/preprocessor/stringize.hpp>
#
# /* PP_LINE */
#
# if PP_CONFIG_EXTENDED_LINE_INFO
#    define PP_LINE(line, file) line PP_CAT(PP_LINE_, PP_IS_ITERATING)(file)
#    define PP_LINE_PP_IS_ITERATING(file) #file
#    define PP_LINE_1(file) PP_STRINGIZE(file PP_CAT(PP_LINE_I_, PP_ITERATION_DEPTH())())
#    define PP_LINE_I_1() [PP_FRAME_ITERATION(1)]
#    define PP_LINE_I_2() PP_LINE_I_1()[PP_FRAME_ITERATION(2)]
#    define PP_LINE_I_3() PP_LINE_I_2()[PP_FRAME_ITERATION(3)]
#    define PP_LINE_I_4() PP_LINE_I_3()[PP_FRAME_ITERATION(4)]
#    define PP_LINE_I_5() PP_LINE_I_4()[PP_FRAME_ITERATION(5)]
# else
#    define PP_LINE(line, file) line __FILE__
# endif
#
# endif
