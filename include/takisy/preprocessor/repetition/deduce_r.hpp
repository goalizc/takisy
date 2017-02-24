# ifndef PREPROCESSOR_REPETITION_DEDUCE_R_HPP
# define PREPROCESSOR_REPETITION_DEDUCE_R_HPP
#
# include <takisy/preprocessor/detail/auto_rec.hpp>
# include <takisy/preprocessor/repetition/for.hpp>
#
# /* PP_DEDUCE_R */
#
# define PP_DEDUCE_R() PP_AUTO_REC(PP_FOR_P, 256)
#
# endif
