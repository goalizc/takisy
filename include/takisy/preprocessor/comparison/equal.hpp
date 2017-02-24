# ifndef PREPROCESSOR_COMPARISON_EQUAL_HPP
# define PREPROCESSOR_COMPARISON_EQUAL_HPP
#
# include <takisy/preprocessor/comparison/not_equal.hpp>
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/logical/compl.hpp>
#
# /* PP_EQUAL */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_EQUAL(x, y) PP_COMPL(PP_NOT_EQUAL(x, y))
# else
#    define PP_EQUAL(x, y) PP_EQUAL_I(x, y)
#    define PP_EQUAL_I(x, y) PP_COMPL(PP_NOT_EQUAL(x, y))
# endif
#
# /* PP_EQUAL_D */
#
# define PP_EQUAL_D(d, x, y) PP_EQUAL(x, y)
#
# endif
