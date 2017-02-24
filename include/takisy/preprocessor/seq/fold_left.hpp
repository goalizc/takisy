# ifndef PREPROCESSOR_SEQ_FOLD_LEFT_HPP
# define PREPROCESSOR_SEQ_FOLD_LEFT_HPP
#
# include <takisy/preprocessor/arithmetic/dec.hpp>
# include <takisy/preprocessor/cat.hpp>
# include <takisy/preprocessor/control/if.hpp>
# include <takisy/preprocessor/debug/error.hpp>
# include <takisy/preprocessor/detail/auto_rec.hpp>
# include <takisy/preprocessor/seq/seq.hpp>
# include <takisy/preprocessor/seq/size.hpp>
#
# /* PP_SEQ_FOLD_LEFT */
#
# if 0
#    define PP_SEQ_FOLD_LEFT(op, state, seq) ...
# endif
#
# define PP_SEQ_FOLD_LEFT PP_CAT(PP_SEQ_FOLD_LEFT_, PP_AUTO_REC(PP_SEQ_FOLD_LEFT_P, 256))
# define PP_SEQ_FOLD_LEFT_P(n) PP_CAT(PP_SEQ_FOLD_LEFT_CHECK_, PP_SEQ_FOLD_LEFT_I_ ## n(PP_SEQ_FOLD_LEFT_O, PP_NIL, (nil), 1))
# define PP_SEQ_FOLD_LEFT_O(s, st, _) st
#
# define PP_SEQ_FOLD_LEFT_257(op, st, ss) PP_ERROR(0x0005)
# define PP_SEQ_FOLD_LEFT_I_257(op, st, ss, sz) PP_ERROR(0x0005)
#
# define PP_SEQ_FOLD_LEFT_CHECK_PP_NIL 1
#
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_1(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_2(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_3(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_4(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_5(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_6(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_7(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_8(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_9(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_10(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_11(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_12(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_13(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_14(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_15(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_16(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_17(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_18(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_19(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_20(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_21(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_22(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_23(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_24(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_25(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_26(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_27(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_28(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_29(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_30(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_31(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_32(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_33(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_34(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_35(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_36(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_37(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_38(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_39(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_40(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_41(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_42(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_43(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_44(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_45(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_46(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_47(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_48(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_49(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_50(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_51(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_52(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_53(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_54(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_55(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_56(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_57(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_58(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_59(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_60(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_61(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_62(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_63(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_64(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_65(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_66(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_67(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_68(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_69(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_70(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_71(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_72(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_73(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_74(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_75(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_76(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_77(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_78(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_79(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_80(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_81(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_82(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_83(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_84(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_85(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_86(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_87(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_88(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_89(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_90(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_91(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_92(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_93(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_94(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_95(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_96(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_97(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_98(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_99(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_100(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_101(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_102(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_103(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_104(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_105(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_106(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_107(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_108(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_109(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_110(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_111(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_112(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_113(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_114(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_115(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_116(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_117(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_118(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_119(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_120(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_121(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_122(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_123(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_124(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_125(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_126(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_127(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_128(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_129(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_130(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_131(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_132(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_133(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_134(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_135(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_136(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_137(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_138(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_139(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_140(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_141(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_142(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_143(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_144(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_145(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_146(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_147(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_148(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_149(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_150(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_151(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_152(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_153(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_154(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_155(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_156(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_157(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_158(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_159(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_160(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_161(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_162(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_163(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_164(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_165(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_166(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_167(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_168(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_169(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_170(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_171(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_172(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_173(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_174(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_175(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_176(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_177(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_178(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_179(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_180(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_181(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_182(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_183(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_184(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_185(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_186(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_187(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_188(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_189(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_190(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_191(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_192(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_193(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_194(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_195(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_196(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_197(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_198(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_199(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_200(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_201(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_202(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_203(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_204(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_205(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_206(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_207(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_208(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_209(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_210(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_211(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_212(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_213(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_214(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_215(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_216(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_217(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_218(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_219(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_220(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_221(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_222(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_223(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_224(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_225(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_226(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_227(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_228(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_229(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_230(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_231(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_232(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_233(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_234(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_235(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_236(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_237(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_238(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_239(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_240(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_241(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_242(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_243(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_244(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_245(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_246(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_247(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_248(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_249(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_250(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_251(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_252(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_253(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_254(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_255(op, st, ss, sz) 0
# define PP_SEQ_FOLD_LEFT_CHECK_PP_SEQ_FOLD_LEFT_I_256(op, st, ss, sz) 0
#
# define PP_SEQ_FOLD_LEFT_F(op, st, ss, sz) st
#
# define PP_SEQ_FOLD_LEFT_1(op, st, ss) PP_SEQ_FOLD_LEFT_I_1(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_2(op, st, ss) PP_SEQ_FOLD_LEFT_I_2(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_3(op, st, ss) PP_SEQ_FOLD_LEFT_I_3(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_4(op, st, ss) PP_SEQ_FOLD_LEFT_I_4(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_5(op, st, ss) PP_SEQ_FOLD_LEFT_I_5(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_6(op, st, ss) PP_SEQ_FOLD_LEFT_I_6(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_7(op, st, ss) PP_SEQ_FOLD_LEFT_I_7(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_8(op, st, ss) PP_SEQ_FOLD_LEFT_I_8(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_9(op, st, ss) PP_SEQ_FOLD_LEFT_I_9(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_10(op, st, ss) PP_SEQ_FOLD_LEFT_I_10(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_11(op, st, ss) PP_SEQ_FOLD_LEFT_I_11(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_12(op, st, ss) PP_SEQ_FOLD_LEFT_I_12(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_13(op, st, ss) PP_SEQ_FOLD_LEFT_I_13(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_14(op, st, ss) PP_SEQ_FOLD_LEFT_I_14(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_15(op, st, ss) PP_SEQ_FOLD_LEFT_I_15(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_16(op, st, ss) PP_SEQ_FOLD_LEFT_I_16(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_17(op, st, ss) PP_SEQ_FOLD_LEFT_I_17(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_18(op, st, ss) PP_SEQ_FOLD_LEFT_I_18(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_19(op, st, ss) PP_SEQ_FOLD_LEFT_I_19(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_20(op, st, ss) PP_SEQ_FOLD_LEFT_I_20(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_21(op, st, ss) PP_SEQ_FOLD_LEFT_I_21(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_22(op, st, ss) PP_SEQ_FOLD_LEFT_I_22(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_23(op, st, ss) PP_SEQ_FOLD_LEFT_I_23(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_24(op, st, ss) PP_SEQ_FOLD_LEFT_I_24(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_25(op, st, ss) PP_SEQ_FOLD_LEFT_I_25(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_26(op, st, ss) PP_SEQ_FOLD_LEFT_I_26(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_27(op, st, ss) PP_SEQ_FOLD_LEFT_I_27(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_28(op, st, ss) PP_SEQ_FOLD_LEFT_I_28(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_29(op, st, ss) PP_SEQ_FOLD_LEFT_I_29(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_30(op, st, ss) PP_SEQ_FOLD_LEFT_I_30(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_31(op, st, ss) PP_SEQ_FOLD_LEFT_I_31(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_32(op, st, ss) PP_SEQ_FOLD_LEFT_I_32(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_33(op, st, ss) PP_SEQ_FOLD_LEFT_I_33(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_34(op, st, ss) PP_SEQ_FOLD_LEFT_I_34(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_35(op, st, ss) PP_SEQ_FOLD_LEFT_I_35(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_36(op, st, ss) PP_SEQ_FOLD_LEFT_I_36(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_37(op, st, ss) PP_SEQ_FOLD_LEFT_I_37(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_38(op, st, ss) PP_SEQ_FOLD_LEFT_I_38(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_39(op, st, ss) PP_SEQ_FOLD_LEFT_I_39(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_40(op, st, ss) PP_SEQ_FOLD_LEFT_I_40(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_41(op, st, ss) PP_SEQ_FOLD_LEFT_I_41(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_42(op, st, ss) PP_SEQ_FOLD_LEFT_I_42(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_43(op, st, ss) PP_SEQ_FOLD_LEFT_I_43(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_44(op, st, ss) PP_SEQ_FOLD_LEFT_I_44(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_45(op, st, ss) PP_SEQ_FOLD_LEFT_I_45(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_46(op, st, ss) PP_SEQ_FOLD_LEFT_I_46(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_47(op, st, ss) PP_SEQ_FOLD_LEFT_I_47(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_48(op, st, ss) PP_SEQ_FOLD_LEFT_I_48(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_49(op, st, ss) PP_SEQ_FOLD_LEFT_I_49(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_50(op, st, ss) PP_SEQ_FOLD_LEFT_I_50(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_51(op, st, ss) PP_SEQ_FOLD_LEFT_I_51(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_52(op, st, ss) PP_SEQ_FOLD_LEFT_I_52(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_53(op, st, ss) PP_SEQ_FOLD_LEFT_I_53(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_54(op, st, ss) PP_SEQ_FOLD_LEFT_I_54(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_55(op, st, ss) PP_SEQ_FOLD_LEFT_I_55(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_56(op, st, ss) PP_SEQ_FOLD_LEFT_I_56(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_57(op, st, ss) PP_SEQ_FOLD_LEFT_I_57(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_58(op, st, ss) PP_SEQ_FOLD_LEFT_I_58(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_59(op, st, ss) PP_SEQ_FOLD_LEFT_I_59(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_60(op, st, ss) PP_SEQ_FOLD_LEFT_I_60(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_61(op, st, ss) PP_SEQ_FOLD_LEFT_I_61(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_62(op, st, ss) PP_SEQ_FOLD_LEFT_I_62(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_63(op, st, ss) PP_SEQ_FOLD_LEFT_I_63(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_64(op, st, ss) PP_SEQ_FOLD_LEFT_I_64(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_65(op, st, ss) PP_SEQ_FOLD_LEFT_I_65(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_66(op, st, ss) PP_SEQ_FOLD_LEFT_I_66(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_67(op, st, ss) PP_SEQ_FOLD_LEFT_I_67(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_68(op, st, ss) PP_SEQ_FOLD_LEFT_I_68(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_69(op, st, ss) PP_SEQ_FOLD_LEFT_I_69(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_70(op, st, ss) PP_SEQ_FOLD_LEFT_I_70(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_71(op, st, ss) PP_SEQ_FOLD_LEFT_I_71(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_72(op, st, ss) PP_SEQ_FOLD_LEFT_I_72(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_73(op, st, ss) PP_SEQ_FOLD_LEFT_I_73(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_74(op, st, ss) PP_SEQ_FOLD_LEFT_I_74(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_75(op, st, ss) PP_SEQ_FOLD_LEFT_I_75(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_76(op, st, ss) PP_SEQ_FOLD_LEFT_I_76(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_77(op, st, ss) PP_SEQ_FOLD_LEFT_I_77(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_78(op, st, ss) PP_SEQ_FOLD_LEFT_I_78(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_79(op, st, ss) PP_SEQ_FOLD_LEFT_I_79(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_80(op, st, ss) PP_SEQ_FOLD_LEFT_I_80(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_81(op, st, ss) PP_SEQ_FOLD_LEFT_I_81(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_82(op, st, ss) PP_SEQ_FOLD_LEFT_I_82(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_83(op, st, ss) PP_SEQ_FOLD_LEFT_I_83(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_84(op, st, ss) PP_SEQ_FOLD_LEFT_I_84(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_85(op, st, ss) PP_SEQ_FOLD_LEFT_I_85(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_86(op, st, ss) PP_SEQ_FOLD_LEFT_I_86(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_87(op, st, ss) PP_SEQ_FOLD_LEFT_I_87(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_88(op, st, ss) PP_SEQ_FOLD_LEFT_I_88(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_89(op, st, ss) PP_SEQ_FOLD_LEFT_I_89(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_90(op, st, ss) PP_SEQ_FOLD_LEFT_I_90(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_91(op, st, ss) PP_SEQ_FOLD_LEFT_I_91(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_92(op, st, ss) PP_SEQ_FOLD_LEFT_I_92(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_93(op, st, ss) PP_SEQ_FOLD_LEFT_I_93(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_94(op, st, ss) PP_SEQ_FOLD_LEFT_I_94(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_95(op, st, ss) PP_SEQ_FOLD_LEFT_I_95(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_96(op, st, ss) PP_SEQ_FOLD_LEFT_I_96(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_97(op, st, ss) PP_SEQ_FOLD_LEFT_I_97(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_98(op, st, ss) PP_SEQ_FOLD_LEFT_I_98(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_99(op, st, ss) PP_SEQ_FOLD_LEFT_I_99(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_100(op, st, ss) PP_SEQ_FOLD_LEFT_I_100(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_101(op, st, ss) PP_SEQ_FOLD_LEFT_I_101(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_102(op, st, ss) PP_SEQ_FOLD_LEFT_I_102(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_103(op, st, ss) PP_SEQ_FOLD_LEFT_I_103(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_104(op, st, ss) PP_SEQ_FOLD_LEFT_I_104(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_105(op, st, ss) PP_SEQ_FOLD_LEFT_I_105(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_106(op, st, ss) PP_SEQ_FOLD_LEFT_I_106(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_107(op, st, ss) PP_SEQ_FOLD_LEFT_I_107(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_108(op, st, ss) PP_SEQ_FOLD_LEFT_I_108(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_109(op, st, ss) PP_SEQ_FOLD_LEFT_I_109(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_110(op, st, ss) PP_SEQ_FOLD_LEFT_I_110(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_111(op, st, ss) PP_SEQ_FOLD_LEFT_I_111(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_112(op, st, ss) PP_SEQ_FOLD_LEFT_I_112(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_113(op, st, ss) PP_SEQ_FOLD_LEFT_I_113(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_114(op, st, ss) PP_SEQ_FOLD_LEFT_I_114(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_115(op, st, ss) PP_SEQ_FOLD_LEFT_I_115(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_116(op, st, ss) PP_SEQ_FOLD_LEFT_I_116(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_117(op, st, ss) PP_SEQ_FOLD_LEFT_I_117(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_118(op, st, ss) PP_SEQ_FOLD_LEFT_I_118(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_119(op, st, ss) PP_SEQ_FOLD_LEFT_I_119(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_120(op, st, ss) PP_SEQ_FOLD_LEFT_I_120(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_121(op, st, ss) PP_SEQ_FOLD_LEFT_I_121(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_122(op, st, ss) PP_SEQ_FOLD_LEFT_I_122(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_123(op, st, ss) PP_SEQ_FOLD_LEFT_I_123(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_124(op, st, ss) PP_SEQ_FOLD_LEFT_I_124(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_125(op, st, ss) PP_SEQ_FOLD_LEFT_I_125(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_126(op, st, ss) PP_SEQ_FOLD_LEFT_I_126(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_127(op, st, ss) PP_SEQ_FOLD_LEFT_I_127(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_128(op, st, ss) PP_SEQ_FOLD_LEFT_I_128(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_129(op, st, ss) PP_SEQ_FOLD_LEFT_I_129(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_130(op, st, ss) PP_SEQ_FOLD_LEFT_I_130(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_131(op, st, ss) PP_SEQ_FOLD_LEFT_I_131(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_132(op, st, ss) PP_SEQ_FOLD_LEFT_I_132(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_133(op, st, ss) PP_SEQ_FOLD_LEFT_I_133(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_134(op, st, ss) PP_SEQ_FOLD_LEFT_I_134(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_135(op, st, ss) PP_SEQ_FOLD_LEFT_I_135(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_136(op, st, ss) PP_SEQ_FOLD_LEFT_I_136(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_137(op, st, ss) PP_SEQ_FOLD_LEFT_I_137(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_138(op, st, ss) PP_SEQ_FOLD_LEFT_I_138(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_139(op, st, ss) PP_SEQ_FOLD_LEFT_I_139(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_140(op, st, ss) PP_SEQ_FOLD_LEFT_I_140(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_141(op, st, ss) PP_SEQ_FOLD_LEFT_I_141(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_142(op, st, ss) PP_SEQ_FOLD_LEFT_I_142(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_143(op, st, ss) PP_SEQ_FOLD_LEFT_I_143(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_144(op, st, ss) PP_SEQ_FOLD_LEFT_I_144(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_145(op, st, ss) PP_SEQ_FOLD_LEFT_I_145(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_146(op, st, ss) PP_SEQ_FOLD_LEFT_I_146(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_147(op, st, ss) PP_SEQ_FOLD_LEFT_I_147(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_148(op, st, ss) PP_SEQ_FOLD_LEFT_I_148(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_149(op, st, ss) PP_SEQ_FOLD_LEFT_I_149(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_150(op, st, ss) PP_SEQ_FOLD_LEFT_I_150(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_151(op, st, ss) PP_SEQ_FOLD_LEFT_I_151(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_152(op, st, ss) PP_SEQ_FOLD_LEFT_I_152(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_153(op, st, ss) PP_SEQ_FOLD_LEFT_I_153(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_154(op, st, ss) PP_SEQ_FOLD_LEFT_I_154(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_155(op, st, ss) PP_SEQ_FOLD_LEFT_I_155(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_156(op, st, ss) PP_SEQ_FOLD_LEFT_I_156(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_157(op, st, ss) PP_SEQ_FOLD_LEFT_I_157(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_158(op, st, ss) PP_SEQ_FOLD_LEFT_I_158(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_159(op, st, ss) PP_SEQ_FOLD_LEFT_I_159(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_160(op, st, ss) PP_SEQ_FOLD_LEFT_I_160(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_161(op, st, ss) PP_SEQ_FOLD_LEFT_I_161(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_162(op, st, ss) PP_SEQ_FOLD_LEFT_I_162(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_163(op, st, ss) PP_SEQ_FOLD_LEFT_I_163(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_164(op, st, ss) PP_SEQ_FOLD_LEFT_I_164(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_165(op, st, ss) PP_SEQ_FOLD_LEFT_I_165(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_166(op, st, ss) PP_SEQ_FOLD_LEFT_I_166(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_167(op, st, ss) PP_SEQ_FOLD_LEFT_I_167(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_168(op, st, ss) PP_SEQ_FOLD_LEFT_I_168(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_169(op, st, ss) PP_SEQ_FOLD_LEFT_I_169(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_170(op, st, ss) PP_SEQ_FOLD_LEFT_I_170(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_171(op, st, ss) PP_SEQ_FOLD_LEFT_I_171(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_172(op, st, ss) PP_SEQ_FOLD_LEFT_I_172(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_173(op, st, ss) PP_SEQ_FOLD_LEFT_I_173(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_174(op, st, ss) PP_SEQ_FOLD_LEFT_I_174(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_175(op, st, ss) PP_SEQ_FOLD_LEFT_I_175(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_176(op, st, ss) PP_SEQ_FOLD_LEFT_I_176(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_177(op, st, ss) PP_SEQ_FOLD_LEFT_I_177(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_178(op, st, ss) PP_SEQ_FOLD_LEFT_I_178(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_179(op, st, ss) PP_SEQ_FOLD_LEFT_I_179(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_180(op, st, ss) PP_SEQ_FOLD_LEFT_I_180(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_181(op, st, ss) PP_SEQ_FOLD_LEFT_I_181(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_182(op, st, ss) PP_SEQ_FOLD_LEFT_I_182(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_183(op, st, ss) PP_SEQ_FOLD_LEFT_I_183(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_184(op, st, ss) PP_SEQ_FOLD_LEFT_I_184(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_185(op, st, ss) PP_SEQ_FOLD_LEFT_I_185(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_186(op, st, ss) PP_SEQ_FOLD_LEFT_I_186(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_187(op, st, ss) PP_SEQ_FOLD_LEFT_I_187(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_188(op, st, ss) PP_SEQ_FOLD_LEFT_I_188(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_189(op, st, ss) PP_SEQ_FOLD_LEFT_I_189(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_190(op, st, ss) PP_SEQ_FOLD_LEFT_I_190(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_191(op, st, ss) PP_SEQ_FOLD_LEFT_I_191(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_192(op, st, ss) PP_SEQ_FOLD_LEFT_I_192(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_193(op, st, ss) PP_SEQ_FOLD_LEFT_I_193(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_194(op, st, ss) PP_SEQ_FOLD_LEFT_I_194(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_195(op, st, ss) PP_SEQ_FOLD_LEFT_I_195(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_196(op, st, ss) PP_SEQ_FOLD_LEFT_I_196(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_197(op, st, ss) PP_SEQ_FOLD_LEFT_I_197(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_198(op, st, ss) PP_SEQ_FOLD_LEFT_I_198(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_199(op, st, ss) PP_SEQ_FOLD_LEFT_I_199(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_200(op, st, ss) PP_SEQ_FOLD_LEFT_I_200(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_201(op, st, ss) PP_SEQ_FOLD_LEFT_I_201(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_202(op, st, ss) PP_SEQ_FOLD_LEFT_I_202(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_203(op, st, ss) PP_SEQ_FOLD_LEFT_I_203(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_204(op, st, ss) PP_SEQ_FOLD_LEFT_I_204(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_205(op, st, ss) PP_SEQ_FOLD_LEFT_I_205(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_206(op, st, ss) PP_SEQ_FOLD_LEFT_I_206(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_207(op, st, ss) PP_SEQ_FOLD_LEFT_I_207(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_208(op, st, ss) PP_SEQ_FOLD_LEFT_I_208(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_209(op, st, ss) PP_SEQ_FOLD_LEFT_I_209(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_210(op, st, ss) PP_SEQ_FOLD_LEFT_I_210(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_211(op, st, ss) PP_SEQ_FOLD_LEFT_I_211(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_212(op, st, ss) PP_SEQ_FOLD_LEFT_I_212(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_213(op, st, ss) PP_SEQ_FOLD_LEFT_I_213(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_214(op, st, ss) PP_SEQ_FOLD_LEFT_I_214(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_215(op, st, ss) PP_SEQ_FOLD_LEFT_I_215(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_216(op, st, ss) PP_SEQ_FOLD_LEFT_I_216(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_217(op, st, ss) PP_SEQ_FOLD_LEFT_I_217(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_218(op, st, ss) PP_SEQ_FOLD_LEFT_I_218(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_219(op, st, ss) PP_SEQ_FOLD_LEFT_I_219(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_220(op, st, ss) PP_SEQ_FOLD_LEFT_I_220(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_221(op, st, ss) PP_SEQ_FOLD_LEFT_I_221(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_222(op, st, ss) PP_SEQ_FOLD_LEFT_I_222(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_223(op, st, ss) PP_SEQ_FOLD_LEFT_I_223(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_224(op, st, ss) PP_SEQ_FOLD_LEFT_I_224(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_225(op, st, ss) PP_SEQ_FOLD_LEFT_I_225(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_226(op, st, ss) PP_SEQ_FOLD_LEFT_I_226(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_227(op, st, ss) PP_SEQ_FOLD_LEFT_I_227(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_228(op, st, ss) PP_SEQ_FOLD_LEFT_I_228(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_229(op, st, ss) PP_SEQ_FOLD_LEFT_I_229(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_230(op, st, ss) PP_SEQ_FOLD_LEFT_I_230(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_231(op, st, ss) PP_SEQ_FOLD_LEFT_I_231(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_232(op, st, ss) PP_SEQ_FOLD_LEFT_I_232(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_233(op, st, ss) PP_SEQ_FOLD_LEFT_I_233(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_234(op, st, ss) PP_SEQ_FOLD_LEFT_I_234(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_235(op, st, ss) PP_SEQ_FOLD_LEFT_I_235(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_236(op, st, ss) PP_SEQ_FOLD_LEFT_I_236(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_237(op, st, ss) PP_SEQ_FOLD_LEFT_I_237(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_238(op, st, ss) PP_SEQ_FOLD_LEFT_I_238(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_239(op, st, ss) PP_SEQ_FOLD_LEFT_I_239(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_240(op, st, ss) PP_SEQ_FOLD_LEFT_I_240(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_241(op, st, ss) PP_SEQ_FOLD_LEFT_I_241(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_242(op, st, ss) PP_SEQ_FOLD_LEFT_I_242(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_243(op, st, ss) PP_SEQ_FOLD_LEFT_I_243(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_244(op, st, ss) PP_SEQ_FOLD_LEFT_I_244(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_245(op, st, ss) PP_SEQ_FOLD_LEFT_I_245(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_246(op, st, ss) PP_SEQ_FOLD_LEFT_I_246(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_247(op, st, ss) PP_SEQ_FOLD_LEFT_I_247(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_248(op, st, ss) PP_SEQ_FOLD_LEFT_I_248(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_249(op, st, ss) PP_SEQ_FOLD_LEFT_I_249(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_250(op, st, ss) PP_SEQ_FOLD_LEFT_I_250(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_251(op, st, ss) PP_SEQ_FOLD_LEFT_I_251(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_252(op, st, ss) PP_SEQ_FOLD_LEFT_I_252(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_253(op, st, ss) PP_SEQ_FOLD_LEFT_I_253(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_254(op, st, ss) PP_SEQ_FOLD_LEFT_I_254(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_255(op, st, ss) PP_SEQ_FOLD_LEFT_I_255(op, st, ss, PP_SEQ_SIZE(ss))
# define PP_SEQ_FOLD_LEFT_256(op, st, ss) PP_SEQ_FOLD_LEFT_I_256(op, st, ss, PP_SEQ_SIZE(ss))
#
# if ~PP_CONFIG_FLAGS() & PP_CONFIG_DMC()
#    define PP_SEQ_FOLD_LEFT_I_1(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_2, PP_SEQ_FOLD_LEFT_F)(op, op(2, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_2(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_3, PP_SEQ_FOLD_LEFT_F)(op, op(3, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_3(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_4, PP_SEQ_FOLD_LEFT_F)(op, op(4, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_4(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_5, PP_SEQ_FOLD_LEFT_F)(op, op(5, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_5(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_6, PP_SEQ_FOLD_LEFT_F)(op, op(6, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_6(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_7, PP_SEQ_FOLD_LEFT_F)(op, op(7, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_7(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_8, PP_SEQ_FOLD_LEFT_F)(op, op(8, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_8(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_9, PP_SEQ_FOLD_LEFT_F)(op, op(9, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_9(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_10, PP_SEQ_FOLD_LEFT_F)(op, op(10, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_10(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_11, PP_SEQ_FOLD_LEFT_F)(op, op(11, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_11(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_12, PP_SEQ_FOLD_LEFT_F)(op, op(12, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_12(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_13, PP_SEQ_FOLD_LEFT_F)(op, op(13, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_13(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_14, PP_SEQ_FOLD_LEFT_F)(op, op(14, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_14(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_15, PP_SEQ_FOLD_LEFT_F)(op, op(15, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_15(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_16, PP_SEQ_FOLD_LEFT_F)(op, op(16, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_16(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_17, PP_SEQ_FOLD_LEFT_F)(op, op(17, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_17(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_18, PP_SEQ_FOLD_LEFT_F)(op, op(18, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_18(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_19, PP_SEQ_FOLD_LEFT_F)(op, op(19, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_19(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_20, PP_SEQ_FOLD_LEFT_F)(op, op(20, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_20(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_21, PP_SEQ_FOLD_LEFT_F)(op, op(21, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_21(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_22, PP_SEQ_FOLD_LEFT_F)(op, op(22, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_22(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_23, PP_SEQ_FOLD_LEFT_F)(op, op(23, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_23(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_24, PP_SEQ_FOLD_LEFT_F)(op, op(24, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_24(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_25, PP_SEQ_FOLD_LEFT_F)(op, op(25, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_25(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_26, PP_SEQ_FOLD_LEFT_F)(op, op(26, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_26(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_27, PP_SEQ_FOLD_LEFT_F)(op, op(27, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_27(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_28, PP_SEQ_FOLD_LEFT_F)(op, op(28, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_28(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_29, PP_SEQ_FOLD_LEFT_F)(op, op(29, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_29(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_30, PP_SEQ_FOLD_LEFT_F)(op, op(30, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_30(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_31, PP_SEQ_FOLD_LEFT_F)(op, op(31, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_31(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_32, PP_SEQ_FOLD_LEFT_F)(op, op(32, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_32(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_33, PP_SEQ_FOLD_LEFT_F)(op, op(33, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_33(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_34, PP_SEQ_FOLD_LEFT_F)(op, op(34, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_34(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_35, PP_SEQ_FOLD_LEFT_F)(op, op(35, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_35(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_36, PP_SEQ_FOLD_LEFT_F)(op, op(36, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_36(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_37, PP_SEQ_FOLD_LEFT_F)(op, op(37, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_37(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_38, PP_SEQ_FOLD_LEFT_F)(op, op(38, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_38(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_39, PP_SEQ_FOLD_LEFT_F)(op, op(39, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_39(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_40, PP_SEQ_FOLD_LEFT_F)(op, op(40, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_40(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_41, PP_SEQ_FOLD_LEFT_F)(op, op(41, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_41(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_42, PP_SEQ_FOLD_LEFT_F)(op, op(42, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_42(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_43, PP_SEQ_FOLD_LEFT_F)(op, op(43, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_43(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_44, PP_SEQ_FOLD_LEFT_F)(op, op(44, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_44(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_45, PP_SEQ_FOLD_LEFT_F)(op, op(45, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_45(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_46, PP_SEQ_FOLD_LEFT_F)(op, op(46, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_46(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_47, PP_SEQ_FOLD_LEFT_F)(op, op(47, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_47(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_48, PP_SEQ_FOLD_LEFT_F)(op, op(48, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_48(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_49, PP_SEQ_FOLD_LEFT_F)(op, op(49, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_49(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_50, PP_SEQ_FOLD_LEFT_F)(op, op(50, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_50(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_51, PP_SEQ_FOLD_LEFT_F)(op, op(51, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_51(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_52, PP_SEQ_FOLD_LEFT_F)(op, op(52, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_52(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_53, PP_SEQ_FOLD_LEFT_F)(op, op(53, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_53(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_54, PP_SEQ_FOLD_LEFT_F)(op, op(54, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_54(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_55, PP_SEQ_FOLD_LEFT_F)(op, op(55, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_55(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_56, PP_SEQ_FOLD_LEFT_F)(op, op(56, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_56(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_57, PP_SEQ_FOLD_LEFT_F)(op, op(57, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_57(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_58, PP_SEQ_FOLD_LEFT_F)(op, op(58, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_58(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_59, PP_SEQ_FOLD_LEFT_F)(op, op(59, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_59(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_60, PP_SEQ_FOLD_LEFT_F)(op, op(60, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_60(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_61, PP_SEQ_FOLD_LEFT_F)(op, op(61, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_61(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_62, PP_SEQ_FOLD_LEFT_F)(op, op(62, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_62(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_63, PP_SEQ_FOLD_LEFT_F)(op, op(63, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_63(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_64, PP_SEQ_FOLD_LEFT_F)(op, op(64, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_64(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_65, PP_SEQ_FOLD_LEFT_F)(op, op(65, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_65(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_66, PP_SEQ_FOLD_LEFT_F)(op, op(66, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_66(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_67, PP_SEQ_FOLD_LEFT_F)(op, op(67, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_67(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_68, PP_SEQ_FOLD_LEFT_F)(op, op(68, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_68(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_69, PP_SEQ_FOLD_LEFT_F)(op, op(69, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_69(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_70, PP_SEQ_FOLD_LEFT_F)(op, op(70, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_70(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_71, PP_SEQ_FOLD_LEFT_F)(op, op(71, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_71(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_72, PP_SEQ_FOLD_LEFT_F)(op, op(72, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_72(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_73, PP_SEQ_FOLD_LEFT_F)(op, op(73, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_73(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_74, PP_SEQ_FOLD_LEFT_F)(op, op(74, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_74(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_75, PP_SEQ_FOLD_LEFT_F)(op, op(75, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_75(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_76, PP_SEQ_FOLD_LEFT_F)(op, op(76, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_76(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_77, PP_SEQ_FOLD_LEFT_F)(op, op(77, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_77(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_78, PP_SEQ_FOLD_LEFT_F)(op, op(78, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_78(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_79, PP_SEQ_FOLD_LEFT_F)(op, op(79, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_79(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_80, PP_SEQ_FOLD_LEFT_F)(op, op(80, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_80(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_81, PP_SEQ_FOLD_LEFT_F)(op, op(81, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_81(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_82, PP_SEQ_FOLD_LEFT_F)(op, op(82, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_82(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_83, PP_SEQ_FOLD_LEFT_F)(op, op(83, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_83(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_84, PP_SEQ_FOLD_LEFT_F)(op, op(84, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_84(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_85, PP_SEQ_FOLD_LEFT_F)(op, op(85, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_85(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_86, PP_SEQ_FOLD_LEFT_F)(op, op(86, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_86(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_87, PP_SEQ_FOLD_LEFT_F)(op, op(87, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_87(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_88, PP_SEQ_FOLD_LEFT_F)(op, op(88, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_88(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_89, PP_SEQ_FOLD_LEFT_F)(op, op(89, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_89(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_90, PP_SEQ_FOLD_LEFT_F)(op, op(90, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_90(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_91, PP_SEQ_FOLD_LEFT_F)(op, op(91, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_91(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_92, PP_SEQ_FOLD_LEFT_F)(op, op(92, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_92(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_93, PP_SEQ_FOLD_LEFT_F)(op, op(93, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_93(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_94, PP_SEQ_FOLD_LEFT_F)(op, op(94, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_94(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_95, PP_SEQ_FOLD_LEFT_F)(op, op(95, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_95(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_96, PP_SEQ_FOLD_LEFT_F)(op, op(96, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_96(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_97, PP_SEQ_FOLD_LEFT_F)(op, op(97, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_97(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_98, PP_SEQ_FOLD_LEFT_F)(op, op(98, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_98(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_99, PP_SEQ_FOLD_LEFT_F)(op, op(99, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_99(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_100, PP_SEQ_FOLD_LEFT_F)(op, op(100, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_100(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_101, PP_SEQ_FOLD_LEFT_F)(op, op(101, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_101(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_102, PP_SEQ_FOLD_LEFT_F)(op, op(102, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_102(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_103, PP_SEQ_FOLD_LEFT_F)(op, op(103, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_103(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_104, PP_SEQ_FOLD_LEFT_F)(op, op(104, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_104(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_105, PP_SEQ_FOLD_LEFT_F)(op, op(105, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_105(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_106, PP_SEQ_FOLD_LEFT_F)(op, op(106, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_106(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_107, PP_SEQ_FOLD_LEFT_F)(op, op(107, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_107(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_108, PP_SEQ_FOLD_LEFT_F)(op, op(108, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_108(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_109, PP_SEQ_FOLD_LEFT_F)(op, op(109, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_109(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_110, PP_SEQ_FOLD_LEFT_F)(op, op(110, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_110(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_111, PP_SEQ_FOLD_LEFT_F)(op, op(111, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_111(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_112, PP_SEQ_FOLD_LEFT_F)(op, op(112, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_112(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_113, PP_SEQ_FOLD_LEFT_F)(op, op(113, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_113(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_114, PP_SEQ_FOLD_LEFT_F)(op, op(114, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_114(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_115, PP_SEQ_FOLD_LEFT_F)(op, op(115, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_115(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_116, PP_SEQ_FOLD_LEFT_F)(op, op(116, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_116(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_117, PP_SEQ_FOLD_LEFT_F)(op, op(117, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_117(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_118, PP_SEQ_FOLD_LEFT_F)(op, op(118, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_118(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_119, PP_SEQ_FOLD_LEFT_F)(op, op(119, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_119(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_120, PP_SEQ_FOLD_LEFT_F)(op, op(120, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_120(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_121, PP_SEQ_FOLD_LEFT_F)(op, op(121, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_121(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_122, PP_SEQ_FOLD_LEFT_F)(op, op(122, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_122(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_123, PP_SEQ_FOLD_LEFT_F)(op, op(123, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_123(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_124, PP_SEQ_FOLD_LEFT_F)(op, op(124, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_124(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_125, PP_SEQ_FOLD_LEFT_F)(op, op(125, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_125(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_126, PP_SEQ_FOLD_LEFT_F)(op, op(126, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_126(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_127, PP_SEQ_FOLD_LEFT_F)(op, op(127, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_127(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_128, PP_SEQ_FOLD_LEFT_F)(op, op(128, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_128(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_129, PP_SEQ_FOLD_LEFT_F)(op, op(129, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_129(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_130, PP_SEQ_FOLD_LEFT_F)(op, op(130, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_130(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_131, PP_SEQ_FOLD_LEFT_F)(op, op(131, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_131(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_132, PP_SEQ_FOLD_LEFT_F)(op, op(132, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_132(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_133, PP_SEQ_FOLD_LEFT_F)(op, op(133, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_133(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_134, PP_SEQ_FOLD_LEFT_F)(op, op(134, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_134(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_135, PP_SEQ_FOLD_LEFT_F)(op, op(135, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_135(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_136, PP_SEQ_FOLD_LEFT_F)(op, op(136, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_136(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_137, PP_SEQ_FOLD_LEFT_F)(op, op(137, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_137(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_138, PP_SEQ_FOLD_LEFT_F)(op, op(138, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_138(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_139, PP_SEQ_FOLD_LEFT_F)(op, op(139, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_139(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_140, PP_SEQ_FOLD_LEFT_F)(op, op(140, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_140(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_141, PP_SEQ_FOLD_LEFT_F)(op, op(141, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_141(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_142, PP_SEQ_FOLD_LEFT_F)(op, op(142, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_142(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_143, PP_SEQ_FOLD_LEFT_F)(op, op(143, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_143(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_144, PP_SEQ_FOLD_LEFT_F)(op, op(144, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_144(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_145, PP_SEQ_FOLD_LEFT_F)(op, op(145, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_145(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_146, PP_SEQ_FOLD_LEFT_F)(op, op(146, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_146(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_147, PP_SEQ_FOLD_LEFT_F)(op, op(147, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_147(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_148, PP_SEQ_FOLD_LEFT_F)(op, op(148, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_148(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_149, PP_SEQ_FOLD_LEFT_F)(op, op(149, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_149(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_150, PP_SEQ_FOLD_LEFT_F)(op, op(150, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_150(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_151, PP_SEQ_FOLD_LEFT_F)(op, op(151, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_151(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_152, PP_SEQ_FOLD_LEFT_F)(op, op(152, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_152(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_153, PP_SEQ_FOLD_LEFT_F)(op, op(153, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_153(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_154, PP_SEQ_FOLD_LEFT_F)(op, op(154, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_154(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_155, PP_SEQ_FOLD_LEFT_F)(op, op(155, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_155(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_156, PP_SEQ_FOLD_LEFT_F)(op, op(156, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_156(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_157, PP_SEQ_FOLD_LEFT_F)(op, op(157, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_157(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_158, PP_SEQ_FOLD_LEFT_F)(op, op(158, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_158(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_159, PP_SEQ_FOLD_LEFT_F)(op, op(159, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_159(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_160, PP_SEQ_FOLD_LEFT_F)(op, op(160, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_160(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_161, PP_SEQ_FOLD_LEFT_F)(op, op(161, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_161(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_162, PP_SEQ_FOLD_LEFT_F)(op, op(162, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_162(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_163, PP_SEQ_FOLD_LEFT_F)(op, op(163, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_163(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_164, PP_SEQ_FOLD_LEFT_F)(op, op(164, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_164(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_165, PP_SEQ_FOLD_LEFT_F)(op, op(165, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_165(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_166, PP_SEQ_FOLD_LEFT_F)(op, op(166, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_166(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_167, PP_SEQ_FOLD_LEFT_F)(op, op(167, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_167(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_168, PP_SEQ_FOLD_LEFT_F)(op, op(168, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_168(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_169, PP_SEQ_FOLD_LEFT_F)(op, op(169, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_169(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_170, PP_SEQ_FOLD_LEFT_F)(op, op(170, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_170(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_171, PP_SEQ_FOLD_LEFT_F)(op, op(171, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_171(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_172, PP_SEQ_FOLD_LEFT_F)(op, op(172, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_172(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_173, PP_SEQ_FOLD_LEFT_F)(op, op(173, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_173(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_174, PP_SEQ_FOLD_LEFT_F)(op, op(174, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_174(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_175, PP_SEQ_FOLD_LEFT_F)(op, op(175, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_175(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_176, PP_SEQ_FOLD_LEFT_F)(op, op(176, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_176(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_177, PP_SEQ_FOLD_LEFT_F)(op, op(177, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_177(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_178, PP_SEQ_FOLD_LEFT_F)(op, op(178, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_178(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_179, PP_SEQ_FOLD_LEFT_F)(op, op(179, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_179(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_180, PP_SEQ_FOLD_LEFT_F)(op, op(180, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_180(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_181, PP_SEQ_FOLD_LEFT_F)(op, op(181, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_181(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_182, PP_SEQ_FOLD_LEFT_F)(op, op(182, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_182(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_183, PP_SEQ_FOLD_LEFT_F)(op, op(183, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_183(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_184, PP_SEQ_FOLD_LEFT_F)(op, op(184, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_184(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_185, PP_SEQ_FOLD_LEFT_F)(op, op(185, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_185(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_186, PP_SEQ_FOLD_LEFT_F)(op, op(186, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_186(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_187, PP_SEQ_FOLD_LEFT_F)(op, op(187, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_187(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_188, PP_SEQ_FOLD_LEFT_F)(op, op(188, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_188(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_189, PP_SEQ_FOLD_LEFT_F)(op, op(189, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_189(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_190, PP_SEQ_FOLD_LEFT_F)(op, op(190, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_190(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_191, PP_SEQ_FOLD_LEFT_F)(op, op(191, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_191(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_192, PP_SEQ_FOLD_LEFT_F)(op, op(192, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_192(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_193, PP_SEQ_FOLD_LEFT_F)(op, op(193, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_193(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_194, PP_SEQ_FOLD_LEFT_F)(op, op(194, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_194(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_195, PP_SEQ_FOLD_LEFT_F)(op, op(195, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_195(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_196, PP_SEQ_FOLD_LEFT_F)(op, op(196, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_196(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_197, PP_SEQ_FOLD_LEFT_F)(op, op(197, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_197(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_198, PP_SEQ_FOLD_LEFT_F)(op, op(198, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_198(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_199, PP_SEQ_FOLD_LEFT_F)(op, op(199, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_199(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_200, PP_SEQ_FOLD_LEFT_F)(op, op(200, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_200(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_201, PP_SEQ_FOLD_LEFT_F)(op, op(201, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_201(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_202, PP_SEQ_FOLD_LEFT_F)(op, op(202, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_202(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_203, PP_SEQ_FOLD_LEFT_F)(op, op(203, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_203(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_204, PP_SEQ_FOLD_LEFT_F)(op, op(204, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_204(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_205, PP_SEQ_FOLD_LEFT_F)(op, op(205, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_205(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_206, PP_SEQ_FOLD_LEFT_F)(op, op(206, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_206(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_207, PP_SEQ_FOLD_LEFT_F)(op, op(207, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_207(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_208, PP_SEQ_FOLD_LEFT_F)(op, op(208, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_208(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_209, PP_SEQ_FOLD_LEFT_F)(op, op(209, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_209(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_210, PP_SEQ_FOLD_LEFT_F)(op, op(210, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_210(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_211, PP_SEQ_FOLD_LEFT_F)(op, op(211, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_211(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_212, PP_SEQ_FOLD_LEFT_F)(op, op(212, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_212(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_213, PP_SEQ_FOLD_LEFT_F)(op, op(213, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_213(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_214, PP_SEQ_FOLD_LEFT_F)(op, op(214, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_214(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_215, PP_SEQ_FOLD_LEFT_F)(op, op(215, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_215(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_216, PP_SEQ_FOLD_LEFT_F)(op, op(216, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_216(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_217, PP_SEQ_FOLD_LEFT_F)(op, op(217, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_217(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_218, PP_SEQ_FOLD_LEFT_F)(op, op(218, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_218(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_219, PP_SEQ_FOLD_LEFT_F)(op, op(219, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_219(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_220, PP_SEQ_FOLD_LEFT_F)(op, op(220, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_220(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_221, PP_SEQ_FOLD_LEFT_F)(op, op(221, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_221(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_222, PP_SEQ_FOLD_LEFT_F)(op, op(222, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_222(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_223, PP_SEQ_FOLD_LEFT_F)(op, op(223, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_223(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_224, PP_SEQ_FOLD_LEFT_F)(op, op(224, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_224(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_225, PP_SEQ_FOLD_LEFT_F)(op, op(225, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_225(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_226, PP_SEQ_FOLD_LEFT_F)(op, op(226, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_226(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_227, PP_SEQ_FOLD_LEFT_F)(op, op(227, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_227(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_228, PP_SEQ_FOLD_LEFT_F)(op, op(228, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_228(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_229, PP_SEQ_FOLD_LEFT_F)(op, op(229, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_229(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_230, PP_SEQ_FOLD_LEFT_F)(op, op(230, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_230(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_231, PP_SEQ_FOLD_LEFT_F)(op, op(231, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_231(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_232, PP_SEQ_FOLD_LEFT_F)(op, op(232, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_232(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_233, PP_SEQ_FOLD_LEFT_F)(op, op(233, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_233(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_234, PP_SEQ_FOLD_LEFT_F)(op, op(234, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_234(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_235, PP_SEQ_FOLD_LEFT_F)(op, op(235, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_235(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_236, PP_SEQ_FOLD_LEFT_F)(op, op(236, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_236(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_237, PP_SEQ_FOLD_LEFT_F)(op, op(237, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_237(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_238, PP_SEQ_FOLD_LEFT_F)(op, op(238, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_238(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_239, PP_SEQ_FOLD_LEFT_F)(op, op(239, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_239(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_240, PP_SEQ_FOLD_LEFT_F)(op, op(240, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_240(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_241, PP_SEQ_FOLD_LEFT_F)(op, op(241, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_241(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_242, PP_SEQ_FOLD_LEFT_F)(op, op(242, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_242(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_243, PP_SEQ_FOLD_LEFT_F)(op, op(243, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_243(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_244, PP_SEQ_FOLD_LEFT_F)(op, op(244, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_244(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_245, PP_SEQ_FOLD_LEFT_F)(op, op(245, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_245(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_246, PP_SEQ_FOLD_LEFT_F)(op, op(246, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_246(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_247, PP_SEQ_FOLD_LEFT_F)(op, op(247, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_247(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_248, PP_SEQ_FOLD_LEFT_F)(op, op(248, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_248(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_249, PP_SEQ_FOLD_LEFT_F)(op, op(249, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_249(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_250, PP_SEQ_FOLD_LEFT_F)(op, op(250, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_250(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_251, PP_SEQ_FOLD_LEFT_F)(op, op(251, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_251(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_252, PP_SEQ_FOLD_LEFT_F)(op, op(252, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_252(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_253, PP_SEQ_FOLD_LEFT_F)(op, op(253, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_253(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_254, PP_SEQ_FOLD_LEFT_F)(op, op(254, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_254(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_255, PP_SEQ_FOLD_LEFT_F)(op, op(255, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_255(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_256, PP_SEQ_FOLD_LEFT_F)(op, op(256, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_256(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_257, PP_SEQ_FOLD_LEFT_F)(op, op(257, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
# else
#    define PP_SEQ_FOLD_LEFT_I_1(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_2, PP_SEQ_FOLD_LEFT_F)(op, op##(2, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_2(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_3, PP_SEQ_FOLD_LEFT_F)(op, op##(3, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_3(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_4, PP_SEQ_FOLD_LEFT_F)(op, op##(4, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_4(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_5, PP_SEQ_FOLD_LEFT_F)(op, op##(5, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_5(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_6, PP_SEQ_FOLD_LEFT_F)(op, op##(6, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_6(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_7, PP_SEQ_FOLD_LEFT_F)(op, op##(7, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_7(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_8, PP_SEQ_FOLD_LEFT_F)(op, op##(8, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_8(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_9, PP_SEQ_FOLD_LEFT_F)(op, op##(9, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_9(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_10, PP_SEQ_FOLD_LEFT_F)(op, op##(10, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_10(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_11, PP_SEQ_FOLD_LEFT_F)(op, op##(11, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_11(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_12, PP_SEQ_FOLD_LEFT_F)(op, op##(12, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_12(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_13, PP_SEQ_FOLD_LEFT_F)(op, op##(13, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_13(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_14, PP_SEQ_FOLD_LEFT_F)(op, op##(14, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_14(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_15, PP_SEQ_FOLD_LEFT_F)(op, op##(15, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_15(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_16, PP_SEQ_FOLD_LEFT_F)(op, op##(16, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_16(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_17, PP_SEQ_FOLD_LEFT_F)(op, op##(17, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_17(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_18, PP_SEQ_FOLD_LEFT_F)(op, op##(18, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_18(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_19, PP_SEQ_FOLD_LEFT_F)(op, op##(19, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_19(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_20, PP_SEQ_FOLD_LEFT_F)(op, op##(20, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_20(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_21, PP_SEQ_FOLD_LEFT_F)(op, op##(21, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_21(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_22, PP_SEQ_FOLD_LEFT_F)(op, op##(22, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_22(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_23, PP_SEQ_FOLD_LEFT_F)(op, op##(23, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_23(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_24, PP_SEQ_FOLD_LEFT_F)(op, op##(24, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_24(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_25, PP_SEQ_FOLD_LEFT_F)(op, op##(25, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_25(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_26, PP_SEQ_FOLD_LEFT_F)(op, op##(26, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_26(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_27, PP_SEQ_FOLD_LEFT_F)(op, op##(27, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_27(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_28, PP_SEQ_FOLD_LEFT_F)(op, op##(28, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_28(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_29, PP_SEQ_FOLD_LEFT_F)(op, op##(29, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_29(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_30, PP_SEQ_FOLD_LEFT_F)(op, op##(30, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_30(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_31, PP_SEQ_FOLD_LEFT_F)(op, op##(31, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_31(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_32, PP_SEQ_FOLD_LEFT_F)(op, op##(32, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_32(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_33, PP_SEQ_FOLD_LEFT_F)(op, op##(33, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_33(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_34, PP_SEQ_FOLD_LEFT_F)(op, op##(34, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_34(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_35, PP_SEQ_FOLD_LEFT_F)(op, op##(35, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_35(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_36, PP_SEQ_FOLD_LEFT_F)(op, op##(36, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_36(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_37, PP_SEQ_FOLD_LEFT_F)(op, op##(37, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_37(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_38, PP_SEQ_FOLD_LEFT_F)(op, op##(38, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_38(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_39, PP_SEQ_FOLD_LEFT_F)(op, op##(39, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_39(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_40, PP_SEQ_FOLD_LEFT_F)(op, op##(40, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_40(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_41, PP_SEQ_FOLD_LEFT_F)(op, op##(41, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_41(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_42, PP_SEQ_FOLD_LEFT_F)(op, op##(42, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_42(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_43, PP_SEQ_FOLD_LEFT_F)(op, op##(43, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_43(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_44, PP_SEQ_FOLD_LEFT_F)(op, op##(44, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_44(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_45, PP_SEQ_FOLD_LEFT_F)(op, op##(45, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_45(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_46, PP_SEQ_FOLD_LEFT_F)(op, op##(46, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_46(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_47, PP_SEQ_FOLD_LEFT_F)(op, op##(47, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_47(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_48, PP_SEQ_FOLD_LEFT_F)(op, op##(48, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_48(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_49, PP_SEQ_FOLD_LEFT_F)(op, op##(49, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_49(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_50, PP_SEQ_FOLD_LEFT_F)(op, op##(50, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_50(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_51, PP_SEQ_FOLD_LEFT_F)(op, op##(51, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_51(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_52, PP_SEQ_FOLD_LEFT_F)(op, op##(52, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_52(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_53, PP_SEQ_FOLD_LEFT_F)(op, op##(53, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_53(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_54, PP_SEQ_FOLD_LEFT_F)(op, op##(54, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_54(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_55, PP_SEQ_FOLD_LEFT_F)(op, op##(55, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_55(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_56, PP_SEQ_FOLD_LEFT_F)(op, op##(56, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_56(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_57, PP_SEQ_FOLD_LEFT_F)(op, op##(57, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_57(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_58, PP_SEQ_FOLD_LEFT_F)(op, op##(58, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_58(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_59, PP_SEQ_FOLD_LEFT_F)(op, op##(59, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_59(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_60, PP_SEQ_FOLD_LEFT_F)(op, op##(60, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_60(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_61, PP_SEQ_FOLD_LEFT_F)(op, op##(61, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_61(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_62, PP_SEQ_FOLD_LEFT_F)(op, op##(62, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_62(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_63, PP_SEQ_FOLD_LEFT_F)(op, op##(63, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_63(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_64, PP_SEQ_FOLD_LEFT_F)(op, op##(64, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_64(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_65, PP_SEQ_FOLD_LEFT_F)(op, op##(65, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_65(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_66, PP_SEQ_FOLD_LEFT_F)(op, op##(66, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_66(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_67, PP_SEQ_FOLD_LEFT_F)(op, op##(67, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_67(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_68, PP_SEQ_FOLD_LEFT_F)(op, op##(68, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_68(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_69, PP_SEQ_FOLD_LEFT_F)(op, op##(69, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_69(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_70, PP_SEQ_FOLD_LEFT_F)(op, op##(70, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_70(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_71, PP_SEQ_FOLD_LEFT_F)(op, op##(71, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_71(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_72, PP_SEQ_FOLD_LEFT_F)(op, op##(72, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_72(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_73, PP_SEQ_FOLD_LEFT_F)(op, op##(73, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_73(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_74, PP_SEQ_FOLD_LEFT_F)(op, op##(74, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_74(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_75, PP_SEQ_FOLD_LEFT_F)(op, op##(75, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_75(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_76, PP_SEQ_FOLD_LEFT_F)(op, op##(76, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_76(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_77, PP_SEQ_FOLD_LEFT_F)(op, op##(77, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_77(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_78, PP_SEQ_FOLD_LEFT_F)(op, op##(78, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_78(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_79, PP_SEQ_FOLD_LEFT_F)(op, op##(79, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_79(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_80, PP_SEQ_FOLD_LEFT_F)(op, op##(80, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_80(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_81, PP_SEQ_FOLD_LEFT_F)(op, op##(81, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_81(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_82, PP_SEQ_FOLD_LEFT_F)(op, op##(82, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_82(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_83, PP_SEQ_FOLD_LEFT_F)(op, op##(83, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_83(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_84, PP_SEQ_FOLD_LEFT_F)(op, op##(84, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_84(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_85, PP_SEQ_FOLD_LEFT_F)(op, op##(85, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_85(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_86, PP_SEQ_FOLD_LEFT_F)(op, op##(86, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_86(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_87, PP_SEQ_FOLD_LEFT_F)(op, op##(87, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_87(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_88, PP_SEQ_FOLD_LEFT_F)(op, op##(88, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_88(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_89, PP_SEQ_FOLD_LEFT_F)(op, op##(89, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_89(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_90, PP_SEQ_FOLD_LEFT_F)(op, op##(90, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_90(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_91, PP_SEQ_FOLD_LEFT_F)(op, op##(91, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_91(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_92, PP_SEQ_FOLD_LEFT_F)(op, op##(92, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_92(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_93, PP_SEQ_FOLD_LEFT_F)(op, op##(93, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_93(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_94, PP_SEQ_FOLD_LEFT_F)(op, op##(94, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_94(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_95, PP_SEQ_FOLD_LEFT_F)(op, op##(95, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_95(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_96, PP_SEQ_FOLD_LEFT_F)(op, op##(96, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_96(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_97, PP_SEQ_FOLD_LEFT_F)(op, op##(97, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_97(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_98, PP_SEQ_FOLD_LEFT_F)(op, op##(98, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_98(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_99, PP_SEQ_FOLD_LEFT_F)(op, op##(99, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_99(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_100, PP_SEQ_FOLD_LEFT_F)(op, op##(100, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_100(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_101, PP_SEQ_FOLD_LEFT_F)(op, op##(101, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_101(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_102, PP_SEQ_FOLD_LEFT_F)(op, op##(102, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_102(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_103, PP_SEQ_FOLD_LEFT_F)(op, op##(103, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_103(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_104, PP_SEQ_FOLD_LEFT_F)(op, op##(104, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_104(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_105, PP_SEQ_FOLD_LEFT_F)(op, op##(105, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_105(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_106, PP_SEQ_FOLD_LEFT_F)(op, op##(106, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_106(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_107, PP_SEQ_FOLD_LEFT_F)(op, op##(107, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_107(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_108, PP_SEQ_FOLD_LEFT_F)(op, op##(108, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_108(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_109, PP_SEQ_FOLD_LEFT_F)(op, op##(109, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_109(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_110, PP_SEQ_FOLD_LEFT_F)(op, op##(110, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_110(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_111, PP_SEQ_FOLD_LEFT_F)(op, op##(111, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_111(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_112, PP_SEQ_FOLD_LEFT_F)(op, op##(112, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_112(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_113, PP_SEQ_FOLD_LEFT_F)(op, op##(113, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_113(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_114, PP_SEQ_FOLD_LEFT_F)(op, op##(114, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_114(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_115, PP_SEQ_FOLD_LEFT_F)(op, op##(115, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_115(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_116, PP_SEQ_FOLD_LEFT_F)(op, op##(116, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_116(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_117, PP_SEQ_FOLD_LEFT_F)(op, op##(117, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_117(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_118, PP_SEQ_FOLD_LEFT_F)(op, op##(118, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_118(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_119, PP_SEQ_FOLD_LEFT_F)(op, op##(119, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_119(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_120, PP_SEQ_FOLD_LEFT_F)(op, op##(120, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_120(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_121, PP_SEQ_FOLD_LEFT_F)(op, op##(121, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_121(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_122, PP_SEQ_FOLD_LEFT_F)(op, op##(122, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_122(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_123, PP_SEQ_FOLD_LEFT_F)(op, op##(123, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_123(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_124, PP_SEQ_FOLD_LEFT_F)(op, op##(124, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_124(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_125, PP_SEQ_FOLD_LEFT_F)(op, op##(125, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_125(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_126, PP_SEQ_FOLD_LEFT_F)(op, op##(126, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_126(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_127, PP_SEQ_FOLD_LEFT_F)(op, op##(127, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_127(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_128, PP_SEQ_FOLD_LEFT_F)(op, op##(128, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_128(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_129, PP_SEQ_FOLD_LEFT_F)(op, op##(129, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_129(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_130, PP_SEQ_FOLD_LEFT_F)(op, op##(130, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_130(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_131, PP_SEQ_FOLD_LEFT_F)(op, op##(131, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_131(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_132, PP_SEQ_FOLD_LEFT_F)(op, op##(132, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_132(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_133, PP_SEQ_FOLD_LEFT_F)(op, op##(133, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_133(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_134, PP_SEQ_FOLD_LEFT_F)(op, op##(134, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_134(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_135, PP_SEQ_FOLD_LEFT_F)(op, op##(135, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_135(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_136, PP_SEQ_FOLD_LEFT_F)(op, op##(136, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_136(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_137, PP_SEQ_FOLD_LEFT_F)(op, op##(137, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_137(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_138, PP_SEQ_FOLD_LEFT_F)(op, op##(138, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_138(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_139, PP_SEQ_FOLD_LEFT_F)(op, op##(139, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_139(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_140, PP_SEQ_FOLD_LEFT_F)(op, op##(140, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_140(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_141, PP_SEQ_FOLD_LEFT_F)(op, op##(141, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_141(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_142, PP_SEQ_FOLD_LEFT_F)(op, op##(142, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_142(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_143, PP_SEQ_FOLD_LEFT_F)(op, op##(143, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_143(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_144, PP_SEQ_FOLD_LEFT_F)(op, op##(144, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_144(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_145, PP_SEQ_FOLD_LEFT_F)(op, op##(145, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_145(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_146, PP_SEQ_FOLD_LEFT_F)(op, op##(146, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_146(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_147, PP_SEQ_FOLD_LEFT_F)(op, op##(147, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_147(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_148, PP_SEQ_FOLD_LEFT_F)(op, op##(148, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_148(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_149, PP_SEQ_FOLD_LEFT_F)(op, op##(149, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_149(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_150, PP_SEQ_FOLD_LEFT_F)(op, op##(150, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_150(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_151, PP_SEQ_FOLD_LEFT_F)(op, op##(151, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_151(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_152, PP_SEQ_FOLD_LEFT_F)(op, op##(152, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_152(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_153, PP_SEQ_FOLD_LEFT_F)(op, op##(153, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_153(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_154, PP_SEQ_FOLD_LEFT_F)(op, op##(154, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_154(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_155, PP_SEQ_FOLD_LEFT_F)(op, op##(155, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_155(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_156, PP_SEQ_FOLD_LEFT_F)(op, op##(156, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_156(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_157, PP_SEQ_FOLD_LEFT_F)(op, op##(157, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_157(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_158, PP_SEQ_FOLD_LEFT_F)(op, op##(158, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_158(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_159, PP_SEQ_FOLD_LEFT_F)(op, op##(159, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_159(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_160, PP_SEQ_FOLD_LEFT_F)(op, op##(160, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_160(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_161, PP_SEQ_FOLD_LEFT_F)(op, op##(161, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_161(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_162, PP_SEQ_FOLD_LEFT_F)(op, op##(162, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_162(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_163, PP_SEQ_FOLD_LEFT_F)(op, op##(163, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_163(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_164, PP_SEQ_FOLD_LEFT_F)(op, op##(164, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_164(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_165, PP_SEQ_FOLD_LEFT_F)(op, op##(165, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_165(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_166, PP_SEQ_FOLD_LEFT_F)(op, op##(166, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_166(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_167, PP_SEQ_FOLD_LEFT_F)(op, op##(167, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_167(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_168, PP_SEQ_FOLD_LEFT_F)(op, op##(168, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_168(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_169, PP_SEQ_FOLD_LEFT_F)(op, op##(169, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_169(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_170, PP_SEQ_FOLD_LEFT_F)(op, op##(170, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_170(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_171, PP_SEQ_FOLD_LEFT_F)(op, op##(171, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_171(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_172, PP_SEQ_FOLD_LEFT_F)(op, op##(172, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_172(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_173, PP_SEQ_FOLD_LEFT_F)(op, op##(173, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_173(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_174, PP_SEQ_FOLD_LEFT_F)(op, op##(174, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_174(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_175, PP_SEQ_FOLD_LEFT_F)(op, op##(175, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_175(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_176, PP_SEQ_FOLD_LEFT_F)(op, op##(176, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_176(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_177, PP_SEQ_FOLD_LEFT_F)(op, op##(177, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_177(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_178, PP_SEQ_FOLD_LEFT_F)(op, op##(178, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_178(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_179, PP_SEQ_FOLD_LEFT_F)(op, op##(179, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_179(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_180, PP_SEQ_FOLD_LEFT_F)(op, op##(180, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_180(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_181, PP_SEQ_FOLD_LEFT_F)(op, op##(181, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_181(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_182, PP_SEQ_FOLD_LEFT_F)(op, op##(182, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_182(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_183, PP_SEQ_FOLD_LEFT_F)(op, op##(183, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_183(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_184, PP_SEQ_FOLD_LEFT_F)(op, op##(184, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_184(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_185, PP_SEQ_FOLD_LEFT_F)(op, op##(185, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_185(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_186, PP_SEQ_FOLD_LEFT_F)(op, op##(186, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_186(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_187, PP_SEQ_FOLD_LEFT_F)(op, op##(187, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_187(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_188, PP_SEQ_FOLD_LEFT_F)(op, op##(188, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_188(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_189, PP_SEQ_FOLD_LEFT_F)(op, op##(189, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_189(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_190, PP_SEQ_FOLD_LEFT_F)(op, op##(190, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_190(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_191, PP_SEQ_FOLD_LEFT_F)(op, op##(191, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_191(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_192, PP_SEQ_FOLD_LEFT_F)(op, op##(192, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_192(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_193, PP_SEQ_FOLD_LEFT_F)(op, op##(193, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_193(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_194, PP_SEQ_FOLD_LEFT_F)(op, op##(194, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_194(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_195, PP_SEQ_FOLD_LEFT_F)(op, op##(195, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_195(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_196, PP_SEQ_FOLD_LEFT_F)(op, op##(196, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_196(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_197, PP_SEQ_FOLD_LEFT_F)(op, op##(197, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_197(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_198, PP_SEQ_FOLD_LEFT_F)(op, op##(198, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_198(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_199, PP_SEQ_FOLD_LEFT_F)(op, op##(199, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_199(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_200, PP_SEQ_FOLD_LEFT_F)(op, op##(200, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_200(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_201, PP_SEQ_FOLD_LEFT_F)(op, op##(201, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_201(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_202, PP_SEQ_FOLD_LEFT_F)(op, op##(202, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_202(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_203, PP_SEQ_FOLD_LEFT_F)(op, op##(203, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_203(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_204, PP_SEQ_FOLD_LEFT_F)(op, op##(204, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_204(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_205, PP_SEQ_FOLD_LEFT_F)(op, op##(205, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_205(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_206, PP_SEQ_FOLD_LEFT_F)(op, op##(206, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_206(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_207, PP_SEQ_FOLD_LEFT_F)(op, op##(207, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_207(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_208, PP_SEQ_FOLD_LEFT_F)(op, op##(208, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_208(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_209, PP_SEQ_FOLD_LEFT_F)(op, op##(209, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_209(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_210, PP_SEQ_FOLD_LEFT_F)(op, op##(210, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_210(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_211, PP_SEQ_FOLD_LEFT_F)(op, op##(211, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_211(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_212, PP_SEQ_FOLD_LEFT_F)(op, op##(212, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_212(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_213, PP_SEQ_FOLD_LEFT_F)(op, op##(213, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_213(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_214, PP_SEQ_FOLD_LEFT_F)(op, op##(214, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_214(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_215, PP_SEQ_FOLD_LEFT_F)(op, op##(215, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_215(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_216, PP_SEQ_FOLD_LEFT_F)(op, op##(216, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_216(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_217, PP_SEQ_FOLD_LEFT_F)(op, op##(217, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_217(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_218, PP_SEQ_FOLD_LEFT_F)(op, op##(218, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_218(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_219, PP_SEQ_FOLD_LEFT_F)(op, op##(219, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_219(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_220, PP_SEQ_FOLD_LEFT_F)(op, op##(220, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_220(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_221, PP_SEQ_FOLD_LEFT_F)(op, op##(221, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_221(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_222, PP_SEQ_FOLD_LEFT_F)(op, op##(222, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_222(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_223, PP_SEQ_FOLD_LEFT_F)(op, op##(223, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_223(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_224, PP_SEQ_FOLD_LEFT_F)(op, op##(224, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_224(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_225, PP_SEQ_FOLD_LEFT_F)(op, op##(225, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_225(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_226, PP_SEQ_FOLD_LEFT_F)(op, op##(226, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_226(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_227, PP_SEQ_FOLD_LEFT_F)(op, op##(227, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_227(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_228, PP_SEQ_FOLD_LEFT_F)(op, op##(228, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_228(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_229, PP_SEQ_FOLD_LEFT_F)(op, op##(229, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_229(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_230, PP_SEQ_FOLD_LEFT_F)(op, op##(230, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_230(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_231, PP_SEQ_FOLD_LEFT_F)(op, op##(231, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_231(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_232, PP_SEQ_FOLD_LEFT_F)(op, op##(232, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_232(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_233, PP_SEQ_FOLD_LEFT_F)(op, op##(233, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_233(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_234, PP_SEQ_FOLD_LEFT_F)(op, op##(234, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_234(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_235, PP_SEQ_FOLD_LEFT_F)(op, op##(235, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_235(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_236, PP_SEQ_FOLD_LEFT_F)(op, op##(236, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_236(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_237, PP_SEQ_FOLD_LEFT_F)(op, op##(237, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_237(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_238, PP_SEQ_FOLD_LEFT_F)(op, op##(238, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_238(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_239, PP_SEQ_FOLD_LEFT_F)(op, op##(239, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_239(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_240, PP_SEQ_FOLD_LEFT_F)(op, op##(240, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_240(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_241, PP_SEQ_FOLD_LEFT_F)(op, op##(241, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_241(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_242, PP_SEQ_FOLD_LEFT_F)(op, op##(242, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_242(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_243, PP_SEQ_FOLD_LEFT_F)(op, op##(243, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_243(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_244, PP_SEQ_FOLD_LEFT_F)(op, op##(244, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_244(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_245, PP_SEQ_FOLD_LEFT_F)(op, op##(245, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_245(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_246, PP_SEQ_FOLD_LEFT_F)(op, op##(246, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_246(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_247, PP_SEQ_FOLD_LEFT_F)(op, op##(247, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_247(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_248, PP_SEQ_FOLD_LEFT_F)(op, op##(248, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_248(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_249, PP_SEQ_FOLD_LEFT_F)(op, op##(249, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_249(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_250, PP_SEQ_FOLD_LEFT_F)(op, op##(250, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_250(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_251, PP_SEQ_FOLD_LEFT_F)(op, op##(251, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_251(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_252, PP_SEQ_FOLD_LEFT_F)(op, op##(252, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_252(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_253, PP_SEQ_FOLD_LEFT_F)(op, op##(253, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_253(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_254, PP_SEQ_FOLD_LEFT_F)(op, op##(254, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_254(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_255, PP_SEQ_FOLD_LEFT_F)(op, op##(255, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_255(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_256, PP_SEQ_FOLD_LEFT_F)(op, op##(256, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
#    define PP_SEQ_FOLD_LEFT_I_256(op, st, ss, sz) PP_IF(PP_DEC(sz), PP_SEQ_FOLD_LEFT_I_257, PP_SEQ_FOLD_LEFT_F)(op, op##(257, st, PP_SEQ_HEAD(ss)), PP_SEQ_TAIL(ss), PP_DEC(sz))
# endif
#
# endif
