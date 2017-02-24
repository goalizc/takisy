# ifndef PREPROCESSOR_LIST_ENUM_HPP
# define PREPROCESSOR_LIST_ENUM_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/list/for_each_i.hpp>
# include <takisy/preprocessor/punctuation/comma_if.hpp>
#
# /* PP_LIST_ENUM */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_ENUM(list) PP_LIST_FOR_EACH_I(PP_LIST_ENUM_O, PP_NIL, list)
# else
#    define PP_LIST_ENUM(list) PP_LIST_ENUM_I(list)
#    define PP_LIST_ENUM_I(list) PP_LIST_FOR_EACH_I(PP_LIST_ENUM_O, PP_NIL, list)
# endif
#
# define PP_LIST_ENUM_O(r, _, i, elem) PP_COMMA_IF(i) elem
#
# /* PP_LIST_ENUM_R */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_ENUM_R(r, list) PP_LIST_FOR_EACH_I_R(r, PP_LIST_ENUM_O, PP_NIL, list)
# else
#    define PP_LIST_ENUM_R(r, list) PP_LIST_ENUM_R_I(r, list)
#    define PP_LIST_ENUM_R_I(r, list) PP_LIST_FOR_EACH_I_R(r, PP_LIST_ENUM_O, PP_NIL, list)
# endif
#
# endif
