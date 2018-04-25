#  NLP written by GAMS Convert at 06/20/02 12:08:58
#  
#  Equation counts
#     Total       E       G       L       N       X
#        11      11       0       0       0       0
#  
#  Variable counts
#                 x       b       i     s1s     s2s      sc      si
#     Total    cont  binary integer    sos1    sos2   scont    sint
#        25      25       0       0       0       0       0       0
#  FX     0       0       0       0       0       0       0       0
#  
#  Nonzero counts
#     Total   const      NL     DLL
#        81      21      60       0
# 
#  Reformualtion has removed 1 variable and 1 equation


var x1 := -0.328252868, >= -0.5, <= 0.5;
var x2 := 6.243266708, >= 5.4, <= 6.4;
var x3 := 0.950375356, >= 0.4, <= 1.4;
var x4 := 5.201137904, >= 4.9, <= 5.9;
var x5 := 1.592212117, >= 1.3, <= 2.3;
var x6 := 4.124052867, >= 3.9, <= 4.9;
var x7 := 2.449830504, >= 2.1, <= 3.1;
var x8 := 4.956270347, >= 4.1, <= 5.1;
var x9 := 2.867113723, >= 2.8, <= 3.8;
var x10 := 3.500210669, >= 3, <= 4;
var x11 := 4.898117627, >= 3.9, <= 4.9;
var x12 := 3.778733378, >= 3.2, <= 4.2;
var x13 := 5.691133039, >= 4.7, <= 5.7;
var x14 := 3.062250467, >= 2.3, <= 3.3;
var x15 := 5.730692483, >= 5.6, <= 6.6;
var x16 := 2.939718759, >= 2.3, <= 3.3;
var x17 := 6.159517864, >= 6, <= 7;
var x18 := 2.150080533, >= 1.9, <= 2.9;
var x19 := 7.568928609, >= 6.9, <= 7.9;
var x20 := 1.435356381, >= 1, <= 2;
var x21 := 3.59700266, >= 0, <= 10;
var x22 := -0.594234528, >= -2, <= 2;
var x23 := -1.47403364, >= -2, <= 2;
var x24 := -1.399592848, >= -2, <= 2;

minimize obj: x1^2 + (x2 - 5.9)^2 + (x3 - 0.9)^2 + (x4 - 5.4)^2 + (x5 - 1.8)^2
               + (x6 - 4.4)^2 + (x7 - 2.6)^2 + (x8 - 4.6)^2 + (x9 - 3.3)^2 + (
              x10 - 3.5)^2 + (x11 - 4.4)^2 + (x12 - 3.7)^2 + (x13 - 5.2)^2 + (
              x14 - 2.8)^2 + (x15 - 6.1)^2 + (x16 - 2.8)^2 + (x17 - 6.5)^2 + (
              x18 - 2.4)^2 + (x19 - 7.4)^2 + (x20 - 1.5)^2;

subject to

e2: x22*x1 + x23*x1^2 + x24*x1^3 - x2 + x21 = 0;

e3: x22*x3 + x23*x3^2 + x24*x3^3 - x4 + x21 = 0;

e4: x22*x5 + x23*x5^2 + x24*x5^3 - x6 + x21 = 0;

e5: x22*x7 + x23*x7^2 + x24*x7^3 - x8 + x21 = 0;

e6: x22*x9 + x23*x9^2 + x24*x9^3 - x10 + x21 = 0;

e7: x22*x11 + x23*x11^2 + x24*x11^3 - x12 + x21 = 0;

e8: x22*x13 + x23*x13^2 + x24*x13^3 - x14 + x21 = 0;

e9: x22*x15 + x23*x15^2 + x24*x15^3 - x16 + x21 = 0;

e10: x22*x17 + x23*x17^2 + x24*x17^3 - x18 + x21 = 0;

e11: x22*x19 + x23*x19^2 + x24*x19^3 - x20 + x21 = 0;
