# ifndef PREPROCESSOR_REPETITION_ENUM_PARAMS_WITH_DEFAULTS_HPP
# define PREPROCESSOR_REPETITION_ENUM_PARAMS_WITH_DEFAULTS_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/repetition/enum_binary_params.hpp>
#
# /* PP_ENUM_PARAMS_WITH_DEFAULTS */
#
# define PP_ENUM_PARAMS_WITH_DEFAULTS(count, param, def) PP_ENUM_BINARY_PARAMS(count, param, = def)
#
# endif
