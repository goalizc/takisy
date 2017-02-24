# ifndef PREPROCESSOR_LIST_FOR_EACH_HPP
# define PREPROCESSOR_LIST_FOR_EACH_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/list/for_each_i.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
# include <takisy/preprocessor/tuple/rem.hpp>
#
# /* PP_LIST_FOR_EACH */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_FOR_EACH(macro, data, list) PP_LIST_FOR_EACH_I(PP_LIST_FOR_EACH_O, (macro, data), list)
# else
#    define PP_LIST_FOR_EACH(macro, data, list) PP_LIST_FOR_EACH_X(macro, data, list)
#    define PP_LIST_FOR_EACH_X(macro, data, list) PP_LIST_FOR_EACH_I(PP_LIST_FOR_EACH_O, (macro, data), list)
# endif
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_FOR_EACH_O(r, md, i, elem) PP_LIST_FOR_EACH_O_D(r, PP_TUPLE_ELEM(2, 0, md), PP_TUPLE_ELEM(2, 1, md), elem)
# else
#    define PP_LIST_FOR_EACH_O(r, md, i, elem) PP_LIST_FOR_EACH_O_I(r, PP_TUPLE_REM_2 md, elem)
#    define PP_LIST_FOR_EACH_O_I(r, im, elem) PP_LIST_FOR_EACH_O_D(r, im, elem)
# endif
#
# define PP_LIST_FOR_EACH_O_D(r, m, d, elem) m(r, d, elem)
#
# /* PP_LIST_FOR_EACH_R */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_LIST_FOR_EACH_R(r, macro, data, list) PP_LIST_FOR_EACH_I_R(r, PP_LIST_FOR_EACH_O, (macro, data), list)
# else
#    define PP_LIST_FOR_EACH_R(r, macro, data, list) PP_LIST_FOR_EACH_R_X(r, macro, data, list)
#    define PP_LIST_FOR_EACH_R_X(r, macro, data, list) PP_LIST_FOR_EACH_I_R(r, PP_LIST_FOR_EACH_O, (macro, data), list)
# endif
#
# endif
