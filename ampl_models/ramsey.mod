#  NLP written by GAMS Convert at 06/20/02 12:59:12
#  
#  Equation counts
#     Total       E       G       L       N       X
#        23      22       0       1       0       0
#  
#  Variable counts
#                 x       b       i     s1s     s2s      sc      si
#     Total    cont  binary integer    sos1    sos2   scont    sint
#        34      34       0       0       0       0       0       0
#  FX     2       2       0       0       0       0       0       0
#  
#  Nonzero counts
#     Total   const      NL     DLL
#        77      55      22       0
# 
#  Reformualtion has removed 1 variable and 1 equation


var x1 := 3, >= 3, <= 3;
var x2 := 3, >= 3;
var x3 := 3, >= 3;
var x4 := 3, >= 3;
var x5 := 3, >= 3;
var x6 := 3, >= 3;
var x7 := 3, >= 3;
var x8 := 3, >= 3;
var x9 := 3, >= 3;
var x10 := 3, >= 3;
var x11 := 3, >= 3;
var x12 := 0.95, >= 0.95;
var x13 := 0.95, >= 0.95;
var x14 := 0.95, >= 0.95;
var x15 := 0.95, >= 0.95;
var x16 := 0.95, >= 0.95;
var x17 := 0.95, >= 0.95;
var x18 := 0.95, >= 0.95;
var x19 := 0.95, >= 0.95;
var x20 := 0.95, >= 0.95;
var x21 := 0.95, >= 0.95;
var x22 := 0.95, >= 0.95;
var x23 := 0.05, >= 0.05, <= 0.05;
var x24 := 0.05, >= 0.05, <= 0.0575;
var x25 := 0.05, >= 0.05, <= 0.066125;
var x26 := 0.05, >= 0.05, <= 0.07604375;
var x27 := 0.05, >= 0.05, <= 0.0874503125;
var x28 := 0.05, >= 0.05, <= 0.100567859375;
var x29 := 0.05, >= 0.05, <= 0.11565303828125;
var x30 := 0.05, >= 0.05, <= 0.133000994023437;
var x31 := 0.05, >= 0.05, <= 0.152951143126953;
var x32 := 0.05, >= 0.05, <= 0.175893814595996;
var x33 := 0.05, >= 0.05, <= 0.202277886785395;

minimize obj:  - (0.95*log(x12) + 0.9025*log(x13) + 0.857375*log(x14) + 
              0.81450625*log(x15) + 0.7737809375*log(x16) + 0.735091890625*log(
              x17) + 0.69833729609375*log(x18) + 0.663420431289062*log(x19) + 
              0.630249409724609*log(x20) + 0.598736939238379*log(x21) + 
              11.3760018455292*log(x22));

subject to

e1: 0.759835685651593*x1**0.25 - x12 - x23 = 0;

e2: 0.77686866556676*x2**0.25 - x13 - x24 = 0;

e3: 0.794283468039448*x3**0.25 - x14 - x25 = 0;

e4: 0.812088652256959*x4**0.25 - x15 - x26 = 0;

e5: 0.830292969275008*x5**0.25 - x16 - x27 = 0;

e6: 0.848905366318769*x6**0.25 - x17 - x28 = 0;

e7: 0.867934991180342*x7**0.25 - x18 - x29 = 0;

e8: 0.88739119671479*x8**0.25 - x19 - x30 = 0;

e9: 0.907283545436972*x9**0.25 - x20 - x31 = 0;

e10: 0.92762181422141*x10**0.25 - x21 - x32 = 0;

e11: 0.948415999107521*x11**0.25 - x22 - x33 = 0;

e12:  - x1 + x2 - x23 = 0;

e13:  - x2 + x3 - x24 = 0;

e14:  - x3 + x4 - x25 = 0;

e15:  - x4 + x5 - x26 = 0;

e16:  - x5 + x6 - x27 = 0;

e17:  - x6 + x7 - x28 = 0;

e18:  - x7 + x8 - x29 = 0;

e19:  - x8 + x9 - x30 = 0;

e20:  - x9 + x10 - x31 = 0;

e21:  - x10 + x11 - x32 = 0;

e22:    0.03*x11 - x33 <= 0;
