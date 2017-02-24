# ifndef PREPROCESSOR_REPETITION_ENUM_PARAMS_HPP
# define PREPROCESSOR_REPETITION_ENUM_PARAMS_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/punctuation/comma_if.hpp>
# include <takisy/preprocessor/repetition/repeat.hpp>
#
# /* PP_ENUM_PARAMS */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_ENUM_PARAMS(count, param) PP_REPEAT(count, PP_ENUM_PARAMS_M, param)
# else
#    define PP_ENUM_PARAMS(count, param) PP_ENUM_PARAMS_I(count, param)
#    define PP_ENUM_PARAMS_I(count, param) PP_REPEAT(count, PP_ENUM_PARAMS_M, param)
# endif
#
# define PP_ENUM_PARAMS_M(z, n, param) PP_COMMA_IF(n) param ## n
#
# /* PP_ENUM_PARAMS_Z */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_ENUM_PARAMS_Z(z, count, param) PP_REPEAT_ ## z(count, PP_ENUM_PARAMS_M, param)
# else
#    define PP_ENUM_PARAMS_Z(z, count, param) PP_ENUM_PARAMS_Z_I(z, count, param)
#    define PP_ENUM_PARAMS_Z_I(z, count, param) PP_REPEAT_ ## z(count, PP_ENUM_PARAMS_M, param)
# endif
#
# endif
