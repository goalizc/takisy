# ifndef PREPROCESSOR_FACILITIES_IS_EMPTY_HPP
# define PREPROCESSOR_FACILITIES_IS_EMPTY_HPP
#
# include <takisy/preprocessor/config/config.hpp>
#
# if PP_VARIADICS
#
# include <takisy/preprocessor/facilities/is_empty_variadic.hpp>
#
# else
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MSVC() && ~PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
# include <takisy/preprocessor/tuple/elem.hpp>
# include <takisy/preprocessor/facilities/identity.hpp>
# else
# include <takisy/preprocessor/cat.hpp>
# include <takisy/preprocessor/detail/split.hpp>
# endif
#
# /* PP_IS_EMPTY */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_MSVC() && ~PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_IS_EMPTY(x) PP_IS_EMPTY_I(x PP_IS_EMPTY_HELPER)
#    define PP_IS_EMPTY_I(contents) PP_TUPLE_ELEM(2, 1, (PP_IS_EMPTY_DEF_ ## contents()))
#    define PP_IS_EMPTY_DEF_PP_IS_EMPTY_HELPER 1, PP_IDENTITY(1)
#    define PP_IS_EMPTY_HELPER() , 0
# else
#    if PP_CONFIG_FLAGS() & PP_CONFIG_MSVC()
#        define PP_IS_EMPTY(x) PP_IS_EMPTY_I(PP_IS_EMPTY_HELPER x ())
#        define PP_IS_EMPTY_I(test) PP_IS_EMPTY_II(PP_SPLIT(0, PP_CAT(PP_IS_EMPTY_DEF_, test)))
#        define PP_IS_EMPTY_II(id) id
#    else
#        define PP_IS_EMPTY(x) PP_IS_EMPTY_I((PP_IS_EMPTY_HELPER x ()))
#        define PP_IS_EMPTY_I(par) PP_IS_EMPTY_II ## par
#        define PP_IS_EMPTY_II(test) PP_SPLIT(0, PP_CAT(PP_IS_EMPTY_DEF_, test))
#    endif
#    define PP_IS_EMPTY_HELPER() 1
#    define PP_IS_EMPTY_DEF_1 1, PP_NIL
#    define PP_IS_EMPTY_DEF_PP_IS_EMPTY_HELPER 0, PP_NIL
# endif
#
# endif /* PP_VARIADICS */
#
# endif /* PREPROCESSOR_FACILITIES_IS_EMPTY_HPP */
