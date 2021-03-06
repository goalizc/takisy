# ifndef PREPROCESSOR_SEQ_SIZE_HPP
# define PREPROCESSOR_SEQ_SIZE_HPP
#
# include <takisy/preprocessor/cat.hpp>
# include <takisy/preprocessor/config/config.hpp>
#
# if PP_CONFIG_FLAGS() & PP_CONFIG_MWCC()
#    define PP_SEQ_SIZE(seq) PP_SEQ_SIZE_I((seq))
#    define PP_SEQ_SIZE_I(par) PP_SEQ_SIZE_II ## par
#    define PP_SEQ_SIZE_II(seq) PP_CAT(PP_SEQ_SIZE_, PP_SEQ_SIZE_0 ## seq)
# elif PP_CONFIG_FLAGS() & PP_CONFIG_EDG() || PP_CONFIG_FLAGS() & PP_CONFIG_MSVC()
#    define PP_SEQ_SIZE(seq) PP_SEQ_SIZE_I(seq)
#    define PP_SEQ_SIZE_I(seq) PP_CAT(PP_SEQ_SIZE_, PP_SEQ_SIZE_0 seq)
# elif defined(__IBMC__) || defined(__IBMCPP__)
#    define PP_SEQ_SIZE(seq) PP_CAT(PP_SEQ_SIZE_, PP_CAT(PP_SEQ_SIZE_0, seq))
# else
#    define PP_SEQ_SIZE(seq) PP_CAT(PP_SEQ_SIZE_, PP_SEQ_SIZE_0 seq)
# endif
#
# define PP_SEQ_SIZE_0(_) PP_SEQ_SIZE_1
# define PP_SEQ_SIZE_1(_) PP_SEQ_SIZE_2
# define PP_SEQ_SIZE_2(_) PP_SEQ_SIZE_3
# define PP_SEQ_SIZE_3(_) PP_SEQ_SIZE_4
# define PP_SEQ_SIZE_4(_) PP_SEQ_SIZE_5
# define PP_SEQ_SIZE_5(_) PP_SEQ_SIZE_6
# define PP_SEQ_SIZE_6(_) PP_SEQ_SIZE_7
# define PP_SEQ_SIZE_7(_) PP_SEQ_SIZE_8
# define PP_SEQ_SIZE_8(_) PP_SEQ_SIZE_9
# define PP_SEQ_SIZE_9(_) PP_SEQ_SIZE_10
# define PP_SEQ_SIZE_10(_) PP_SEQ_SIZE_11
# define PP_SEQ_SIZE_11(_) PP_SEQ_SIZE_12
# define PP_SEQ_SIZE_12(_) PP_SEQ_SIZE_13
# define PP_SEQ_SIZE_13(_) PP_SEQ_SIZE_14
# define PP_SEQ_SIZE_14(_) PP_SEQ_SIZE_15
# define PP_SEQ_SIZE_15(_) PP_SEQ_SIZE_16
# define PP_SEQ_SIZE_16(_) PP_SEQ_SIZE_17
# define PP_SEQ_SIZE_17(_) PP_SEQ_SIZE_18
# define PP_SEQ_SIZE_18(_) PP_SEQ_SIZE_19
# define PP_SEQ_SIZE_19(_) PP_SEQ_SIZE_20
# define PP_SEQ_SIZE_20(_) PP_SEQ_SIZE_21
# define PP_SEQ_SIZE_21(_) PP_SEQ_SIZE_22
# define PP_SEQ_SIZE_22(_) PP_SEQ_SIZE_23
# define PP_SEQ_SIZE_23(_) PP_SEQ_SIZE_24
# define PP_SEQ_SIZE_24(_) PP_SEQ_SIZE_25
# define PP_SEQ_SIZE_25(_) PP_SEQ_SIZE_26
# define PP_SEQ_SIZE_26(_) PP_SEQ_SIZE_27
# define PP_SEQ_SIZE_27(_) PP_SEQ_SIZE_28
# define PP_SEQ_SIZE_28(_) PP_SEQ_SIZE_29
# define PP_SEQ_SIZE_29(_) PP_SEQ_SIZE_30
# define PP_SEQ_SIZE_30(_) PP_SEQ_SIZE_31
# define PP_SEQ_SIZE_31(_) PP_SEQ_SIZE_32
# define PP_SEQ_SIZE_32(_) PP_SEQ_SIZE_33
# define PP_SEQ_SIZE_33(_) PP_SEQ_SIZE_34
# define PP_SEQ_SIZE_34(_) PP_SEQ_SIZE_35
# define PP_SEQ_SIZE_35(_) PP_SEQ_SIZE_36
# define PP_SEQ_SIZE_36(_) PP_SEQ_SIZE_37
# define PP_SEQ_SIZE_37(_) PP_SEQ_SIZE_38
# define PP_SEQ_SIZE_38(_) PP_SEQ_SIZE_39
# define PP_SEQ_SIZE_39(_) PP_SEQ_SIZE_40
# define PP_SEQ_SIZE_40(_) PP_SEQ_SIZE_41
# define PP_SEQ_SIZE_41(_) PP_SEQ_SIZE_42
# define PP_SEQ_SIZE_42(_) PP_SEQ_SIZE_43
# define PP_SEQ_SIZE_43(_) PP_SEQ_SIZE_44
# define PP_SEQ_SIZE_44(_) PP_SEQ_SIZE_45
# define PP_SEQ_SIZE_45(_) PP_SEQ_SIZE_46
# define PP_SEQ_SIZE_46(_) PP_SEQ_SIZE_47
# define PP_SEQ_SIZE_47(_) PP_SEQ_SIZE_48
# define PP_SEQ_SIZE_48(_) PP_SEQ_SIZE_49
# define PP_SEQ_SIZE_49(_) PP_SEQ_SIZE_50
# define PP_SEQ_SIZE_50(_) PP_SEQ_SIZE_51
# define PP_SEQ_SIZE_51(_) PP_SEQ_SIZE_52
# define PP_SEQ_SIZE_52(_) PP_SEQ_SIZE_53
# define PP_SEQ_SIZE_53(_) PP_SEQ_SIZE_54
# define PP_SEQ_SIZE_54(_) PP_SEQ_SIZE_55
# define PP_SEQ_SIZE_55(_) PP_SEQ_SIZE_56
# define PP_SEQ_SIZE_56(_) PP_SEQ_SIZE_57
# define PP_SEQ_SIZE_57(_) PP_SEQ_SIZE_58
# define PP_SEQ_SIZE_58(_) PP_SEQ_SIZE_59
# define PP_SEQ_SIZE_59(_) PP_SEQ_SIZE_60
# define PP_SEQ_SIZE_60(_) PP_SEQ_SIZE_61
# define PP_SEQ_SIZE_61(_) PP_SEQ_SIZE_62
# define PP_SEQ_SIZE_62(_) PP_SEQ_SIZE_63
# define PP_SEQ_SIZE_63(_) PP_SEQ_SIZE_64
# define PP_SEQ_SIZE_64(_) PP_SEQ_SIZE_65
# define PP_SEQ_SIZE_65(_) PP_SEQ_SIZE_66
# define PP_SEQ_SIZE_66(_) PP_SEQ_SIZE_67
# define PP_SEQ_SIZE_67(_) PP_SEQ_SIZE_68
# define PP_SEQ_SIZE_68(_) PP_SEQ_SIZE_69
# define PP_SEQ_SIZE_69(_) PP_SEQ_SIZE_70
# define PP_SEQ_SIZE_70(_) PP_SEQ_SIZE_71
# define PP_SEQ_SIZE_71(_) PP_SEQ_SIZE_72
# define PP_SEQ_SIZE_72(_) PP_SEQ_SIZE_73
# define PP_SEQ_SIZE_73(_) PP_SEQ_SIZE_74
# define PP_SEQ_SIZE_74(_) PP_SEQ_SIZE_75
# define PP_SEQ_SIZE_75(_) PP_SEQ_SIZE_76
# define PP_SEQ_SIZE_76(_) PP_SEQ_SIZE_77
# define PP_SEQ_SIZE_77(_) PP_SEQ_SIZE_78
# define PP_SEQ_SIZE_78(_) PP_SEQ_SIZE_79
# define PP_SEQ_SIZE_79(_) PP_SEQ_SIZE_80
# define PP_SEQ_SIZE_80(_) PP_SEQ_SIZE_81
# define PP_SEQ_SIZE_81(_) PP_SEQ_SIZE_82
# define PP_SEQ_SIZE_82(_) PP_SEQ_SIZE_83
# define PP_SEQ_SIZE_83(_) PP_SEQ_SIZE_84
# define PP_SEQ_SIZE_84(_) PP_SEQ_SIZE_85
# define PP_SEQ_SIZE_85(_) PP_SEQ_SIZE_86
# define PP_SEQ_SIZE_86(_) PP_SEQ_SIZE_87
# define PP_SEQ_SIZE_87(_) PP_SEQ_SIZE_88
# define PP_SEQ_SIZE_88(_) PP_SEQ_SIZE_89
# define PP_SEQ_SIZE_89(_) PP_SEQ_SIZE_90
# define PP_SEQ_SIZE_90(_) PP_SEQ_SIZE_91
# define PP_SEQ_SIZE_91(_) PP_SEQ_SIZE_92
# define PP_SEQ_SIZE_92(_) PP_SEQ_SIZE_93
# define PP_SEQ_SIZE_93(_) PP_SEQ_SIZE_94
# define PP_SEQ_SIZE_94(_) PP_SEQ_SIZE_95
# define PP_SEQ_SIZE_95(_) PP_SEQ_SIZE_96
# define PP_SEQ_SIZE_96(_) PP_SEQ_SIZE_97
# define PP_SEQ_SIZE_97(_) PP_SEQ_SIZE_98
# define PP_SEQ_SIZE_98(_) PP_SEQ_SIZE_99
# define PP_SEQ_SIZE_99(_) PP_SEQ_SIZE_100
# define PP_SEQ_SIZE_100(_) PP_SEQ_SIZE_101
# define PP_SEQ_SIZE_101(_) PP_SEQ_SIZE_102
# define PP_SEQ_SIZE_102(_) PP_SEQ_SIZE_103
# define PP_SEQ_SIZE_103(_) PP_SEQ_SIZE_104
# define PP_SEQ_SIZE_104(_) PP_SEQ_SIZE_105
# define PP_SEQ_SIZE_105(_) PP_SEQ_SIZE_106
# define PP_SEQ_SIZE_106(_) PP_SEQ_SIZE_107
# define PP_SEQ_SIZE_107(_) PP_SEQ_SIZE_108
# define PP_SEQ_SIZE_108(_) PP_SEQ_SIZE_109
# define PP_SEQ_SIZE_109(_) PP_SEQ_SIZE_110
# define PP_SEQ_SIZE_110(_) PP_SEQ_SIZE_111
# define PP_SEQ_SIZE_111(_) PP_SEQ_SIZE_112
# define PP_SEQ_SIZE_112(_) PP_SEQ_SIZE_113
# define PP_SEQ_SIZE_113(_) PP_SEQ_SIZE_114
# define PP_SEQ_SIZE_114(_) PP_SEQ_SIZE_115
# define PP_SEQ_SIZE_115(_) PP_SEQ_SIZE_116
# define PP_SEQ_SIZE_116(_) PP_SEQ_SIZE_117
# define PP_SEQ_SIZE_117(_) PP_SEQ_SIZE_118
# define PP_SEQ_SIZE_118(_) PP_SEQ_SIZE_119
# define PP_SEQ_SIZE_119(_) PP_SEQ_SIZE_120
# define PP_SEQ_SIZE_120(_) PP_SEQ_SIZE_121
# define PP_SEQ_SIZE_121(_) PP_SEQ_SIZE_122
# define PP_SEQ_SIZE_122(_) PP_SEQ_SIZE_123
# define PP_SEQ_SIZE_123(_) PP_SEQ_SIZE_124
# define PP_SEQ_SIZE_124(_) PP_SEQ_SIZE_125
# define PP_SEQ_SIZE_125(_) PP_SEQ_SIZE_126
# define PP_SEQ_SIZE_126(_) PP_SEQ_SIZE_127
# define PP_SEQ_SIZE_127(_) PP_SEQ_SIZE_128
# define PP_SEQ_SIZE_128(_) PP_SEQ_SIZE_129
# define PP_SEQ_SIZE_129(_) PP_SEQ_SIZE_130
# define PP_SEQ_SIZE_130(_) PP_SEQ_SIZE_131
# define PP_SEQ_SIZE_131(_) PP_SEQ_SIZE_132
# define PP_SEQ_SIZE_132(_) PP_SEQ_SIZE_133
# define PP_SEQ_SIZE_133(_) PP_SEQ_SIZE_134
# define PP_SEQ_SIZE_134(_) PP_SEQ_SIZE_135
# define PP_SEQ_SIZE_135(_) PP_SEQ_SIZE_136
# define PP_SEQ_SIZE_136(_) PP_SEQ_SIZE_137
# define PP_SEQ_SIZE_137(_) PP_SEQ_SIZE_138
# define PP_SEQ_SIZE_138(_) PP_SEQ_SIZE_139
# define PP_SEQ_SIZE_139(_) PP_SEQ_SIZE_140
# define PP_SEQ_SIZE_140(_) PP_SEQ_SIZE_141
# define PP_SEQ_SIZE_141(_) PP_SEQ_SIZE_142
# define PP_SEQ_SIZE_142(_) PP_SEQ_SIZE_143
# define PP_SEQ_SIZE_143(_) PP_SEQ_SIZE_144
# define PP_SEQ_SIZE_144(_) PP_SEQ_SIZE_145
# define PP_SEQ_SIZE_145(_) PP_SEQ_SIZE_146
# define PP_SEQ_SIZE_146(_) PP_SEQ_SIZE_147
# define PP_SEQ_SIZE_147(_) PP_SEQ_SIZE_148
# define PP_SEQ_SIZE_148(_) PP_SEQ_SIZE_149
# define PP_SEQ_SIZE_149(_) PP_SEQ_SIZE_150
# define PP_SEQ_SIZE_150(_) PP_SEQ_SIZE_151
# define PP_SEQ_SIZE_151(_) PP_SEQ_SIZE_152
# define PP_SEQ_SIZE_152(_) PP_SEQ_SIZE_153
# define PP_SEQ_SIZE_153(_) PP_SEQ_SIZE_154
# define PP_SEQ_SIZE_154(_) PP_SEQ_SIZE_155
# define PP_SEQ_SIZE_155(_) PP_SEQ_SIZE_156
# define PP_SEQ_SIZE_156(_) PP_SEQ_SIZE_157
# define PP_SEQ_SIZE_157(_) PP_SEQ_SIZE_158
# define PP_SEQ_SIZE_158(_) PP_SEQ_SIZE_159
# define PP_SEQ_SIZE_159(_) PP_SEQ_SIZE_160
# define PP_SEQ_SIZE_160(_) PP_SEQ_SIZE_161
# define PP_SEQ_SIZE_161(_) PP_SEQ_SIZE_162
# define PP_SEQ_SIZE_162(_) PP_SEQ_SIZE_163
# define PP_SEQ_SIZE_163(_) PP_SEQ_SIZE_164
# define PP_SEQ_SIZE_164(_) PP_SEQ_SIZE_165
# define PP_SEQ_SIZE_165(_) PP_SEQ_SIZE_166
# define PP_SEQ_SIZE_166(_) PP_SEQ_SIZE_167
# define PP_SEQ_SIZE_167(_) PP_SEQ_SIZE_168
# define PP_SEQ_SIZE_168(_) PP_SEQ_SIZE_169
# define PP_SEQ_SIZE_169(_) PP_SEQ_SIZE_170
# define PP_SEQ_SIZE_170(_) PP_SEQ_SIZE_171
# define PP_SEQ_SIZE_171(_) PP_SEQ_SIZE_172
# define PP_SEQ_SIZE_172(_) PP_SEQ_SIZE_173
# define PP_SEQ_SIZE_173(_) PP_SEQ_SIZE_174
# define PP_SEQ_SIZE_174(_) PP_SEQ_SIZE_175
# define PP_SEQ_SIZE_175(_) PP_SEQ_SIZE_176
# define PP_SEQ_SIZE_176(_) PP_SEQ_SIZE_177
# define PP_SEQ_SIZE_177(_) PP_SEQ_SIZE_178
# define PP_SEQ_SIZE_178(_) PP_SEQ_SIZE_179
# define PP_SEQ_SIZE_179(_) PP_SEQ_SIZE_180
# define PP_SEQ_SIZE_180(_) PP_SEQ_SIZE_181
# define PP_SEQ_SIZE_181(_) PP_SEQ_SIZE_182
# define PP_SEQ_SIZE_182(_) PP_SEQ_SIZE_183
# define PP_SEQ_SIZE_183(_) PP_SEQ_SIZE_184
# define PP_SEQ_SIZE_184(_) PP_SEQ_SIZE_185
# define PP_SEQ_SIZE_185(_) PP_SEQ_SIZE_186
# define PP_SEQ_SIZE_186(_) PP_SEQ_SIZE_187
# define PP_SEQ_SIZE_187(_) PP_SEQ_SIZE_188
# define PP_SEQ_SIZE_188(_) PP_SEQ_SIZE_189
# define PP_SEQ_SIZE_189(_) PP_SEQ_SIZE_190
# define PP_SEQ_SIZE_190(_) PP_SEQ_SIZE_191
# define PP_SEQ_SIZE_191(_) PP_SEQ_SIZE_192
# define PP_SEQ_SIZE_192(_) PP_SEQ_SIZE_193
# define PP_SEQ_SIZE_193(_) PP_SEQ_SIZE_194
# define PP_SEQ_SIZE_194(_) PP_SEQ_SIZE_195
# define PP_SEQ_SIZE_195(_) PP_SEQ_SIZE_196
# define PP_SEQ_SIZE_196(_) PP_SEQ_SIZE_197
# define PP_SEQ_SIZE_197(_) PP_SEQ_SIZE_198
# define PP_SEQ_SIZE_198(_) PP_SEQ_SIZE_199
# define PP_SEQ_SIZE_199(_) PP_SEQ_SIZE_200
# define PP_SEQ_SIZE_200(_) PP_SEQ_SIZE_201
# define PP_SEQ_SIZE_201(_) PP_SEQ_SIZE_202
# define PP_SEQ_SIZE_202(_) PP_SEQ_SIZE_203
# define PP_SEQ_SIZE_203(_) PP_SEQ_SIZE_204
# define PP_SEQ_SIZE_204(_) PP_SEQ_SIZE_205
# define PP_SEQ_SIZE_205(_) PP_SEQ_SIZE_206
# define PP_SEQ_SIZE_206(_) PP_SEQ_SIZE_207
# define PP_SEQ_SIZE_207(_) PP_SEQ_SIZE_208
# define PP_SEQ_SIZE_208(_) PP_SEQ_SIZE_209
# define PP_SEQ_SIZE_209(_) PP_SEQ_SIZE_210
# define PP_SEQ_SIZE_210(_) PP_SEQ_SIZE_211
# define PP_SEQ_SIZE_211(_) PP_SEQ_SIZE_212
# define PP_SEQ_SIZE_212(_) PP_SEQ_SIZE_213
# define PP_SEQ_SIZE_213(_) PP_SEQ_SIZE_214
# define PP_SEQ_SIZE_214(_) PP_SEQ_SIZE_215
# define PP_SEQ_SIZE_215(_) PP_SEQ_SIZE_216
# define PP_SEQ_SIZE_216(_) PP_SEQ_SIZE_217
# define PP_SEQ_SIZE_217(_) PP_SEQ_SIZE_218
# define PP_SEQ_SIZE_218(_) PP_SEQ_SIZE_219
# define PP_SEQ_SIZE_219(_) PP_SEQ_SIZE_220
# define PP_SEQ_SIZE_220(_) PP_SEQ_SIZE_221
# define PP_SEQ_SIZE_221(_) PP_SEQ_SIZE_222
# define PP_SEQ_SIZE_222(_) PP_SEQ_SIZE_223
# define PP_SEQ_SIZE_223(_) PP_SEQ_SIZE_224
# define PP_SEQ_SIZE_224(_) PP_SEQ_SIZE_225
# define PP_SEQ_SIZE_225(_) PP_SEQ_SIZE_226
# define PP_SEQ_SIZE_226(_) PP_SEQ_SIZE_227
# define PP_SEQ_SIZE_227(_) PP_SEQ_SIZE_228
# define PP_SEQ_SIZE_228(_) PP_SEQ_SIZE_229
# define PP_SEQ_SIZE_229(_) PP_SEQ_SIZE_230
# define PP_SEQ_SIZE_230(_) PP_SEQ_SIZE_231
# define PP_SEQ_SIZE_231(_) PP_SEQ_SIZE_232
# define PP_SEQ_SIZE_232(_) PP_SEQ_SIZE_233
# define PP_SEQ_SIZE_233(_) PP_SEQ_SIZE_234
# define PP_SEQ_SIZE_234(_) PP_SEQ_SIZE_235
# define PP_SEQ_SIZE_235(_) PP_SEQ_SIZE_236
# define PP_SEQ_SIZE_236(_) PP_SEQ_SIZE_237
# define PP_SEQ_SIZE_237(_) PP_SEQ_SIZE_238
# define PP_SEQ_SIZE_238(_) PP_SEQ_SIZE_239
# define PP_SEQ_SIZE_239(_) PP_SEQ_SIZE_240
# define PP_SEQ_SIZE_240(_) PP_SEQ_SIZE_241
# define PP_SEQ_SIZE_241(_) PP_SEQ_SIZE_242
# define PP_SEQ_SIZE_242(_) PP_SEQ_SIZE_243
# define PP_SEQ_SIZE_243(_) PP_SEQ_SIZE_244
# define PP_SEQ_SIZE_244(_) PP_SEQ_SIZE_245
# define PP_SEQ_SIZE_245(_) PP_SEQ_SIZE_246
# define PP_SEQ_SIZE_246(_) PP_SEQ_SIZE_247
# define PP_SEQ_SIZE_247(_) PP_SEQ_SIZE_248
# define PP_SEQ_SIZE_248(_) PP_SEQ_SIZE_249
# define PP_SEQ_SIZE_249(_) PP_SEQ_SIZE_250
# define PP_SEQ_SIZE_250(_) PP_SEQ_SIZE_251
# define PP_SEQ_SIZE_251(_) PP_SEQ_SIZE_252
# define PP_SEQ_SIZE_252(_) PP_SEQ_SIZE_253
# define PP_SEQ_SIZE_253(_) PP_SEQ_SIZE_254
# define PP_SEQ_SIZE_254(_) PP_SEQ_SIZE_255
# define PP_SEQ_SIZE_255(_) PP_SEQ_SIZE_256
# define PP_SEQ_SIZE_256(_) PP_SEQ_SIZE_257
#
# define PP_SEQ_SIZE_PP_SEQ_SIZE_0 0
# define PP_SEQ_SIZE_PP_SEQ_SIZE_1 1
# define PP_SEQ_SIZE_PP_SEQ_SIZE_2 2
# define PP_SEQ_SIZE_PP_SEQ_SIZE_3 3
# define PP_SEQ_SIZE_PP_SEQ_SIZE_4 4
# define PP_SEQ_SIZE_PP_SEQ_SIZE_5 5
# define PP_SEQ_SIZE_PP_SEQ_SIZE_6 6
# define PP_SEQ_SIZE_PP_SEQ_SIZE_7 7
# define PP_SEQ_SIZE_PP_SEQ_SIZE_8 8
# define PP_SEQ_SIZE_PP_SEQ_SIZE_9 9
# define PP_SEQ_SIZE_PP_SEQ_SIZE_10 10
# define PP_SEQ_SIZE_PP_SEQ_SIZE_11 11
# define PP_SEQ_SIZE_PP_SEQ_SIZE_12 12
# define PP_SEQ_SIZE_PP_SEQ_SIZE_13 13
# define PP_SEQ_SIZE_PP_SEQ_SIZE_14 14
# define PP_SEQ_SIZE_PP_SEQ_SIZE_15 15
# define PP_SEQ_SIZE_PP_SEQ_SIZE_16 16
# define PP_SEQ_SIZE_PP_SEQ_SIZE_17 17
# define PP_SEQ_SIZE_PP_SEQ_SIZE_18 18
# define PP_SEQ_SIZE_PP_SEQ_SIZE_19 19
# define PP_SEQ_SIZE_PP_SEQ_SIZE_20 20
# define PP_SEQ_SIZE_PP_SEQ_SIZE_21 21
# define PP_SEQ_SIZE_PP_SEQ_SIZE_22 22
# define PP_SEQ_SIZE_PP_SEQ_SIZE_23 23
# define PP_SEQ_SIZE_PP_SEQ_SIZE_24 24
# define PP_SEQ_SIZE_PP_SEQ_SIZE_25 25
# define PP_SEQ_SIZE_PP_SEQ_SIZE_26 26
# define PP_SEQ_SIZE_PP_SEQ_SIZE_27 27
# define PP_SEQ_SIZE_PP_SEQ_SIZE_28 28
# define PP_SEQ_SIZE_PP_SEQ_SIZE_29 29
# define PP_SEQ_SIZE_PP_SEQ_SIZE_30 30
# define PP_SEQ_SIZE_PP_SEQ_SIZE_31 31
# define PP_SEQ_SIZE_PP_SEQ_SIZE_32 32
# define PP_SEQ_SIZE_PP_SEQ_SIZE_33 33
# define PP_SEQ_SIZE_PP_SEQ_SIZE_34 34
# define PP_SEQ_SIZE_PP_SEQ_SIZE_35 35
# define PP_SEQ_SIZE_PP_SEQ_SIZE_36 36
# define PP_SEQ_SIZE_PP_SEQ_SIZE_37 37
# define PP_SEQ_SIZE_PP_SEQ_SIZE_38 38
# define PP_SEQ_SIZE_PP_SEQ_SIZE_39 39
# define PP_SEQ_SIZE_PP_SEQ_SIZE_40 40
# define PP_SEQ_SIZE_PP_SEQ_SIZE_41 41
# define PP_SEQ_SIZE_PP_SEQ_SIZE_42 42
# define PP_SEQ_SIZE_PP_SEQ_SIZE_43 43
# define PP_SEQ_SIZE_PP_SEQ_SIZE_44 44
# define PP_SEQ_SIZE_PP_SEQ_SIZE_45 45
# define PP_SEQ_SIZE_PP_SEQ_SIZE_46 46
# define PP_SEQ_SIZE_PP_SEQ_SIZE_47 47
# define PP_SEQ_SIZE_PP_SEQ_SIZE_48 48
# define PP_SEQ_SIZE_PP_SEQ_SIZE_49 49
# define PP_SEQ_SIZE_PP_SEQ_SIZE_50 50
# define PP_SEQ_SIZE_PP_SEQ_SIZE_51 51
# define PP_SEQ_SIZE_PP_SEQ_SIZE_52 52
# define PP_SEQ_SIZE_PP_SEQ_SIZE_53 53
# define PP_SEQ_SIZE_PP_SEQ_SIZE_54 54
# define PP_SEQ_SIZE_PP_SEQ_SIZE_55 55
# define PP_SEQ_SIZE_PP_SEQ_SIZE_56 56
# define PP_SEQ_SIZE_PP_SEQ_SIZE_57 57
# define PP_SEQ_SIZE_PP_SEQ_SIZE_58 58
# define PP_SEQ_SIZE_PP_SEQ_SIZE_59 59
# define PP_SEQ_SIZE_PP_SEQ_SIZE_60 60
# define PP_SEQ_SIZE_PP_SEQ_SIZE_61 61
# define PP_SEQ_SIZE_PP_SEQ_SIZE_62 62
# define PP_SEQ_SIZE_PP_SEQ_SIZE_63 63
# define PP_SEQ_SIZE_PP_SEQ_SIZE_64 64
# define PP_SEQ_SIZE_PP_SEQ_SIZE_65 65
# define PP_SEQ_SIZE_PP_SEQ_SIZE_66 66
# define PP_SEQ_SIZE_PP_SEQ_SIZE_67 67
# define PP_SEQ_SIZE_PP_SEQ_SIZE_68 68
# define PP_SEQ_SIZE_PP_SEQ_SIZE_69 69
# define PP_SEQ_SIZE_PP_SEQ_SIZE_70 70
# define PP_SEQ_SIZE_PP_SEQ_SIZE_71 71
# define PP_SEQ_SIZE_PP_SEQ_SIZE_72 72
# define PP_SEQ_SIZE_PP_SEQ_SIZE_73 73
# define PP_SEQ_SIZE_PP_SEQ_SIZE_74 74
# define PP_SEQ_SIZE_PP_SEQ_SIZE_75 75
# define PP_SEQ_SIZE_PP_SEQ_SIZE_76 76
# define PP_SEQ_SIZE_PP_SEQ_SIZE_77 77
# define PP_SEQ_SIZE_PP_SEQ_SIZE_78 78
# define PP_SEQ_SIZE_PP_SEQ_SIZE_79 79
# define PP_SEQ_SIZE_PP_SEQ_SIZE_80 80
# define PP_SEQ_SIZE_PP_SEQ_SIZE_81 81
# define PP_SEQ_SIZE_PP_SEQ_SIZE_82 82
# define PP_SEQ_SIZE_PP_SEQ_SIZE_83 83
# define PP_SEQ_SIZE_PP_SEQ_SIZE_84 84
# define PP_SEQ_SIZE_PP_SEQ_SIZE_85 85
# define PP_SEQ_SIZE_PP_SEQ_SIZE_86 86
# define PP_SEQ_SIZE_PP_SEQ_SIZE_87 87
# define PP_SEQ_SIZE_PP_SEQ_SIZE_88 88
# define PP_SEQ_SIZE_PP_SEQ_SIZE_89 89
# define PP_SEQ_SIZE_PP_SEQ_SIZE_90 90
# define PP_SEQ_SIZE_PP_SEQ_SIZE_91 91
# define PP_SEQ_SIZE_PP_SEQ_SIZE_92 92
# define PP_SEQ_SIZE_PP_SEQ_SIZE_93 93
# define PP_SEQ_SIZE_PP_SEQ_SIZE_94 94
# define PP_SEQ_SIZE_PP_SEQ_SIZE_95 95
# define PP_SEQ_SIZE_PP_SEQ_SIZE_96 96
# define PP_SEQ_SIZE_PP_SEQ_SIZE_97 97
# define PP_SEQ_SIZE_PP_SEQ_SIZE_98 98
# define PP_SEQ_SIZE_PP_SEQ_SIZE_99 99
# define PP_SEQ_SIZE_PP_SEQ_SIZE_100 100
# define PP_SEQ_SIZE_PP_SEQ_SIZE_101 101
# define PP_SEQ_SIZE_PP_SEQ_SIZE_102 102
# define PP_SEQ_SIZE_PP_SEQ_SIZE_103 103
# define PP_SEQ_SIZE_PP_SEQ_SIZE_104 104
# define PP_SEQ_SIZE_PP_SEQ_SIZE_105 105
# define PP_SEQ_SIZE_PP_SEQ_SIZE_106 106
# define PP_SEQ_SIZE_PP_SEQ_SIZE_107 107
# define PP_SEQ_SIZE_PP_SEQ_SIZE_108 108
# define PP_SEQ_SIZE_PP_SEQ_SIZE_109 109
# define PP_SEQ_SIZE_PP_SEQ_SIZE_110 110
# define PP_SEQ_SIZE_PP_SEQ_SIZE_111 111
# define PP_SEQ_SIZE_PP_SEQ_SIZE_112 112
# define PP_SEQ_SIZE_PP_SEQ_SIZE_113 113
# define PP_SEQ_SIZE_PP_SEQ_SIZE_114 114
# define PP_SEQ_SIZE_PP_SEQ_SIZE_115 115
# define PP_SEQ_SIZE_PP_SEQ_SIZE_116 116
# define PP_SEQ_SIZE_PP_SEQ_SIZE_117 117
# define PP_SEQ_SIZE_PP_SEQ_SIZE_118 118
# define PP_SEQ_SIZE_PP_SEQ_SIZE_119 119
# define PP_SEQ_SIZE_PP_SEQ_SIZE_120 120
# define PP_SEQ_SIZE_PP_SEQ_SIZE_121 121
# define PP_SEQ_SIZE_PP_SEQ_SIZE_122 122
# define PP_SEQ_SIZE_PP_SEQ_SIZE_123 123
# define PP_SEQ_SIZE_PP_SEQ_SIZE_124 124
# define PP_SEQ_SIZE_PP_SEQ_SIZE_125 125
# define PP_SEQ_SIZE_PP_SEQ_SIZE_126 126
# define PP_SEQ_SIZE_PP_SEQ_SIZE_127 127
# define PP_SEQ_SIZE_PP_SEQ_SIZE_128 128
# define PP_SEQ_SIZE_PP_SEQ_SIZE_129 129
# define PP_SEQ_SIZE_PP_SEQ_SIZE_130 130
# define PP_SEQ_SIZE_PP_SEQ_SIZE_131 131
# define PP_SEQ_SIZE_PP_SEQ_SIZE_132 132
# define PP_SEQ_SIZE_PP_SEQ_SIZE_133 133
# define PP_SEQ_SIZE_PP_SEQ_SIZE_134 134
# define PP_SEQ_SIZE_PP_SEQ_SIZE_135 135
# define PP_SEQ_SIZE_PP_SEQ_SIZE_136 136
# define PP_SEQ_SIZE_PP_SEQ_SIZE_137 137
# define PP_SEQ_SIZE_PP_SEQ_SIZE_138 138
# define PP_SEQ_SIZE_PP_SEQ_SIZE_139 139
# define PP_SEQ_SIZE_PP_SEQ_SIZE_140 140
# define PP_SEQ_SIZE_PP_SEQ_SIZE_141 141
# define PP_SEQ_SIZE_PP_SEQ_SIZE_142 142
# define PP_SEQ_SIZE_PP_SEQ_SIZE_143 143
# define PP_SEQ_SIZE_PP_SEQ_SIZE_144 144
# define PP_SEQ_SIZE_PP_SEQ_SIZE_145 145
# define PP_SEQ_SIZE_PP_SEQ_SIZE_146 146
# define PP_SEQ_SIZE_PP_SEQ_SIZE_147 147
# define PP_SEQ_SIZE_PP_SEQ_SIZE_148 148
# define PP_SEQ_SIZE_PP_SEQ_SIZE_149 149
# define PP_SEQ_SIZE_PP_SEQ_SIZE_150 150
# define PP_SEQ_SIZE_PP_SEQ_SIZE_151 151
# define PP_SEQ_SIZE_PP_SEQ_SIZE_152 152
# define PP_SEQ_SIZE_PP_SEQ_SIZE_153 153
# define PP_SEQ_SIZE_PP_SEQ_SIZE_154 154
# define PP_SEQ_SIZE_PP_SEQ_SIZE_155 155
# define PP_SEQ_SIZE_PP_SEQ_SIZE_156 156
# define PP_SEQ_SIZE_PP_SEQ_SIZE_157 157
# define PP_SEQ_SIZE_PP_SEQ_SIZE_158 158
# define PP_SEQ_SIZE_PP_SEQ_SIZE_159 159
# define PP_SEQ_SIZE_PP_SEQ_SIZE_160 160
# define PP_SEQ_SIZE_PP_SEQ_SIZE_161 161
# define PP_SEQ_SIZE_PP_SEQ_SIZE_162 162
# define PP_SEQ_SIZE_PP_SEQ_SIZE_163 163
# define PP_SEQ_SIZE_PP_SEQ_SIZE_164 164
# define PP_SEQ_SIZE_PP_SEQ_SIZE_165 165
# define PP_SEQ_SIZE_PP_SEQ_SIZE_166 166
# define PP_SEQ_SIZE_PP_SEQ_SIZE_167 167
# define PP_SEQ_SIZE_PP_SEQ_SIZE_168 168
# define PP_SEQ_SIZE_PP_SEQ_SIZE_169 169
# define PP_SEQ_SIZE_PP_SEQ_SIZE_170 170
# define PP_SEQ_SIZE_PP_SEQ_SIZE_171 171
# define PP_SEQ_SIZE_PP_SEQ_SIZE_172 172
# define PP_SEQ_SIZE_PP_SEQ_SIZE_173 173
# define PP_SEQ_SIZE_PP_SEQ_SIZE_174 174
# define PP_SEQ_SIZE_PP_SEQ_SIZE_175 175
# define PP_SEQ_SIZE_PP_SEQ_SIZE_176 176
# define PP_SEQ_SIZE_PP_SEQ_SIZE_177 177
# define PP_SEQ_SIZE_PP_SEQ_SIZE_178 178
# define PP_SEQ_SIZE_PP_SEQ_SIZE_179 179
# define PP_SEQ_SIZE_PP_SEQ_SIZE_180 180
# define PP_SEQ_SIZE_PP_SEQ_SIZE_181 181
# define PP_SEQ_SIZE_PP_SEQ_SIZE_182 182
# define PP_SEQ_SIZE_PP_SEQ_SIZE_183 183
# define PP_SEQ_SIZE_PP_SEQ_SIZE_184 184
# define PP_SEQ_SIZE_PP_SEQ_SIZE_185 185
# define PP_SEQ_SIZE_PP_SEQ_SIZE_186 186
# define PP_SEQ_SIZE_PP_SEQ_SIZE_187 187
# define PP_SEQ_SIZE_PP_SEQ_SIZE_188 188
# define PP_SEQ_SIZE_PP_SEQ_SIZE_189 189
# define PP_SEQ_SIZE_PP_SEQ_SIZE_190 190
# define PP_SEQ_SIZE_PP_SEQ_SIZE_191 191
# define PP_SEQ_SIZE_PP_SEQ_SIZE_192 192
# define PP_SEQ_SIZE_PP_SEQ_SIZE_193 193
# define PP_SEQ_SIZE_PP_SEQ_SIZE_194 194
# define PP_SEQ_SIZE_PP_SEQ_SIZE_195 195
# define PP_SEQ_SIZE_PP_SEQ_SIZE_196 196
# define PP_SEQ_SIZE_PP_SEQ_SIZE_197 197
# define PP_SEQ_SIZE_PP_SEQ_SIZE_198 198
# define PP_SEQ_SIZE_PP_SEQ_SIZE_199 199
# define PP_SEQ_SIZE_PP_SEQ_SIZE_200 200
# define PP_SEQ_SIZE_PP_SEQ_SIZE_201 201
# define PP_SEQ_SIZE_PP_SEQ_SIZE_202 202
# define PP_SEQ_SIZE_PP_SEQ_SIZE_203 203
# define PP_SEQ_SIZE_PP_SEQ_SIZE_204 204
# define PP_SEQ_SIZE_PP_SEQ_SIZE_205 205
# define PP_SEQ_SIZE_PP_SEQ_SIZE_206 206
# define PP_SEQ_SIZE_PP_SEQ_SIZE_207 207
# define PP_SEQ_SIZE_PP_SEQ_SIZE_208 208
# define PP_SEQ_SIZE_PP_SEQ_SIZE_209 209
# define PP_SEQ_SIZE_PP_SEQ_SIZE_210 210
# define PP_SEQ_SIZE_PP_SEQ_SIZE_211 211
# define PP_SEQ_SIZE_PP_SEQ_SIZE_212 212
# define PP_SEQ_SIZE_PP_SEQ_SIZE_213 213
# define PP_SEQ_SIZE_PP_SEQ_SIZE_214 214
# define PP_SEQ_SIZE_PP_SEQ_SIZE_215 215
# define PP_SEQ_SIZE_PP_SEQ_SIZE_216 216
# define PP_SEQ_SIZE_PP_SEQ_SIZE_217 217
# define PP_SEQ_SIZE_PP_SEQ_SIZE_218 218
# define PP_SEQ_SIZE_PP_SEQ_SIZE_219 219
# define PP_SEQ_SIZE_PP_SEQ_SIZE_220 220
# define PP_SEQ_SIZE_PP_SEQ_SIZE_221 221
# define PP_SEQ_SIZE_PP_SEQ_SIZE_222 222
# define PP_SEQ_SIZE_PP_SEQ_SIZE_223 223
# define PP_SEQ_SIZE_PP_SEQ_SIZE_224 224
# define PP_SEQ_SIZE_PP_SEQ_SIZE_225 225
# define PP_SEQ_SIZE_PP_SEQ_SIZE_226 226
# define PP_SEQ_SIZE_PP_SEQ_SIZE_227 227
# define PP_SEQ_SIZE_PP_SEQ_SIZE_228 228
# define PP_SEQ_SIZE_PP_SEQ_SIZE_229 229
# define PP_SEQ_SIZE_PP_SEQ_SIZE_230 230
# define PP_SEQ_SIZE_PP_SEQ_SIZE_231 231
# define PP_SEQ_SIZE_PP_SEQ_SIZE_232 232
# define PP_SEQ_SIZE_PP_SEQ_SIZE_233 233
# define PP_SEQ_SIZE_PP_SEQ_SIZE_234 234
# define PP_SEQ_SIZE_PP_SEQ_SIZE_235 235
# define PP_SEQ_SIZE_PP_SEQ_SIZE_236 236
# define PP_SEQ_SIZE_PP_SEQ_SIZE_237 237
# define PP_SEQ_SIZE_PP_SEQ_SIZE_238 238
# define PP_SEQ_SIZE_PP_SEQ_SIZE_239 239
# define PP_SEQ_SIZE_PP_SEQ_SIZE_240 240
# define PP_SEQ_SIZE_PP_SEQ_SIZE_241 241
# define PP_SEQ_SIZE_PP_SEQ_SIZE_242 242
# define PP_SEQ_SIZE_PP_SEQ_SIZE_243 243
# define PP_SEQ_SIZE_PP_SEQ_SIZE_244 244
# define PP_SEQ_SIZE_PP_SEQ_SIZE_245 245
# define PP_SEQ_SIZE_PP_SEQ_SIZE_246 246
# define PP_SEQ_SIZE_PP_SEQ_SIZE_247 247
# define PP_SEQ_SIZE_PP_SEQ_SIZE_248 248
# define PP_SEQ_SIZE_PP_SEQ_SIZE_249 249
# define PP_SEQ_SIZE_PP_SEQ_SIZE_250 250
# define PP_SEQ_SIZE_PP_SEQ_SIZE_251 251
# define PP_SEQ_SIZE_PP_SEQ_SIZE_252 252
# define PP_SEQ_SIZE_PP_SEQ_SIZE_253 253
# define PP_SEQ_SIZE_PP_SEQ_SIZE_254 254
# define PP_SEQ_SIZE_PP_SEQ_SIZE_255 255
# define PP_SEQ_SIZE_PP_SEQ_SIZE_256 256
# define PP_SEQ_SIZE_PP_SEQ_SIZE_257 257
#
# endif
