# if !defined(PP_LOCAL_LIMITS)
#    error PP_ERROR:  local iteration boundaries are not defined
# elif !defined(PP_LOCAL_MACRO)
#    error PP_ERROR:  local iteration target macro is not defined
# else
#    if ~PP_CONFIG_FLAGS() & PP_CONFIG_EDG()
#        define PP_LOCAL_S PP_TUPLE_ELEM(2, 0, PP_LOCAL_LIMITS)
#        define PP_LOCAL_F PP_TUPLE_ELEM(2, 1, PP_LOCAL_LIMITS)
#    else
#        define PP_VALUE PP_TUPLE_ELEM(2, 0, PP_LOCAL_LIMITS)
#        include <takisy/preprocessor/iteration/detail/start.hpp>
#        define PP_VALUE PP_TUPLE_ELEM(2, 1, PP_LOCAL_LIMITS)
#        include <takisy/preprocessor/iteration/detail/finish.hpp>
#        define PP_LOCAL_S PP_LOCAL_SE()
#        define PP_LOCAL_F PP_LOCAL_FE()
#    endif
# endif
#
# if (PP_LOCAL_S) > (PP_LOCAL_F)
#    include <takisy/preprocessor/iteration/detail/rlocal.hpp>
# else
#    if PP_LOCAL_C(0)
        PP_LOCAL_MACRO(0)
#    endif
#    if PP_LOCAL_C(1)
        PP_LOCAL_MACRO(1)
#    endif
#    if PP_LOCAL_C(2)
        PP_LOCAL_MACRO(2)
#    endif
#    if PP_LOCAL_C(3)
        PP_LOCAL_MACRO(3)
#    endif
#    if PP_LOCAL_C(4)
        PP_LOCAL_MACRO(4)
#    endif
#    if PP_LOCAL_C(5)
        PP_LOCAL_MACRO(5)
#    endif
#    if PP_LOCAL_C(6)
        PP_LOCAL_MACRO(6)
#    endif
#    if PP_LOCAL_C(7)
        PP_LOCAL_MACRO(7)
#    endif
#    if PP_LOCAL_C(8)
        PP_LOCAL_MACRO(8)
#    endif
#    if PP_LOCAL_C(9)
        PP_LOCAL_MACRO(9)
#    endif
#    if PP_LOCAL_C(10)
        PP_LOCAL_MACRO(10)
#    endif
#    if PP_LOCAL_C(11)
        PP_LOCAL_MACRO(11)
#    endif
#    if PP_LOCAL_C(12)
        PP_LOCAL_MACRO(12)
#    endif
#    if PP_LOCAL_C(13)
        PP_LOCAL_MACRO(13)
#    endif
#    if PP_LOCAL_C(14)
        PP_LOCAL_MACRO(14)
#    endif
#    if PP_LOCAL_C(15)
        PP_LOCAL_MACRO(15)
#    endif
#    if PP_LOCAL_C(16)
        PP_LOCAL_MACRO(16)
#    endif
#    if PP_LOCAL_C(17)
        PP_LOCAL_MACRO(17)
#    endif
#    if PP_LOCAL_C(18)
        PP_LOCAL_MACRO(18)
#    endif
#    if PP_LOCAL_C(19)
        PP_LOCAL_MACRO(19)
#    endif
#    if PP_LOCAL_C(20)
        PP_LOCAL_MACRO(20)
#    endif
#    if PP_LOCAL_C(21)
        PP_LOCAL_MACRO(21)
#    endif
#    if PP_LOCAL_C(22)
        PP_LOCAL_MACRO(22)
#    endif
#    if PP_LOCAL_C(23)
        PP_LOCAL_MACRO(23)
#    endif
#    if PP_LOCAL_C(24)
        PP_LOCAL_MACRO(24)
#    endif
#    if PP_LOCAL_C(25)
        PP_LOCAL_MACRO(25)
#    endif
#    if PP_LOCAL_C(26)
        PP_LOCAL_MACRO(26)
#    endif
#    if PP_LOCAL_C(27)
        PP_LOCAL_MACRO(27)
#    endif
#    if PP_LOCAL_C(28)
        PP_LOCAL_MACRO(28)
#    endif
#    if PP_LOCAL_C(29)
        PP_LOCAL_MACRO(29)
#    endif
#    if PP_LOCAL_C(30)
        PP_LOCAL_MACRO(30)
#    endif
#    if PP_LOCAL_C(31)
        PP_LOCAL_MACRO(31)
#    endif
#    if PP_LOCAL_C(32)
        PP_LOCAL_MACRO(32)
#    endif
#    if PP_LOCAL_C(33)
        PP_LOCAL_MACRO(33)
#    endif
#    if PP_LOCAL_C(34)
        PP_LOCAL_MACRO(34)
#    endif
#    if PP_LOCAL_C(35)
        PP_LOCAL_MACRO(35)
#    endif
#    if PP_LOCAL_C(36)
        PP_LOCAL_MACRO(36)
#    endif
#    if PP_LOCAL_C(37)
        PP_LOCAL_MACRO(37)
#    endif
#    if PP_LOCAL_C(38)
        PP_LOCAL_MACRO(38)
#    endif
#    if PP_LOCAL_C(39)
        PP_LOCAL_MACRO(39)
#    endif
#    if PP_LOCAL_C(40)
        PP_LOCAL_MACRO(40)
#    endif
#    if PP_LOCAL_C(41)
        PP_LOCAL_MACRO(41)
#    endif
#    if PP_LOCAL_C(42)
        PP_LOCAL_MACRO(42)
#    endif
#    if PP_LOCAL_C(43)
        PP_LOCAL_MACRO(43)
#    endif
#    if PP_LOCAL_C(44)
        PP_LOCAL_MACRO(44)
#    endif
#    if PP_LOCAL_C(45)
        PP_LOCAL_MACRO(45)
#    endif
#    if PP_LOCAL_C(46)
        PP_LOCAL_MACRO(46)
#    endif
#    if PP_LOCAL_C(47)
        PP_LOCAL_MACRO(47)
#    endif
#    if PP_LOCAL_C(48)
        PP_LOCAL_MACRO(48)
#    endif
#    if PP_LOCAL_C(49)
        PP_LOCAL_MACRO(49)
#    endif
#    if PP_LOCAL_C(50)
        PP_LOCAL_MACRO(50)
#    endif
#    if PP_LOCAL_C(51)
        PP_LOCAL_MACRO(51)
#    endif
#    if PP_LOCAL_C(52)
        PP_LOCAL_MACRO(52)
#    endif
#    if PP_LOCAL_C(53)
        PP_LOCAL_MACRO(53)
#    endif
#    if PP_LOCAL_C(54)
        PP_LOCAL_MACRO(54)
#    endif
#    if PP_LOCAL_C(55)
        PP_LOCAL_MACRO(55)
#    endif
#    if PP_LOCAL_C(56)
        PP_LOCAL_MACRO(56)
#    endif
#    if PP_LOCAL_C(57)
        PP_LOCAL_MACRO(57)
#    endif
#    if PP_LOCAL_C(58)
        PP_LOCAL_MACRO(58)
#    endif
#    if PP_LOCAL_C(59)
        PP_LOCAL_MACRO(59)
#    endif
#    if PP_LOCAL_C(60)
        PP_LOCAL_MACRO(60)
#    endif
#    if PP_LOCAL_C(61)
        PP_LOCAL_MACRO(61)
#    endif
#    if PP_LOCAL_C(62)
        PP_LOCAL_MACRO(62)
#    endif
#    if PP_LOCAL_C(63)
        PP_LOCAL_MACRO(63)
#    endif
#    if PP_LOCAL_C(64)
        PP_LOCAL_MACRO(64)
#    endif
#    if PP_LOCAL_C(65)
        PP_LOCAL_MACRO(65)
#    endif
#    if PP_LOCAL_C(66)
        PP_LOCAL_MACRO(66)
#    endif
#    if PP_LOCAL_C(67)
        PP_LOCAL_MACRO(67)
#    endif
#    if PP_LOCAL_C(68)
        PP_LOCAL_MACRO(68)
#    endif
#    if PP_LOCAL_C(69)
        PP_LOCAL_MACRO(69)
#    endif
#    if PP_LOCAL_C(70)
        PP_LOCAL_MACRO(70)
#    endif
#    if PP_LOCAL_C(71)
        PP_LOCAL_MACRO(71)
#    endif
#    if PP_LOCAL_C(72)
        PP_LOCAL_MACRO(72)
#    endif
#    if PP_LOCAL_C(73)
        PP_LOCAL_MACRO(73)
#    endif
#    if PP_LOCAL_C(74)
        PP_LOCAL_MACRO(74)
#    endif
#    if PP_LOCAL_C(75)
        PP_LOCAL_MACRO(75)
#    endif
#    if PP_LOCAL_C(76)
        PP_LOCAL_MACRO(76)
#    endif
#    if PP_LOCAL_C(77)
        PP_LOCAL_MACRO(77)
#    endif
#    if PP_LOCAL_C(78)
        PP_LOCAL_MACRO(78)
#    endif
#    if PP_LOCAL_C(79)
        PP_LOCAL_MACRO(79)
#    endif
#    if PP_LOCAL_C(80)
        PP_LOCAL_MACRO(80)
#    endif
#    if PP_LOCAL_C(81)
        PP_LOCAL_MACRO(81)
#    endif
#    if PP_LOCAL_C(82)
        PP_LOCAL_MACRO(82)
#    endif
#    if PP_LOCAL_C(83)
        PP_LOCAL_MACRO(83)
#    endif
#    if PP_LOCAL_C(84)
        PP_LOCAL_MACRO(84)
#    endif
#    if PP_LOCAL_C(85)
        PP_LOCAL_MACRO(85)
#    endif
#    if PP_LOCAL_C(86)
        PP_LOCAL_MACRO(86)
#    endif
#    if PP_LOCAL_C(87)
        PP_LOCAL_MACRO(87)
#    endif
#    if PP_LOCAL_C(88)
        PP_LOCAL_MACRO(88)
#    endif
#    if PP_LOCAL_C(89)
        PP_LOCAL_MACRO(89)
#    endif
#    if PP_LOCAL_C(90)
        PP_LOCAL_MACRO(90)
#    endif
#    if PP_LOCAL_C(91)
        PP_LOCAL_MACRO(91)
#    endif
#    if PP_LOCAL_C(92)
        PP_LOCAL_MACRO(92)
#    endif
#    if PP_LOCAL_C(93)
        PP_LOCAL_MACRO(93)
#    endif
#    if PP_LOCAL_C(94)
        PP_LOCAL_MACRO(94)
#    endif
#    if PP_LOCAL_C(95)
        PP_LOCAL_MACRO(95)
#    endif
#    if PP_LOCAL_C(96)
        PP_LOCAL_MACRO(96)
#    endif
#    if PP_LOCAL_C(97)
        PP_LOCAL_MACRO(97)
#    endif
#    if PP_LOCAL_C(98)
        PP_LOCAL_MACRO(98)
#    endif
#    if PP_LOCAL_C(99)
        PP_LOCAL_MACRO(99)
#    endif
#    if PP_LOCAL_C(100)
        PP_LOCAL_MACRO(100)
#    endif
#    if PP_LOCAL_C(101)
        PP_LOCAL_MACRO(101)
#    endif
#    if PP_LOCAL_C(102)
        PP_LOCAL_MACRO(102)
#    endif
#    if PP_LOCAL_C(103)
        PP_LOCAL_MACRO(103)
#    endif
#    if PP_LOCAL_C(104)
        PP_LOCAL_MACRO(104)
#    endif
#    if PP_LOCAL_C(105)
        PP_LOCAL_MACRO(105)
#    endif
#    if PP_LOCAL_C(106)
        PP_LOCAL_MACRO(106)
#    endif
#    if PP_LOCAL_C(107)
        PP_LOCAL_MACRO(107)
#    endif
#    if PP_LOCAL_C(108)
        PP_LOCAL_MACRO(108)
#    endif
#    if PP_LOCAL_C(109)
        PP_LOCAL_MACRO(109)
#    endif
#    if PP_LOCAL_C(110)
        PP_LOCAL_MACRO(110)
#    endif
#    if PP_LOCAL_C(111)
        PP_LOCAL_MACRO(111)
#    endif
#    if PP_LOCAL_C(112)
        PP_LOCAL_MACRO(112)
#    endif
#    if PP_LOCAL_C(113)
        PP_LOCAL_MACRO(113)
#    endif
#    if PP_LOCAL_C(114)
        PP_LOCAL_MACRO(114)
#    endif
#    if PP_LOCAL_C(115)
        PP_LOCAL_MACRO(115)
#    endif
#    if PP_LOCAL_C(116)
        PP_LOCAL_MACRO(116)
#    endif
#    if PP_LOCAL_C(117)
        PP_LOCAL_MACRO(117)
#    endif
#    if PP_LOCAL_C(118)
        PP_LOCAL_MACRO(118)
#    endif
#    if PP_LOCAL_C(119)
        PP_LOCAL_MACRO(119)
#    endif
#    if PP_LOCAL_C(120)
        PP_LOCAL_MACRO(120)
#    endif
#    if PP_LOCAL_C(121)
        PP_LOCAL_MACRO(121)
#    endif
#    if PP_LOCAL_C(122)
        PP_LOCAL_MACRO(122)
#    endif
#    if PP_LOCAL_C(123)
        PP_LOCAL_MACRO(123)
#    endif
#    if PP_LOCAL_C(124)
        PP_LOCAL_MACRO(124)
#    endif
#    if PP_LOCAL_C(125)
        PP_LOCAL_MACRO(125)
#    endif
#    if PP_LOCAL_C(126)
        PP_LOCAL_MACRO(126)
#    endif
#    if PP_LOCAL_C(127)
        PP_LOCAL_MACRO(127)
#    endif
#    if PP_LOCAL_C(128)
        PP_LOCAL_MACRO(128)
#    endif
#    if PP_LOCAL_C(129)
        PP_LOCAL_MACRO(129)
#    endif
#    if PP_LOCAL_C(130)
        PP_LOCAL_MACRO(130)
#    endif
#    if PP_LOCAL_C(131)
        PP_LOCAL_MACRO(131)
#    endif
#    if PP_LOCAL_C(132)
        PP_LOCAL_MACRO(132)
#    endif
#    if PP_LOCAL_C(133)
        PP_LOCAL_MACRO(133)
#    endif
#    if PP_LOCAL_C(134)
        PP_LOCAL_MACRO(134)
#    endif
#    if PP_LOCAL_C(135)
        PP_LOCAL_MACRO(135)
#    endif
#    if PP_LOCAL_C(136)
        PP_LOCAL_MACRO(136)
#    endif
#    if PP_LOCAL_C(137)
        PP_LOCAL_MACRO(137)
#    endif
#    if PP_LOCAL_C(138)
        PP_LOCAL_MACRO(138)
#    endif
#    if PP_LOCAL_C(139)
        PP_LOCAL_MACRO(139)
#    endif
#    if PP_LOCAL_C(140)
        PP_LOCAL_MACRO(140)
#    endif
#    if PP_LOCAL_C(141)
        PP_LOCAL_MACRO(141)
#    endif
#    if PP_LOCAL_C(142)
        PP_LOCAL_MACRO(142)
#    endif
#    if PP_LOCAL_C(143)
        PP_LOCAL_MACRO(143)
#    endif
#    if PP_LOCAL_C(144)
        PP_LOCAL_MACRO(144)
#    endif
#    if PP_LOCAL_C(145)
        PP_LOCAL_MACRO(145)
#    endif
#    if PP_LOCAL_C(146)
        PP_LOCAL_MACRO(146)
#    endif
#    if PP_LOCAL_C(147)
        PP_LOCAL_MACRO(147)
#    endif
#    if PP_LOCAL_C(148)
        PP_LOCAL_MACRO(148)
#    endif
#    if PP_LOCAL_C(149)
        PP_LOCAL_MACRO(149)
#    endif
#    if PP_LOCAL_C(150)
        PP_LOCAL_MACRO(150)
#    endif
#    if PP_LOCAL_C(151)
        PP_LOCAL_MACRO(151)
#    endif
#    if PP_LOCAL_C(152)
        PP_LOCAL_MACRO(152)
#    endif
#    if PP_LOCAL_C(153)
        PP_LOCAL_MACRO(153)
#    endif
#    if PP_LOCAL_C(154)
        PP_LOCAL_MACRO(154)
#    endif
#    if PP_LOCAL_C(155)
        PP_LOCAL_MACRO(155)
#    endif
#    if PP_LOCAL_C(156)
        PP_LOCAL_MACRO(156)
#    endif
#    if PP_LOCAL_C(157)
        PP_LOCAL_MACRO(157)
#    endif
#    if PP_LOCAL_C(158)
        PP_LOCAL_MACRO(158)
#    endif
#    if PP_LOCAL_C(159)
        PP_LOCAL_MACRO(159)
#    endif
#    if PP_LOCAL_C(160)
        PP_LOCAL_MACRO(160)
#    endif
#    if PP_LOCAL_C(161)
        PP_LOCAL_MACRO(161)
#    endif
#    if PP_LOCAL_C(162)
        PP_LOCAL_MACRO(162)
#    endif
#    if PP_LOCAL_C(163)
        PP_LOCAL_MACRO(163)
#    endif
#    if PP_LOCAL_C(164)
        PP_LOCAL_MACRO(164)
#    endif
#    if PP_LOCAL_C(165)
        PP_LOCAL_MACRO(165)
#    endif
#    if PP_LOCAL_C(166)
        PP_LOCAL_MACRO(166)
#    endif
#    if PP_LOCAL_C(167)
        PP_LOCAL_MACRO(167)
#    endif
#    if PP_LOCAL_C(168)
        PP_LOCAL_MACRO(168)
#    endif
#    if PP_LOCAL_C(169)
        PP_LOCAL_MACRO(169)
#    endif
#    if PP_LOCAL_C(170)
        PP_LOCAL_MACRO(170)
#    endif
#    if PP_LOCAL_C(171)
        PP_LOCAL_MACRO(171)
#    endif
#    if PP_LOCAL_C(172)
        PP_LOCAL_MACRO(172)
#    endif
#    if PP_LOCAL_C(173)
        PP_LOCAL_MACRO(173)
#    endif
#    if PP_LOCAL_C(174)
        PP_LOCAL_MACRO(174)
#    endif
#    if PP_LOCAL_C(175)
        PP_LOCAL_MACRO(175)
#    endif
#    if PP_LOCAL_C(176)
        PP_LOCAL_MACRO(176)
#    endif
#    if PP_LOCAL_C(177)
        PP_LOCAL_MACRO(177)
#    endif
#    if PP_LOCAL_C(178)
        PP_LOCAL_MACRO(178)
#    endif
#    if PP_LOCAL_C(179)
        PP_LOCAL_MACRO(179)
#    endif
#    if PP_LOCAL_C(180)
        PP_LOCAL_MACRO(180)
#    endif
#    if PP_LOCAL_C(181)
        PP_LOCAL_MACRO(181)
#    endif
#    if PP_LOCAL_C(182)
        PP_LOCAL_MACRO(182)
#    endif
#    if PP_LOCAL_C(183)
        PP_LOCAL_MACRO(183)
#    endif
#    if PP_LOCAL_C(184)
        PP_LOCAL_MACRO(184)
#    endif
#    if PP_LOCAL_C(185)
        PP_LOCAL_MACRO(185)
#    endif
#    if PP_LOCAL_C(186)
        PP_LOCAL_MACRO(186)
#    endif
#    if PP_LOCAL_C(187)
        PP_LOCAL_MACRO(187)
#    endif
#    if PP_LOCAL_C(188)
        PP_LOCAL_MACRO(188)
#    endif
#    if PP_LOCAL_C(189)
        PP_LOCAL_MACRO(189)
#    endif
#    if PP_LOCAL_C(190)
        PP_LOCAL_MACRO(190)
#    endif
#    if PP_LOCAL_C(191)
        PP_LOCAL_MACRO(191)
#    endif
#    if PP_LOCAL_C(192)
        PP_LOCAL_MACRO(192)
#    endif
#    if PP_LOCAL_C(193)
        PP_LOCAL_MACRO(193)
#    endif
#    if PP_LOCAL_C(194)
        PP_LOCAL_MACRO(194)
#    endif
#    if PP_LOCAL_C(195)
        PP_LOCAL_MACRO(195)
#    endif
#    if PP_LOCAL_C(196)
        PP_LOCAL_MACRO(196)
#    endif
#    if PP_LOCAL_C(197)
        PP_LOCAL_MACRO(197)
#    endif
#    if PP_LOCAL_C(198)
        PP_LOCAL_MACRO(198)
#    endif
#    if PP_LOCAL_C(199)
        PP_LOCAL_MACRO(199)
#    endif
#    if PP_LOCAL_C(200)
        PP_LOCAL_MACRO(200)
#    endif
#    if PP_LOCAL_C(201)
        PP_LOCAL_MACRO(201)
#    endif
#    if PP_LOCAL_C(202)
        PP_LOCAL_MACRO(202)
#    endif
#    if PP_LOCAL_C(203)
        PP_LOCAL_MACRO(203)
#    endif
#    if PP_LOCAL_C(204)
        PP_LOCAL_MACRO(204)
#    endif
#    if PP_LOCAL_C(205)
        PP_LOCAL_MACRO(205)
#    endif
#    if PP_LOCAL_C(206)
        PP_LOCAL_MACRO(206)
#    endif
#    if PP_LOCAL_C(207)
        PP_LOCAL_MACRO(207)
#    endif
#    if PP_LOCAL_C(208)
        PP_LOCAL_MACRO(208)
#    endif
#    if PP_LOCAL_C(209)
        PP_LOCAL_MACRO(209)
#    endif
#    if PP_LOCAL_C(210)
        PP_LOCAL_MACRO(210)
#    endif
#    if PP_LOCAL_C(211)
        PP_LOCAL_MACRO(211)
#    endif
#    if PP_LOCAL_C(212)
        PP_LOCAL_MACRO(212)
#    endif
#    if PP_LOCAL_C(213)
        PP_LOCAL_MACRO(213)
#    endif
#    if PP_LOCAL_C(214)
        PP_LOCAL_MACRO(214)
#    endif
#    if PP_LOCAL_C(215)
        PP_LOCAL_MACRO(215)
#    endif
#    if PP_LOCAL_C(216)
        PP_LOCAL_MACRO(216)
#    endif
#    if PP_LOCAL_C(217)
        PP_LOCAL_MACRO(217)
#    endif
#    if PP_LOCAL_C(218)
        PP_LOCAL_MACRO(218)
#    endif
#    if PP_LOCAL_C(219)
        PP_LOCAL_MACRO(219)
#    endif
#    if PP_LOCAL_C(220)
        PP_LOCAL_MACRO(220)
#    endif
#    if PP_LOCAL_C(221)
        PP_LOCAL_MACRO(221)
#    endif
#    if PP_LOCAL_C(222)
        PP_LOCAL_MACRO(222)
#    endif
#    if PP_LOCAL_C(223)
        PP_LOCAL_MACRO(223)
#    endif
#    if PP_LOCAL_C(224)
        PP_LOCAL_MACRO(224)
#    endif
#    if PP_LOCAL_C(225)
        PP_LOCAL_MACRO(225)
#    endif
#    if PP_LOCAL_C(226)
        PP_LOCAL_MACRO(226)
#    endif
#    if PP_LOCAL_C(227)
        PP_LOCAL_MACRO(227)
#    endif
#    if PP_LOCAL_C(228)
        PP_LOCAL_MACRO(228)
#    endif
#    if PP_LOCAL_C(229)
        PP_LOCAL_MACRO(229)
#    endif
#    if PP_LOCAL_C(230)
        PP_LOCAL_MACRO(230)
#    endif
#    if PP_LOCAL_C(231)
        PP_LOCAL_MACRO(231)
#    endif
#    if PP_LOCAL_C(232)
        PP_LOCAL_MACRO(232)
#    endif
#    if PP_LOCAL_C(233)
        PP_LOCAL_MACRO(233)
#    endif
#    if PP_LOCAL_C(234)
        PP_LOCAL_MACRO(234)
#    endif
#    if PP_LOCAL_C(235)
        PP_LOCAL_MACRO(235)
#    endif
#    if PP_LOCAL_C(236)
        PP_LOCAL_MACRO(236)
#    endif

#    if PP_LOCAL_C(237)
        PP_LOCAL_MACRO(237)
#    endif
#    if PP_LOCAL_C(238)
        PP_LOCAL_MACRO(238)
#    endif
#    if PP_LOCAL_C(239)
        PP_LOCAL_MACRO(239)
#    endif
#    if PP_LOCAL_C(240)
        PP_LOCAL_MACRO(240)
#    endif
#    if PP_LOCAL_C(241)
        PP_LOCAL_MACRO(241)
#    endif
#    if PP_LOCAL_C(242)
        PP_LOCAL_MACRO(242)
#    endif
#    if PP_LOCAL_C(243)
        PP_LOCAL_MACRO(243)
#    endif
#    if PP_LOCAL_C(244)
        PP_LOCAL_MACRO(244)
#    endif
#    if PP_LOCAL_C(245)
        PP_LOCAL_MACRO(245)
#    endif
#    if PP_LOCAL_C(246)
        PP_LOCAL_MACRO(246)
#    endif
#    if PP_LOCAL_C(247)
        PP_LOCAL_MACRO(247)
#    endif
#    if PP_LOCAL_C(248)
        PP_LOCAL_MACRO(248)
#    endif
#    if PP_LOCAL_C(249)
        PP_LOCAL_MACRO(249)
#    endif
#    if PP_LOCAL_C(250)
        PP_LOCAL_MACRO(250)
#    endif
#    if PP_LOCAL_C(251)
        PP_LOCAL_MACRO(251)
#    endif
#    if PP_LOCAL_C(252)
        PP_LOCAL_MACRO(252)
#    endif
#    if PP_LOCAL_C(253)
        PP_LOCAL_MACRO(253)
#    endif
#    if PP_LOCAL_C(254)
        PP_LOCAL_MACRO(254)
#    endif
#    if PP_LOCAL_C(255)
        PP_LOCAL_MACRO(255)
#    endif
#    if PP_LOCAL_C(256)
        PP_LOCAL_MACRO(256)
#    endif
# endif
#
# undef PP_LOCAL_LIMITS
#
# undef PP_LOCAL_S
# undef PP_LOCAL_F
#
# undef PP_LOCAL_MACRO
