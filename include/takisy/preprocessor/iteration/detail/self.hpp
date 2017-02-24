# if !defined(PP_INDIRECT_SELF)
#    error PP_ERROR:  no indirect file to include
# endif
#
# define PP_IS_SELFISH 1
#
# include PP_INDIRECT_SELF
#
# undef PP_IS_SELFISH
# undef PP_INDIRECT_SELF
