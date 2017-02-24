# ifndef PREPROCESSOR_REPETITION_ENUM_SHIFTED_HPP
# define PREPROCESSOR_REPETITION_ENUM_SHIFTED_HPP
#
# include <takisy/preprocessor/cat.hpp>
# include <takisy/preprocessor/arithmetic/dec.hpp>
# include <takisy/preprocessor/arithmetic/inc.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/debug/error.hpp>
# include <takisy/preprocessor/detail/auto_rec.hpp>
# include <takisy/preprocessor/punctuation/comma_if.hpp>
# include <takisy/preprocessor/repetition/repeat.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
# include <takisy/preprocessor/tuple/rem.hpp>
#
# /* PP_ENUM_SHIFTED */
#
# if 0
#    define PP_ENUM_SHIFTED(count, macro, data)
# endif
#
# define PP_ENUM_SHIFTED PP_CAT(PP_ENUM_SHIFTED_, PP_AUTO_REC(PP_REPEAT_P, 4))
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_ENUM_SHIFTED_1(c, m, d) PP_REPEAT_1(PP_DEC(c), PP_ENUM_SHIFTED_M_1, (m, d))
#    define PP_ENUM_SHIFTED_2(c, m, d) PP_REPEAT_2(PP_DEC(c), PP_ENUM_SHIFTED_M_2, (m, d))
#    define PP_ENUM_SHIFTED_3(c, m, d) PP_REPEAT_3(PP_DEC(c), PP_ENUM_SHIFTED_M_3, (m, d))
# else
#    define PP_ENUM_SHIFTED_1(c, m, d) PP_ENUM_SHIFTED_1_I(c, m, d)
#    define PP_ENUM_SHIFTED_2(c, m, d) PP_ENUM_SHIFTED_1_2(c, m, d)
#    define PP_ENUM_SHIFTED_3(c, m, d) PP_ENUM_SHIFTED_1_3(c, m, d)
#    define PP_ENUM_SHIFTED_1_I(c, m, d) PP_REPEAT_1(PP_DEC(c), PP_ENUM_SHIFTED_M_1, (m, d))
#    define PP_ENUM_SHIFTED_2_I(c, m, d) PP_REPEAT_2(PP_DEC(c), PP_ENUM_SHIFTED_M_2, (m, d))
#    define PP_ENUM_SHIFTED_3_I(c, m, d) PP_REPEAT_3(PP_DEC(c), PP_ENUM_SHIFTED_M_3, (m, d))
# endif
#
# define PP_ENUM_SHIFTED_4(c, m, d) PP_ERROR(0x0003)
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_STRICT()
#    define PP_ENUM_SHIFTED_M_1(z, n, md) PP_ENUM_SHIFTED_M_1_IM(z, n, PP_TUPLE_REM_2 md)
#    define PP_ENUM_SHIFTED_M_2(z, n, md) PP_ENUM_SHIFTED_M_2_IM(z, n, PP_TUPLE_REM_2 md)
#    define PP_ENUM_SHIFTED_M_3(z, n, md) PP_ENUM_SHIFTED_M_3_IM(z, n, PP_TUPLE_REM_2 md)
#    define PP_ENUM_SHIFTED_M_1_IM(z, n, im) PP_ENUM_SHIFTED_M_1_I(z, n, im)
#    define PP_ENUM_SHIFTED_M_2_IM(z, n, im) PP_ENUM_SHIFTED_M_2_I(z, n, im)
#    define PP_ENUM_SHIFTED_M_3_IM(z, n, im) PP_ENUM_SHIFTED_M_3_I(z, n, im)
# else
#    define PP_ENUM_SHIFTED_M_1(z, n, md) PP_ENUM_SHIFTED_M_1_I(z, n, PP_TUPLE_ELEM(2, 0, md), PP_TUPLE_ELEM(2, 1, md))
#    define PP_ENUM_SHIFTED_M_2(z, n, md) PP_ENUM_SHIFTED_M_2_I(z, n, PP_TUPLE_ELEM(2, 0, md), PP_TUPLE_ELEM(2, 1, md))
#    define PP_ENUM_SHIFTED_M_3(z, n, md) PP_ENUM_SHIFTED_M_3_I(z, n, PP_TUPLE_ELEM(2, 0, md), PP_TUPLE_ELEM(2, 1, md))
# endif
#
# define PP_ENUM_SHIFTED_M_1_I(z, n, m, d) PP_COMMA_IF(n) m(z, PP_INC(n), d)
# define PP_ENUM_SHIFTED_M_2_I(z, n, m, d) PP_COMMA_IF(n) m(z, PP_INC(n), d)
# define PP_ENUM_SHIFTED_M_3_I(z, n, m, d) PP_COMMA_IF(n) m(z, PP_INC(n), d)
#
# endif
