import sys
import numpy as np
import os
import matplotlib.pyplot as plt

os.chdir(sys.argv[1])

dat0 = np.loadtxt('0')
dat1 = np.loadtxt('1')

X0 = [x for (x, y) in dat0]
Y0 = [y for (x, y) in dat0]
X1 = [x for (x, y) in dat1]
Y1 = [y for (x, y) in dat1]

print(len(X0))
print(len(X1))

X0 = np.array(X0)
X1 = np.array(X1)

X0 /= 1000*60
X1 /= 1000*60

plt.title('Función de costo')
plt.xlabel('Tiempo en minutos')
plt.ylabel('Costo de mejor individuo')

plt.plot(X0, Y0, label = 'Torneo')
plt.plot(X1, Y1, label = 'Diversidad')
plt.legend()
plt.show()

