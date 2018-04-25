#  NLP written by GAMS Convert at 06/20/02 11:37:43
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


var x1 := 0.937, >= 1E-6, <= 1;
var x2 := 1E-6, >= 1E-6, <= 1;
var x3 := 80.166, >= 60, <= 100;
var x5 >= 0;

minimize obj:    x5;

subject to

e2: 0.361872516756437*x2/(x1 + 0.888649896608059*x2) + 0.868134622480909*x2/(
    0.696880695582072*x1 + x2) - (0.361872516756437*x1*x2/(x1 + 
    0.888649896608059*x2)^2 + 0.604986259573375*x2*x1/(0.696880695582072*x1 + 
    x2)^2) - 2755.64173589155/(219.161 + x3) - x5 <= -9.20816767045657;

e3: 0.868134622480909*x1/(0.696880695582072*x1 + x2) + 0.361872516756437*x1/(x1
     + 0.888649896608059*x2) - (0.321577974600906*x1*x2/(x1 + 0.888649896608059
    *x2)^2 + 0.868134622480909*x2*x1/(0.696880695582072*x1 + x2)^2) - 
    4117.06819797521/(227.438 + x3) - x5 <= -12.6599269316621;

e4: (-0.361872516756437*x2/(x1 + 0.888649896608059*x2)) - 0.868134622480909*x2/
    (0.696880695582072*x1 + x2) + 0.361872516756437*x1*x2/(x1 + 
    0.888649896608059*x2)^2 + 0.604986259573375*x2*x1/(0.696880695582072*x1 + 
    x2)^2 + 2755.64173589155/(219.161 + x3) - x5 <= 9.20816767045657;

e5: (-0.868134622480909*x1/(0.696880695582072*x1 + x2)) - 0.361872516756437*x1/
    (x1 + 0.888649896608059*x2) + 0.321577974600906*x1*x2/(x1 + 
    0.888649896608059*x2)^2 + 0.868134622480909*x2*x1/(0.696880695582072*x1 + 
    x2)^2 + 4117.06819797521/(227.438 + x3) - x5 <= 12.6599269316621;

e6:    x1 + x2 = 1;
