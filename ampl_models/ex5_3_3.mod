#  NLP written by GAMS Convert at 06/20/02 11:49:55
#  
#  Equation counts
#     Total       E       G       L       N       X
#        54      54       0       0       0       0
#  
#  Variable counts
#                 x       b       i     s1s     s2s      sc      si
#     Total    cont  binary integer    sos1    sos2   scont    sint
#        63      63       0       0       0       0       0       0
#  FX     0       0       0       0       0       0       0       0
#  
#  Nonzero counts
#     Total   const      NL     DLL
#       288      88     200       0
# 
#  Reformualtion has removed 1 variable and 1 equation


var x1 >= 0, <= 600;
var x2 >= 0, <= 600;
var x3 >= 0, <= 600;
var x4 >= 0, <= 600;
var x5 >= 0, <= 600;
var x6 >= 0, <= 600;
var x7 >= 0, <= 600;
var x8 >= 0, <= 600;
var x9 >= 0, <= 600;
var x10 >= 0, <= 600;
var x11 >= 0, <= 600;
var x12 >= 0, <= 600;
var x13 >= 0, <= 600;
var x14 >= 0, <= 600;
var x15 >= 0, <= 600;
var x16 >= 0, <= 600;
var x17 >= 0, <= 600;
var x18 >= 0, <= 600;
var x19 >= 0, <= 600;
var x20 >= 0, <= 600;
var x21 >= 0, <= 600;
var x22 >= 0, <= 600;
var x23 >= 0, <= 600;
var x24 >= 0, <= 600;
var x25 >= 0, <= 600;
var x26 >= 0, <= 600;
var x27 >= 0, <= 600;
var x28 >= 0, <= 600;
var x29 >= 0, <= 600;
var x30 >= 0, <= 600;
var x31 >= 0, <= 600;
var x32 >= 0, <= 600;
var x33 >= 0, <= 1;
var x34 >= 0, <= 1;
var x35 >= 0, <= 1;
var x36 >= 0, <= 1;
var x37 >= 0, <= 1;
var x38 >= 0, <= 1;
var x39 >= 0, <= 1;
var x40 >= 0, <= 1;
var x41 >= 0, <= 1;
var x42 >= 0, <= 1;
var x43 >= 0, <= 1;
var x44 >= 0, <= 1;
var x45 >= 0, <= 1;
var x46 >= 0, <= 1;
var x47 >= 0, <= 1;
var x48 >= 0, <= 1;
var x49 >= 0, <= 1;
var x50 >= 0, <= 1;
var x51 >= 0, <= 1;
var x52 >= 0, <= 1;
var x53 >= 0, <= 1;
var x54 >= 0, <= 1;
var x55 >= 0, <= 1;
var x56 >= 0, <= 1;
var x57 >= 0, <= 1;
var x58 >= 0, <= 1;
var x59 >= 0, <= 1;
var x60 >= 0, <= 1;
var x61 >= 0, <= 1;
var x62 >= 0, <= 1;

minimize obj: (0.00132223 + 0.0016446*x33 + 0.0018611*x34 + 0.001262*x35)*x6 + 
              (0.0015625*x43 + 0.0091604*x44 + 0.0076758*x45 - 
              9.40700000000017e-5)*x15 + (0.00457315 + (-0.001748*x53) - 
              0.0002583*x54 - 0.0004691*x55)*x24 + 1.68776;

subject to

e1:    x1 + x2 + x3 + x4 + x5 = 600;

e2:  - x1 + x6 - x17 - x26 = 0;

e3:  - x2 - x11 + x15 - x27 = 0;

e4:  - x3 - x12 - x21 + x24 = 0;

e5:    x7 - x8 - x9 = 0;

e6:    x10 - x11 - x12 - x13 - x14 = 0;

e7:    x16 - x17 - x18 - x19 = 0;

e8:    x20 - x21 - x22 - x23 = 0;

e9:    x25 - x26 - x27 - x28 - x29 = 0;

e10:    x30 - x31 - x32 = 0;

e11: x7*x37 - 0.85*x6*x33 = 0;

e12: x16*x48 - 0.85*x15*x44 = 0;

e13: x25*x59 - x24*x55 = 0;

e14: x10*x40 - x6*x34 = 0;

e15: x20*x51 - 0.85*x15*x45 = 0;

e16: x30*x62 - 0.85*x24*x56 = 0;

e17: x6*x33 - x7*x37 - x10*x39 = 0;

e18: x6*x34 - x7*x38 - x10*x40 = 0;

e19: x6*x35 - x10*x41 = 0;

e20: x6*x36 - x10*x42 = 0;

e21: x15*x43 - x16*x47 = 0;

e22: x15*x44 - x16*x48 - x20*x50 = 0;

e23: x15*x45 - x16*x49 - x20*x51 = 0;

e24: x15*x46 - x20*x52 = 0;

e25: x24*x53 - x25*x57 = 0;

e26: x24*x54 - x25*x58 = 0;

e27: x24*x55 - x25*x59 - x30*x61 = 0;

e28: x24*x56 - x25*x60 - x30*x62 = 0;

e29: x17*x47 + x26*x57 - x6*x33 + 0.25*x1 = 0;

e30: x17*x48 + x26*x58 - x6*x34 + 0.333*x1 = 0;

e31: x17*x49 + x26*x59 - x6*x35 + 0.167*x1 = 0;

e32: x26*x60 - x6*x36 + 0.25*x1 = 0;

e33: x11*x39 + x27*x57 - x15*x43 + 0.25*x2 = 0;

e34: x11*x40 + x27*x58 - x15*x44 + 0.333*x2 = 0;

e35: x11*x41 + x27*x59 - x15*x45 + 0.167*x2 = 0;

e36: x11*x42 + x27*x60 - x15*x46 + 0.25*x2 = 0;

e37: x12*x39 - x24*x53 + 0.25*x3 = 0;

e38: x12*x40 + x21*x50 - x24*x54 + 0.333*x3 = 0;

e39: x12*x41 + x21*x51 - x24*x55 + 0.167*x3 = 0;

e40: x12*x42 + x21*x52 - x24*x56 + 0.25*x3 = 0;

e41: x8*x37 + x13*x39 + x18*x47 + x28*x57 + 0.25*x4 = 50;

e42: x8*x38 + x13*x40 + x18*x48 + x22*x50 + x28*x58 + 0.222*x4 = 100;

e43: x13*x41 + x18*x49 + x22*x51 + x28*x59 + x31*x61 + 0.167*x4 = 40;

e44: x13*x42 + x22*x52 + x28*x60 + x31*x62 + 0.25*x4 = 100;

e45:    x33 + x34 + x35 + x36 = 1;

e46:    x37 + x38 = 1;

e47:    x39 + x40 + x41 + x42 = 1;

e48:    x43 + x44 + x45 + x46 = 1;

e49:    x47 + x48 + x49 = 1;

e50:    x50 + x51 + x52 = 1;

e51:    x53 + x54 + x55 + x56 = 1;

e52:    x57 + x58 + x59 + x60 = 1;

e53:    x61 + x62 = 1;
