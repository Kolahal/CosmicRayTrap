import ROOT
import numpy as np
import h5py
from numpy import genfromtxt
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.gridspec as gridspec

print('try to open the file')
#f=h5py.File('MagFieldData_array_3D.hdf5','r')
f=h5py.File('MagFieldData_array_2cm_grid_gpu.hdf5','r')

array_bx = f['Bx_map']
array_by = f['By_map']
array_bz = f['Bz_map']

Bz = []

plot=plt.figure()
ax=plot.gca()

x = np.linspace(-500.0,500.0, num=501)
y = np.linspace(-500.0,500.0, num=501)
z = np.linspace(-500.0,500.0, num=501)

Z = z.tolist()

print('comes up to this')
print(str(Z))

for zk in range(-500,501,2):
	Bz.append(array_bx[0,0,zk])

plt.plot(Z, Bz, 'ro')
plt.show()
plot.savefig('abc.png')
