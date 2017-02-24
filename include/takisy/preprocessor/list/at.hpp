# ifndef PREPROCESSOR_LIST_AT_HPP
# define PREPROCESSOR_LIST_AT_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/list/adt.hpp>
# include <takisy/preprocessor/list/rest_n.hpp>
#
# /* PP_LIST_AT */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_AT(list, index) PP_LIST_FIRST(PP_LIST_REST_N(index, list))
# else
#    define PP_LIST_AT(list, index) PP_LIST_AT_I(list, index)
#    define PP_LIST_AT_I(list, index) PP_LIST_FIRST(PP_LIST_REST_N(index, list))
# endif
#
# /* PP_LIST_AT_D */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_AT_D(d, list, index) PP_LIST_FIRST(PP_LIST_REST_N_D(d, index, list))
# else
#    define PP_LIST_AT_D(d, list, index) PP_LIST_AT_D_I(d, list, index)
#    define PP_LIST_AT_D_I(d, list, index) PP_LIST_FIRST(PP_LIST_REST_N_D(d, index, list))
# endif
#
# endif
