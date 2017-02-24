# ifndef PREPROCESSOR_ITERATION_LOCAL_HPP
# define PREPROCESSOR_ITERATION_LOCAL_HPP
#
# include <takisy/preprocessor/config/config.hpp>
# include <takisy/preprocessor/slot/slot.hpp>
# include <takisy/preprocessor/tuple/elem.hpp>
#
# /* PP_LOCAL_ITERATE */
#
# define PP_LOCAL_ITERATE() <takisy/preprocessor/iteration/detail/local.hpp>
#
# define PP_LOCAL_C(n) (PP_LOCAL_S) <= n && (PP_LOCAL_F) >= n
# define PP_LOCAL_R(n) (PP_LOCAL_F) <= n && (PP_LOCAL_S) >= n
#
# endif
