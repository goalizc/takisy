# ifndef PREPROCESSOR_SLOT_SLOT_HPP
# define PREPROCESSOR_SLOT_SLOT_HPP
#
# include <takisy/preprocessor/cat.hpp>
# include <takisy/preprocessor/slot/detail/def.hpp>
#
# /* PP_ASSIGN_SLOT */
#
# define PP_ASSIGN_SLOT(i) PP_CAT(PP_ASSIGN_SLOT_, i)
#
# define PP_ASSIGN_SLOT_1 <takisy/preprocessor/slot/detail/slot1.hpp>
# define PP_ASSIGN_SLOT_2 <takisy/preprocessor/slot/detail/slot2.hpp>
# define PP_ASSIGN_SLOT_3 <takisy/preprocessor/slot/detail/slot3.hpp>
# define PP_ASSIGN_SLOT_4 <takisy/preprocessor/slot/detail/slot4.hpp>
# define PP_ASSIGN_SLOT_5 <takisy/preprocessor/slot/detail/slot5.hpp>
#
# /* PP_SLOT */
#
# define PP_SLOT(i) PP_CAT(PP_SLOT_, i)()
#
# endif
