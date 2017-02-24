# ifndef PREPROCESSOR_TUPLE_SIZE_HPP
# define PREPROCESSOR_TUPLE_SIZE_HPP
#
# include <takisy/preprocessor/cat.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/variadic/size.hpp>
#
# if PP_VARIADICS
#    if PP_VARIADICS_MSVC
#        define PP_TUPLE_SIZE(tuple) PP_CAT(PP_VARIADIC_SIZE tuple,)
#    else
#        define PP_TUPLE_SIZE(tuple) PP_VARIADIC_SIZE tuple
#    endif
# endif
#
# endif
