# -*- coding: utf-8 -*-
"""
Created on Tue May  7 13:26:59 2024

@author: maart
"""
import numpy as np
Cl = [0.2183, 0.4104, 0.5895, 0.8100, 0.9873, 1.1304, 1.2751]
Cd = [0.018, 0.0226, 0.03545, 0.048, 0.0805, 0.099, 0.11665]

Cl_0 = 0.2183
Cd_0 = 0.018

V = 12.12

S = 0.02
c = 0.1
rho = 1.29
L_th = 0.5 * rho * (V**2) * S * Cl_0
D_th = 0.5 * rho * (V**2) * S * Cd_0
D = D_th
AC = (97e-3)*0.25

LE2F = (25e-3)-(3.5e-3)
LE2B = (25e-3)+(43.5e-3)
AC2F = LE2F-AC
AC2B= LE2B-AC

print("LE2F:", LE2F)
print("LE2B:", LE2B)
print("AC2F:", AC2F)
print("AC2B:", AC2B)
N = L_th*np.cos(np.radians(4)) + D_th*np.sin(np.radians(4))
A = -L_th*np.sin(np.radians(4)) + D_th*np.cos(np.radians(4))
m = 0.011509243642660547 #+ 7.41e-3
g = 9.81
L = L_th
F = L #kracht gelijkzetten aan lift

F1= (-44.5/47)*F #F1 is voor loadcell 1 aan de front
F2 = -F-F1 #F2 is voor loadcell 2 aan de back
L_pract = F1 + F2 #test om na te gaan dat de som van de 2 gelijk is aan de lift
CL = L/(0.5*rho*V**2*S)
print('F1:', F1)
print('F2:', F2)

#massa die we per loadcell moeten meten
m1 = F1 * 1000/g
m2 = F2 * 1000/g
m_tot = m1 + m2

m_d_tot = D * 1e6/g
m_D_LC = m_d_tot/2

print('massa 1 (gram)=', m1)
print('massa 2 (gram)=', m2)
print('massa totaal lift =', m_tot  )
print("massa drag per loadcell (miligram)=", m_D_LC)
#print("massa drag (miligram)=", m_d)'''

M  = (-F1)*(-AC2B) - (-F2)*(AC2F) #F1 = front loadcell, F2 back loadcell
Cm = M/(0.5*rho*V**2*S*c)



print('L', -L)
print('Cl: ',CL)
print('D', D)
print('M:',M)
print('Cm: ', Cm)