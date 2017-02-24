# ifndef PREPROCESSOR_LIST_TO_SEQ_HPP
# define PREPROCESSOR_LIST_TO_SEQ_HPP
#
# include <takisy/preprocessor/list/for_each.hpp>
#
# /* PP_LIST_TO_SEQ */
#
# define PP_LIST_TO_SEQ(list) \
    PP_LIST_FOR_EACH(PP_LIST_TO_SEQ_MACRO, ~, list) \
    /**/
# define PP_LIST_TO_SEQ_MACRO(r, data, elem) (elem)
#
# /* PP_LIST_TO_SEQ_R */
#
# define PP_LIST_TO_SEQ_R(r, list) \
    PP_LIST_FOR_EACH_R(r, PP_LIST_TO_SEQ_MACRO, ~, list) \
    /**/
#
# endif /* PREPROCESSOR_LIST_TO_SEQ_HPP */
