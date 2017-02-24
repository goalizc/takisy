# ifndef PREPROCESSOR_REPETITION_ENUM_PARAMS_WITH_A_DEFAULT_HPP
# define PREPROCESSOR_REPETITION_ENUM_PARAMS_WITH_A_DEFAULT_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/facilities/intercept.hpp>
# include <takisy/preprocessor/repetition/enum_binary_params.hpp>
#
# /* PP_ENUM_PARAMS_WITH_A_DEFAULT */
#
# define PP_ENUM_PARAMS_WITH_A_DEFAULT(count, param, def) PP_ENUM_BINARY_PARAMS(count, param, = def PP_INTERCEPT)
#
# endif
