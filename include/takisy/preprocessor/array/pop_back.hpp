# ifndef PREPROCESSOR_ARRAY_POP_BACK_HPP
# define PREPROCESSOR_ARRAY_POP_BACK_HPP
#
# include <takisy/preprocessor/arithmetic/dec.hpp>
# include <takisy/preprocessor/array/elem.hpp>
# include <takisy/preprocessor/array/size.hpp>
# include <takisy/preprocessor/repetition/enum.hpp>
# include <takisy/preprocessor/repetition/deduce_z.hpp>
#
# /* PP_ARRAY_POP_BACK */
#
# define PP_ARRAY_POP_BACK(array) PP_ARRAY_POP_BACK_Z(PP_DEDUCE_Z(), array)
#
# /* PP_ARRAY_POP_BACK_Z */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_ARRAY_POP_BACK_Z(z, array) PP_ARRAY_POP_BACK_I(z, PP_ARRAY_SIZE(array), array)
# else
#    define PP_ARRAY_POP_BACK_Z(z, array) PP_ARRAY_POP_BACK_Z_D(z, array)
#    define PP_ARRAY_POP_BACK_Z_D(z, array) PP_ARRAY_POP_BACK_I(z, PP_ARRAY_SIZE(array), array)
# endif
#
# define PP_ARRAY_POP_BACK_I(z, size, array) (PP_DEC(size), (PP_ENUM_ ## z(PP_DEC(size), PP_ARRAY_POP_BACK_M, array)))
# define PP_ARRAY_POP_BACK_M(z, n, data) PP_ARRAY_ELEM(n, data)
#
# endif
