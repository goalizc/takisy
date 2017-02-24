# ifndef PREPROCESSOR_REPETITION_ENUM_TRAILING_BINARY_PARAMS_HPP
# define PREPROCESSOR_REPETITION_ENUM_TRAILING_BINARY_PARAMS_HPP
#
# include <takisy/preprocessor/cat.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/repetition/repeat.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
# include <takisy/preprocessor/tuple/rem.hpp>
#
# /* PP_ENUM_TRAILING_BINARY_PARAMS */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_ENUM_TRAILING_BINARY_PARAMS(count, p1, p2) PP_REPEAT(count, PP_ENUM_TRAILING_BINARY_PARAMS_M, (p1, p2))
# else
#    define PP_ENUM_TRAILING_BINARY_PARAMS(count, p1, p2) PP_ENUM_TRAILING_BINARY_PARAMS_I(count, p1, p2)
#    define PP_ENUM_TRAILING_BINARY_PARAMS_I(count, p1, p2) PP_REPEAT(count, PP_ENUM_TRAILING_BINARY_PARAMS_M, (p1, p2))
# endif
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_STRICT()
#    define PP_ENUM_TRAILING_BINARY_PARAMS_M(z, n, pp) PP_ENUM_TRAILING_BINARY_PARAMS_M_IM(z, n, PP_TUPLE_REM_2 pp)
#    define PP_ENUM_TRAILING_BINARY_PARAMS_M_IM(z, n, im) PP_ENUM_TRAILING_BINARY_PARAMS_M_I(z, n, im)
# else
#    define PP_ENUM_TRAILING_BINARY_PARAMS_M(z, n, pp) PP_ENUM_TRAILING_BINARY_PARAMS_M_I(z, n, PP_TUPLE_ELEM(2, 0, pp), PP_TUPLE_ELEM(2, 1, pp))
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MSVC()
#    define PP_ENUM_TRAILING_BINARY_PARAMS_M_I(z, n, p1, p2) PP_ENUM_TRAILING_BINARY_PARAMS_M_II(z, n, p1, p2)
#    define PP_ENUM_TRAILING_BINARY_PARAMS_M_II(z, n, p1, p2) , p1 ## n p2 ## n
# else
#    define PP_ENUM_TRAILING_BINARY_PARAMS_M_I(z, n, p1, p2) , PP_CAT(p1, n) PP_CAT(p2, n)
# endif
#
# /* PP_ENUM_TRAILING_BINARY_PARAMS_Z */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_ENUM_TRAILING_BINARY_PARAMS_Z(z, count, p1, p2) PP_REPEAT_ ## z(count, PP_ENUM_TRAILING_BINARY_PARAMS_M, (p1, p2))
# else
#    define PP_ENUM_TRAILING_BINARY_PARAMS_Z(z, count, p1, p2) PP_ENUM_TRAILING_BINARY_PARAMS_Z_I(z, count, p1, p2)
#    define PP_ENUM_TRAILING_BINARY_PARAMS_Z_I(z, count, p1, p2) PP_REPEAT_ ## z(count, PP_ENUM_TRAILING_BINARY_PARAMS_M, (p1, p2))
# endif
#
# endif
