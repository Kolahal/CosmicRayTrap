#import ROOT
import sys
import os
import numpy as np 
import h5py
from scipy.interpolate import RegularGridInterpolator

f = h5py.File('/home/kolahalb/Works/Computation/Projects/MMT/MMTSim/MMT/MagFieldData_array_2cm_grid_gpu.hdf5', 'r') 
setBx = f['Bx_map']
setBy = f['By_map']
setBz = f['Bz_map']

x = np.linspace(-500,500,501)
y = np.linspace(-500,500,501)
z = np.linspace(-500,500,501)

Bx = RegularGridInterpolator((x, y, z), setBx[()])
By = RegularGridInterpolator((x, y, z), setBy[()])
Bz = RegularGridInterpolator((x, y, z), setBz[()])

Bfield = []

def Field_interpolator(*list_of_args):
    arr = np.array(list_of_args)
    arr = 1.e-1*arr
    print('array: '+str(arr))
    arr[2] = arr[2]+0.0#50
    #python2
    #del Bfield[:]
    #python3
    Bfield.clear()
    if arr[0]<-500.0 or arr[0]> 500.0 or arr[1]<-500.0 or arr[1]> 500.0 or arr[2]< -500.0 or arr[2]> 500.0:
        Bfield.append(0.0)
        Bfield.append(0.0)
        Bfield.append(0.0)
        return Bfield
    Bfield.append(Bx(arr))
    Bfield.append(By(arr))
    Bfield.append(Bz(arr))
    print('---> '+str(Bfield))
    return Bfield
