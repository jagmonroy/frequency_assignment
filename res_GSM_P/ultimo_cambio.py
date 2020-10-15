import os
import sys
import matplotlib.pyplot as plt
import numpy as np

lista = []

for f in os.listdir():

	if f.split('_')[0] != 'costo':
		continue

	if f.split('_')[-2] != sys.argv[1]:
		continue

	fil = open(f)
	l = fil.readlines()
	fil.close()

	dat = [list(map(float, x.split())) for x in l]

	X = [x for (x, y) in dat]
	Y = [y for (x, y) in dat]

	X = np.array(X)
	X /= 1000*60

	i = 0

	for i in range(len(X)):
		if X[i] > 200:
			break

	lista.append(-Y[-1] + Y[i])

	# plt.plot(X, Y)
	# plt.show()

print(lista)
print(sum(lista)/len(lista))
