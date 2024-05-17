# -*- coding: utf-8 -*-
"""
Created on Tue May  7 13:26:59 2024

@author: maart
"""
Cl = [0.2183, 0.4104, 0.5895, 0.8100, 0.9873, 1.1304, 1.2751]
Cd = [0.0018, 0.0226, 0.03545, 0.048, 0.0805, 0.099, 0.11665]

Cl_0 = 0.2183
Cd_0 = 0.0018

V = 16.67
S = 0.02
c = 0.1
rho = 1.29
L_th = 0.5 * rho * (V**2) * S * Cl_0
D_th = 0.5 * rho * (V**2) * S * Cd_0


NP = (97e-3)*0.25
D1 = 19.5e-3 - NP
D2 = 66.5e-3 - NP

m = 0.011509243642660547 #+ 7.41e-3
g = 9.81
F = L_th
F2 = -F*D1/(D2-D1)
F1 = F-F2

m1 = F1 * 1000/g
m2 = F2 * 1000/g
m_tot = m1 + m2

m_d_tot = D_th * 1000000/g


print('massa 1 =', m1)
print('massa 2 =', m2)
print('massa totaal lift =', m_tot/1000  )
print("massa drag totaal (miligram)=", m_d_tot)
#print("massa drag (miligram)=", m_d)

M  = NP*L_th
Cm = M/(0.5*rho*V**2*S*c)
print(Cm)