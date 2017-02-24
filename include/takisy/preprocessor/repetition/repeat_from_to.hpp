# ifndef PREPROCESSOR_REPETITION_REPEAT_FROM_TO_HPP
# define PREPROCESSOR_REPETITION_REPEAT_FROM_TO_HPP
#
# include <takisy/preprocessor/arithmetic/add.hpp>
# include <takisy/preprocessor/arithmetic/sub.hpp>
# include <takisy/preprocessor/cat.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/while.hpp>
# include <takisy/preprocessor/debug/error.hpp>
# include <takisy/preprocessor/detail/auto_rec.hpp>
# include <takisy/preprocessor/repetition/repeat.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
# include <takisy/preprocessor/tuple/rem.hpp>
#
# /* PP_REPEAT_FROM_TO */
#
# if 0
#    define PP_REPEAT_FROM_TO(first, last, macro, data)
# endif
#
# define PP_REPEAT_FROM_TO PP_CAT(PP_REPEAT_FROM_TO_, PP_AUTO_REC(PP_REPEAT_P, 4))
#
# define PP_REPEAT_FROM_TO_1(f, l, m, dt) PP_REPEAT_FROM_TO_D_1(PP_AUTO_REC(PP_WHILE_P, 256), f, l, m, dt)
# define PP_REPEAT_FROM_TO_2(f, l, m, dt) PP_REPEAT_FROM_TO_D_2(PP_AUTO_REC(PP_WHILE_P, 256), f, l, m, dt)
# define PP_REPEAT_FROM_TO_3(f, l, m, dt) PP_REPEAT_FROM_TO_D_3(PP_AUTO_REC(PP_WHILE_P, 256), f, l, m, dt)
# define PP_REPEAT_FROM_TO_4(f, l, m, dt) PP_ERROR(0x0003)
#
# define PP_REPEAT_FROM_TO_1ST PP_REPEAT_FROM_TO_1
# define PP_REPEAT_FROM_TO_2ND PP_REPEAT_FROM_TO_2
# define PP_REPEAT_FROM_TO_3RD PP_REPEAT_FROM_TO_3
#
# /* PP_REPEAT_FROM_TO_D */
#
# if 0
#    define PP_REPEAT_FROM_TO_D(d, first, last, macro, data)
# endif
#
# define PP_REPEAT_FROM_TO_D PP_CAT(PP_REPEAT_FROM_TO_D_, PP_AUTO_REC(PP_REPEAT_P, 4))
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_REPEAT_FROM_TO_D_1(d, f, l, m, dt) PP_REPEAT_1(PP_SUB_D(d, l, f), PP_REPEAT_FROM_TO_M_1, (d, f, m, dt))
#    define PP_REPEAT_FROM_TO_D_2(d, f, l, m, dt) PP_REPEAT_2(PP_SUB_D(d, l, f), PP_REPEAT_FROM_TO_M_2, (d, f, m, dt))
#    define PP_REPEAT_FROM_TO_D_3(d, f, l, m, dt) PP_REPEAT_3(PP_SUB_D(d, l, f), PP_REPEAT_FROM_TO_M_3, (d, f, m, dt))
# else
#    define PP_REPEAT_FROM_TO_D_1(d, f, l, m, dt) PP_REPEAT_FROM_TO_D_1_I(d, f, l, m, dt)
#    define PP_REPEAT_FROM_TO_D_2(d, f, l, m, dt) PP_REPEAT_FROM_TO_D_2_I(d, f, l, m, dt)
#    define PP_REPEAT_FROM_TO_D_3(d, f, l, m, dt) PP_REPEAT_FROM_TO_D_3_I(d, f, l, m, dt)
#    define PP_REPEAT_FROM_TO_D_1_I(d, f, l, m, dt) PP_REPEAT_1(PP_SUB_D(d, l, f), PP_REPEAT_FROM_TO_M_1, (d, f, m, dt))
#    define PP_REPEAT_FROM_TO_D_2_I(d, f, l, m, dt) PP_REPEAT_2(PP_SUB_D(d, l, f), PP_REPEAT_FROM_TO_M_2, (d, f, m, dt))
#    define PP_REPEAT_FROM_TO_D_3_I(d, f, l, m, dt) PP_REPEAT_3(PP_SUB_D(d, l, f), PP_REPEAT_FROM_TO_M_3, (d, f, m, dt))
# endif
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_STRICT()
#    define PP_REPEAT_FROM_TO_M_1(z, n, dfmd) PP_REPEAT_FROM_TO_M_1_IM(z, n, PP_TUPLE_REM_4 dfmd)
#    define PP_REPEAT_FROM_TO_M_2(z, n, dfmd) PP_REPEAT_FROM_TO_M_2_IM(z, n, PP_TUPLE_REM_4 dfmd)
#    define PP_REPEAT_FROM_TO_M_3(z, n, dfmd) PP_REPEAT_FROM_TO_M_3_IM(z, n, PP_TUPLE_REM_4 dfmd)
#    define PP_REPEAT_FROM_TO_M_1_IM(z, n, im) PP_REPEAT_FROM_TO_M_1_I(z, n, im)
#    define PP_REPEAT_FROM_TO_M_2_IM(z, n, im) PP_REPEAT_FROM_TO_M_2_I(z, n, im)
#    define PP_REPEAT_FROM_TO_M_3_IM(z, n, im) PP_REPEAT_FROM_TO_M_3_I(z, n, im)
# else
#    define PP_REPEAT_FROM_TO_M_1(z, n, dfmd) PP_REPEAT_FROM_TO_M_1_I(z, n, PP_TUPLE_ELEM(4, 0, dfmd), PP_TUPLE_ELEM(4, 1, dfmd), PP_TUPLE_ELEM(4, 2, dfmd), PP_TUPLE_ELEM(4, 3, dfmd))
#    define PP_REPEAT_FROM_TO_M_2(z, n, dfmd) PP_REPEAT_FROM_TO_M_2_I(z, n, PP_TUPLE_ELEM(4, 0, dfmd), PP_TUPLE_ELEM(4, 1, dfmd), PP_TUPLE_ELEM(4, 2, dfmd), PP_TUPLE_ELEM(4, 3, dfmd))
#    define PP_REPEAT_FROM_TO_M_3(z, n, dfmd) PP_REPEAT_FROM_TO_M_3_I(z, n, PP_TUPLE_ELEM(4, 0, dfmd), PP_TUPLE_ELEM(4, 1, dfmd), PP_TUPLE_ELEM(4, 2, dfmd), PP_TUPLE_ELEM(4, 3, dfmd))
# endif
#
# define PP_REPEAT_FROM_TO_M_1_I(z, n, d, f, m, dt) PP_REPEAT_FROM_TO_M_1_II(z, PP_ADD_D(d, n, f), m, dt)
# define PP_REPEAT_FROM_TO_M_2_I(z, n, d, f, m, dt) PP_REPEAT_FROM_TO_M_2_II(z, PP_ADD_D(d, n, f), m, dt)
# define PP_REPEAT_FROM_TO_M_3_I(z, n, d, f, m, dt) PP_REPEAT_FROM_TO_M_3_II(z, PP_ADD_D(d, n, f), m, dt)
#
# define PP_REPEAT_FROM_TO_M_1_II(z, n, m, dt) m(z, n, dt)
# define PP_REPEAT_FROM_TO_M_2_II(z, n, m, dt) m(z, n, dt)
# define PP_REPEAT_FROM_TO_M_3_II(z, n, m, dt) m(z, n, dt)
#
# endif
