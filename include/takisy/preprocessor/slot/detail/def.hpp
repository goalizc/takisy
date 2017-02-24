# ifndef PREPROCESSOR_SLOT_DETAIL_DEF_HPP
# define PREPROCESSOR_SLOT_DETAIL_DEF_HPP
#
# /* PP_SLOT_OFFSET_x */
#
# define PP_SLOT_OFFSET_10(x) (x) % 1000000000UL
# define PP_SLOT_OFFSET_9(x) PP_SLOT_OFFSET_10(x) % 100000000UL
# define PP_SLOT_OFFSET_8(x) PP_SLOT_OFFSET_9(x) % 10000000UL
# define PP_SLOT_OFFSET_7(x) PP_SLOT_OFFSET_8(x) % 1000000UL
# define PP_SLOT_OFFSET_6(x) PP_SLOT_OFFSET_7(x) % 100000UL
# define PP_SLOT_OFFSET_5(x) PP_SLOT_OFFSET_6(x) % 10000UL
# define PP_SLOT_OFFSET_4(x) PP_SLOT_OFFSET_5(x) % 1000UL
# define PP_SLOT_OFFSET_3(x) PP_SLOT_OFFSET_4(x) % 100UL
# define PP_SLOT_OFFSET_2(x) PP_SLOT_OFFSET_3(x) % 10UL
#
# /* PP_SLOT_CC_x */
#
# define PP_SLOT_CC_2(a, b) PP_SLOT_CC_2_D(a, b)
# define PP_SLOT_CC_3(a, b, c) PP_SLOT_CC_3_D(a, b, c)
# define PP_SLOT_CC_4(a, b, c, d) PP_SLOT_CC_4_D(a, b, c, d)
# define PP_SLOT_CC_5(a, b, c, d, e) PP_SLOT_CC_5_D(a, b, c, d, e)
# define PP_SLOT_CC_6(a, b, c, d, e, f) PP_SLOT_CC_6_D(a, b, c, d, e, f)
# define PP_SLOT_CC_7(a, b, c, d, e, f, g) PP_SLOT_CC_7_D(a, b, c, d, e, f, g)
# define PP_SLOT_CC_8(a, b, c, d, e, f, g, h) PP_SLOT_CC_8_D(a, b, c, d, e, f, g, h)
# define PP_SLOT_CC_9(a, b, c, d, e, f, g, h, i) PP_SLOT_CC_9_D(a, b, c, d, e, f, g, h, i)
# define PP_SLOT_CC_10(a, b, c, d, e, f, g, h, i, j) PP_SLOT_CC_10_D(a, b, c, d, e, f, g, h, i, j)
#
# define PP_SLOT_CC_2_D(a, b) a ## b
# define PP_SLOT_CC_3_D(a, b, c) a ## b ## c
# define PP_SLOT_CC_4_D(a, b, c, d) a ## b ## c ## d
# define PP_SLOT_CC_5_D(a, b, c, d, e) a ## b ## c ## d ## e
# define PP_SLOT_CC_6_D(a, b, c, d, e, f) a ## b ## c ## d ## e ## f
# define PP_SLOT_CC_7_D(a, b, c, d, e, f, g) a ## b ## c ## d ## e ## f ## g
# define PP_SLOT_CC_8_D(a, b, c, d, e, f, g, h) a ## b ## c ## d ## e ## f ## g ## h
# define PP_SLOT_CC_9_D(a, b, c, d, e, f, g, h, i) a ## b ## c ## d ## e ## f ## g ## h ## i
# define PP_SLOT_CC_10_D(a, b, c, d, e, f, g, h, i, j) a ## b ## c ## d ## e ## f ## g ## h ## i ## j
#
# endif
