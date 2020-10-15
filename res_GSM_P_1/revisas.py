import sys
import os
import matplotlib.pyplot as plt
import numpy as np


os.chdir(sys.argv[3])
res = []

v_mejor = None
f_mejor = None

for f in os.listdir():

	if f.split('_')[0] != sys.argv[1]:
		continue
	if f.split('_')[-2] != sys.argv[2]:
		continue

	fil = open(f)
	l = fil.readlines()[-1]
	fil.close()
	_, a = l.split()
	a = int(a)

	res.append(a)

	if v_mejor is None:
		v_mejor = res[-1]
		f_mejor = f
	elif v_mejor > res[-1]:
		v_mejor = res[-1]
		f_mejor = f


print()
print("maximo:", max(res))
print("minimo:", min(res))
print("promed:", sum(res)/len(res))
print()

print(f_mejor)

fil = open(f_mejor)
l = fil.readlines()

dat = [list(map(int, x.split())) for x in l]
X = [x for (x, y) in dat]
Y = [y for (x, y) in dat]

np.savetxt('{0}'.format(sys.argv[2]), dat)

#plt.plot(X, Y)
#plt.show()
