# include <takisy/preprocessor/slot/detail/shared.hpp>
#
# undef PP_LOCAL_FE
#
# undef PP_LOCAL_FE_DIGIT_1
# undef PP_LOCAL_FE_DIGIT_2
# undef PP_LOCAL_FE_DIGIT_3
# undef PP_LOCAL_FE_DIGIT_4
# undef PP_LOCAL_FE_DIGIT_5
# undef PP_LOCAL_FE_DIGIT_6
# undef PP_LOCAL_FE_DIGIT_7
# undef PP_LOCAL_FE_DIGIT_8
# undef PP_LOCAL_FE_DIGIT_9
# undef PP_LOCAL_FE_DIGIT_10
#
# if PP_SLOT_TEMP_3 == 0
#    define PP_LOCAL_FE_DIGIT_3 0
# elif PP_SLOT_TEMP_3 == 1
#    define PP_LOCAL_FE_DIGIT_3 1
# elif PP_SLOT_TEMP_3 == 2
#    define PP_LOCAL_FE_DIGIT_3 2
# elif PP_SLOT_TEMP_3 == 3
#    define PP_LOCAL_FE_DIGIT_3 3
# elif PP_SLOT_TEMP_3 == 4
#    define PP_LOCAL_FE_DIGIT_3 4
# elif PP_SLOT_TEMP_3 == 5
#    define PP_LOCAL_FE_DIGIT_3 5
# elif PP_SLOT_TEMP_3 == 6
#    define PP_LOCAL_FE_DIGIT_3 6
# elif PP_SLOT_TEMP_3 == 7
#    define PP_LOCAL_FE_DIGIT_3 7
# elif PP_SLOT_TEMP_3 == 8
#    define PP_LOCAL_FE_DIGIT_3 8
# elif PP_SLOT_TEMP_3 == 9
#    define PP_LOCAL_FE_DIGIT_3 9
# endif
#
# if PP_SLOT_TEMP_2 == 0
#    define PP_LOCAL_FE_DIGIT_2 0
# elif PP_SLOT_TEMP_2 == 1
#    define PP_LOCAL_FE_DIGIT_2 1
# elif PP_SLOT_TEMP_2 == 2
#    define PP_LOCAL_FE_DIGIT_2 2
# elif PP_SLOT_TEMP_2 == 3
#    define PP_LOCAL_FE_DIGIT_2 3
# elif PP_SLOT_TEMP_2 == 4
#    define PP_LOCAL_FE_DIGIT_2 4
# elif PP_SLOT_TEMP_2 == 5
#    define PP_LOCAL_FE_DIGIT_2 5
# elif PP_SLOT_TEMP_2 == 6
#    define PP_LOCAL_FE_DIGIT_2 6
# elif PP_SLOT_TEMP_2 == 7
#    define PP_LOCAL_FE_DIGIT_2 7
# elif PP_SLOT_TEMP_2 == 8
#    define PP_LOCAL_FE_DIGIT_2 8
# elif PP_SLOT_TEMP_2 == 9
#    define PP_LOCAL_FE_DIGIT_2 9
# endif
#
# if PP_SLOT_TEMP_1 == 0
#    define PP_LOCAL_FE_DIGIT_1 0
# elif PP_SLOT_TEMP_1 == 1
#    define PP_LOCAL_FE_DIGIT_1 1
# elif PP_SLOT_TEMP_1 == 2
#    define PP_LOCAL_FE_DIGIT_1 2
# elif PP_SLOT_TEMP_1 == 3
#    define PP_LOCAL_FE_DIGIT_1 3
# elif PP_SLOT_TEMP_1 == 4
#    define PP_LOCAL_FE_DIGIT_1 4
# elif PP_SLOT_TEMP_1 == 5
#    define PP_LOCAL_FE_DIGIT_1 5
# elif PP_SLOT_TEMP_1 == 6
#    define PP_LOCAL_FE_DIGIT_1 6
# elif PP_SLOT_TEMP_1 == 7
#    define PP_LOCAL_FE_DIGIT_1 7
# elif PP_SLOT_TEMP_1 == 8
#    define PP_LOCAL_FE_DIGIT_1 8
# elif PP_SLOT_TEMP_1 == 9
#    define PP_LOCAL_FE_DIGIT_1 9
# endif
#
# if PP_LOCAL_FE_DIGIT_3
#    define PP_LOCAL_FE() PP_SLOT_CC_3(PP_LOCAL_FE_DIGIT_3, PP_LOCAL_FE_DIGIT_2, PP_LOCAL_FE_DIGIT_1)
# elif PP_LOCAL_FE_DIGIT_2
#    define PP_LOCAL_FE() PP_SLOT_CC_2(PP_LOCAL_FE_DIGIT_2, PP_LOCAL_FE_DIGIT_1)
# else
#    define PP_LOCAL_FE() PP_LOCAL_FE_DIGIT_1
# endif
