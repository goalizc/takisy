# ifndef PREPROCESSOR_FACILITIES_IDENTITY_HPP
# define PREPROCESSOR_FACILITIES_IDENTITY_HPP
#
# include <takisy/preprocessor/facilities/empty.hpp>
# include <takisy/preprocessor/tuple/eat.hpp>
#
# /* PP_IDENTITY */
#
# define PP_IDENTITY(item) item PP_EMPTY
#
# define PP_IDENTITY_N(item,n) item PP_TUPLE_EAT_N(n)
#
# endif
