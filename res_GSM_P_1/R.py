import sys
import os
import matplotlib.pyplot as plt
import numpy as np


res = []

v_mejor = None
f_mejor = None

for f in os.listdir():

	if f.split('_')[0] != sys.argv[1]:
		continue
	if f.split('_')[-2] != sys.argv[2]:
		continue

	fil = open(f)
	l = fil.readlines()
	fil.close()

	dat = [list(map(int, x.split())) for x in l]
	X = [x for (x, y) in dat]
	Y = [y for (x, y) in dat]


	plt.plot(X, Y)

plt.show()

#plt.plot(X, Y)
#plt.show()
