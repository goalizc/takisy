# ifndef PREPROCESSOR_CONTROL_DEDUCE_D_HPP
# define PREPROCESSOR_CONTROL_DEDUCE_D_HPP
#
# include <takisy/preprocessor/control/while.hpp>
# include <takisy/preprocessor/detail/auto_rec.hpp>
#
# /* PP_DEDUCE_D */
#
# define PP_DEDUCE_D() PP_AUTO_REC(PP_WHILE_P, 256)
#
# endif
