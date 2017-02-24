# ifndef PREPROCESSOR_LIST_ADT_HPP
# define PREPROCESSOR_LIST_ADT_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/detail/is_binary.hpp>
# include <takisy/preprocessor/logical/compl.hpp>
# include <takisy/preprocessor/tuple/eat.hpp>
#
# /* PP_LIST_CONS */
#
# define PP_LIST_CONS(head, tail) (head, tail)
#
# /* PP_LIST_NIL */
#
# define PP_LIST_NIL PP_NIL
#
# /* PP_LIST_FIRST */
#
# define PP_LIST_FIRST(list) PP_LIST_FIRST_D(list)
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_LIST_FIRST_D(list) PP_LIST_FIRST_I list
# else
#    define PP_LIST_FIRST_D(list) PP_LIST_FIRST_I ## list
# endif
#
# define PP_LIST_FIRST_I(head, tail) head
#
# /* PP_LIST_REST */
#
# define PP_LIST_REST(list) PP_LIST_REST_D(list)
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_LIST_REST_D(list) PP_LIST_REST_I list
# else
#    define PP_LIST_REST_D(list) PP_LIST_REST_I ## list
# endif
#
# define PP_LIST_REST_I(head, tail) tail
#
# /* PP_LIST_IS_CONS */
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_BCC()
#    define PP_LIST_IS_CONS(list) PP_LIST_IS_CONS_D(list)
#    define PP_LIST_IS_CONS_D(list) PP_LIST_IS_CONS_ ## list
#    define PP_LIST_IS_CONS_(head, tail) 1
#    define PP_LIST_IS_CONS_PP_NIL 0
# else
#    define PP_LIST_IS_CONS(list) PP_IS_BINARY(list)
# endif
#
# /* PP_LIST_IS_NIL */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_BCC()
#    define PP_LIST_IS_NIL(list) PP_COMPL(PP_IS_BINARY(list))
# else
#    define PP_LIST_IS_NIL(list) PP_COMPL(PP_LIST_IS_CONS(list))
# endif
#
# endif
