# ifndef PREPROCESSOR_DEBUG_ASSERT_HPP
# define PREPROCESSOR_DEBUG_ASSERT_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/control/expr_iif.hpp>
# include <takisy/preprocessor/control/iif.hpp>
# include <takisy/preprocessor/logical/not.hpp>
# include <takisy/preprocessor/tuple/eat.hpp>
#
# /* PP_ASSERT */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_ASSERT PP_ASSERT_D
# else
#    define PP_ASSERT(cond) PP_ASSERT_D(cond)
# endif
#
# define PP_ASSERT_D(cond) PP_IIF(PP_NOT(cond), PP_ASSERT_ERROR, PP_TUPLE_EAT_1)(...)
# define PP_ASSERT_ERROR(x, y, z)
#
# /* PP_ASSERT_MSG */
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    define PP_ASSERT_MSG PP_ASSERT_MSG_D
# else
#    define PP_ASSERT_MSG(cond, msg) PP_ASSERT_MSG_D(cond, msg)
# endif
#
# define PP_ASSERT_MSG_D(cond, msg) PP_EXPR_IIF(PP_NOT(cond), msg)
#
# endif
