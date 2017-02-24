# ifndef PREPROCESSOR_LIST_FOLD_RIGHT_HPP
# define PREPROCESSOR_LIST_FOLD_RIGHT_HPP
#
# include <takisy/preprocessor/cat.hpp>
# include <takisy/preprocessor/control/while.hpp>
# include <takisy/preprocessor/debug/error.hpp>
# include <takisy/preprocessor/detail/auto_rec.hpp>
#
# if 0
#    define PP_LIST_FOLD_RIGHT(op, state, list)
# endif
#
# define PP_LIST_FOLD_RIGHT PP_CAT(PP_LIST_FOLD_RIGHT_, PP_AUTO_REC(PP_WHILE_P, 256))
#
# define PP_LIST_FOLD_RIGHT_257(o, s, l) PP_ERROR(0x0004)
#
# define PP_LIST_FOLD_RIGHT_D(d, o, s, l) PP_LIST_FOLD_RIGHT_ ## d(o, s, l)
# define PP_LIST_FOLD_RIGHT_2ND PP_LIST_FOLD_RIGHT
# define PP_LIST_FOLD_RIGHT_2ND_D PP_LIST_FOLD_RIGHT_D
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#    include <takisy/preprocessor/list/detail/edg/fold_right.hpp>
# else
#    include <takisy/preprocessor/list/detail/fold_right.hpp>
# endif
#
# endif
