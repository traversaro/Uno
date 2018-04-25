#  NLP written by GAMS Convert at 06/20/02 12:23:52
#  
#  Equation counts
#     Total       E       G       L       N       X
#        25      19       6       0       0       0
#  
#  Variable counts
#                 x       b       i     s1s     s2s      sc      si
#     Total    cont  binary integer    sos1    sos2   scont    sint
#        32      32       0       0       0       0       0       0
#  FX     1       1       0       0       0       0       0       0
#  
#  Nonzero counts
#     Total   const      NL     DLL
#        67      55      12       0
# 
#  Reformualtion has removed 1 variable and 1 equation


var x1 := 150, >= 150, <= 1500;
var x2 := 150, >= 150, <= 1500;
var x3 := 150, >= 150, <= 1500;
var x4 := 150, >= 150, <= 1500;
var x5 := 150, >= 150, <= 1500;
var x6 := 150, >= 150, <= 1500;
var x7 >= 0, <= 1000;
var x8 >= 0, <= 1000;
var x9 >= 0, <= 1000;
var x10 >= 0, <= 1000;
var x11 >= 0, <= 1000;
var x12 >= 0, <= 1000;
var x13 >= 0;
var x14 >= 0;
var x15 >= 0;
var x16 >= 0;
var x17 >= 0;
var x18 >= 0;
var x19 >= 0;
var x20 >= 0;
var x21 >= 0;
var x22 >= 0;
var x23 >= 0;
var x24 >= 0;
var x25 := 100000, >= 100000, <= 100000;
var x26 := 60000, >= 60000, <= 120000;
var x27 := 60000, >= 60000, <= 120000;
var x28 := 60000, >= 60000, <= 120000;
var x29 := 60000, >= 60000, <= 120000;
var x30 := 60000, >= 60000, <= 120000;
var x31 := 60000, >= 60000, <= 120000;

minimize obj: 82.8*(0.0016*x1^2 + 8*x1 + 0.0016*x2^2 + 8*x2 + 0.0016*x3^2 + 8*
              x3 + 0.0016*x4^2 + 8*x4 + 0.0016*x5^2 + 8*x5 + 0.0016*x6^2 + 8*x6
              ) + 248400;

subject to

e2:    x1 + x7 - x13 >= 1200;

e3:    x2 + x8 - x14 >= 1500;

e4:    x3 + x9 - x15 >= 1100;

e5:    x4 + x10 - x16 >= 1800;

e6:    x5 + x11 - x17 >= 950;

e7:    x6 + x12 - x18 >= 1300;

e8:    12*x19 - x25 + x26 = 24000;

e9:    12*x20 - x26 + x27 = 24000;

e10:    12*x21 - x27 + x28 = 24000;

e11:    12*x22 - x28 + x29 = 24000;

e12:    12*x23 - x29 + x30 = 24000;

e13:    12*x24 - x30 + x31 = 24000;

e14:  - 8e-5*x7^2 + x13 = 0;

e15:  - 8e-5*x8^2 + x14 = 0;

e16:  - 8e-5*x9^2 + x15 = 0;

e17:  - 8e-5*x10^2 + x16 = 0;

e18:  - 8e-5*x11^2 + x17 = 0;

e19:  - 8e-5*x12^2 + x18 = 0;

e20:  - 4.97*x7 + x19 = 330;

e21:  - 4.97*x8 + x20 = 330;

e22:  - 4.97*x9 + x21 = 330;

e23:  - 4.97*x10 + x22 = 330;

e24:  - 4.97*x11 + x23 = 330;

e25:  - 4.97*x12 + x24 = 330;
