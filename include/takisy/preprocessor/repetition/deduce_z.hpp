# ifndef PREPROCESSOR_REPETITION_DEDUCE_Z_HPP
# define PREPROCESSOR_REPETITION_DEDUCE_Z_HPP
#
# include <takisy/preprocessor/detail/auto_rec.hpp>
# include <takisy/preprocessor/repetition/repeat.hpp>
#
# /* PP_DEDUCE_Z */
#
# define PP_DEDUCE_Z() PP_AUTO_REC(PP_REPEAT_P, 4)
#
# endif
