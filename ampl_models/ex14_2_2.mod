#  NLP written by GAMS Convert at 06/20/02 11:36:50
#  
#  Equation counts
#     Total       E       G       L       N       X
#         6       2       0       4       0       0
#  
#  Variable counts
#                 x       b       i     s1s     s2s      sc      si
#     Total    cont  binary integer    sos1    sos2   scont    sint
#         5       5       0       0       0       0       0       0
#  FX     0       0       0       0       0       0       0       0
#  
#  Nonzero counts
#     Total   const      NL     DLL
#        20       8      12       0
# 
#  Reformualtion has removed 1 variable and 1 equation


var x1 := 0.624, >= 1E-6, <= 1;
var x2 := 0.376, >= 1E-6, <= 1;
var x3 := 58.129, >= 20, <= 80;
var x5 >= 0;

minimize obj:    x5;

subject to

e2: log(x1 + 0.191987347447993*x2) + x1/(x1 + 0.191987347447993*x2) + 
    0.315693799947296*x2/(0.315693799947296*x1 + x2) + 3643.31361767678/(
    239.726 + x3) - x5 <= 12.9738026256517;

e3: log(0.315693799947296*x1 + x2) + 0.191987347447993*x1/(x1 + 
    0.191987347447993*x2) + x2/(0.315693799947296*x1 + x2) + 2755.64173589155/(
    219.161 + x3) - x5 <= 10.2081676704566;

e4: (-log(x1 + 0.191987347447993*x2)) - (x1/(x1 + 0.191987347447993*x2) + 
    0.315693799947296*x2/(0.315693799947296*x1 + x2)) - 3643.31361767678/(
    239.726 + x3) - x5 <= -12.9738026256517;

e5: (-log(0.315693799947296*x1 + x2)) - (0.191987347447993*x1/(x1 + 
    0.191987347447993*x2) + x2/(0.315693799947296*x1 + x2)) - 2755.64173589155/
    (219.161 + x3) - x5 <= -10.2081676704566;

e6:    x1 + x2 = 1;
