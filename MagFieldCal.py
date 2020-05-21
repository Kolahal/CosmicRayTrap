import sys
import scipy as sp
import scipy.special
import numpy as np
import h5py
import itertools
from itertools import product
import time
from math import pi
import multiprocessing
from multiprocessing import Pool #  Process pool

MMT_dim = int(sys.argv[1])     # m
mu_0    = 4*pi*1.e-7            # Tm/A
a       = []                    # m
I       = []                    # A
C       = []                    # (Tm/A * A)

#---------------------------------------------------------
for zi in range(-50*MMT_dim,50*MMT_dim+1):
        a.append(0.0)
        I.append(0.0)
        C.append(0.0)
#---------------------------------------------------------

for zi in range(-50*MMT_dim,-40*MMT_dim,10):
        fi = abs((abs(zi)-500)/10)
        a[zi] = float("%0.2f" % (0.05 + 0.1*fi))#0.05 + 0.1*fi
        I[zi] = 8.e5 -4.e4*fi
        C[zi] = mu_0*(I[zi]/pi)
        print(str(zi)+'    '+str(I[zi])+'    '+str(a[zi]))

for zi in range(-40*MMT_dim,40*MMT_dim+1,20):
        a[zi] = 7.50
        I[zi] = 4.e5
        C[zi] = mu_0*(I[zi]/pi)
        print(str(zi)+'    '+str(I[zi])+'    '+str(a[zi]))

for zi in range(41*MMT_dim,50*MMT_dim+1,10):
        fi = 10-(500-abs(zi))/10
        a[zi] = float("%0.2f" % (1.05 - 0.1*fi))#1.05 - 0.1*fi
        I[zi] = 4.e5 +4.e4*fi
        C[zi] = mu_0*(I[zi]/pi)
        print(str(zi)+'    '+str(I[zi])+'    '+str(a[zi]))

def job(params):
        xi= paramlist[0]
        yj= paramlist[1]
        zk= paramlist[2]
        
        x = float("%0.2f" % (1.e-2*xi)) #1.e-2*xi
        y = float("%0.2f" % (1.e-2*yj)) #1.e-2*yj
        
        _bx = 0.0
        _by = 0.0
        _bz = 0.0
        
        for z_n in ZZ:          #iterate over the current loops
                if a[z_n]==0.0:
                        continue
                z = float("%0.2f" % (1.e-2*(zk - z_n))) #1.e-2*(zk - z_n)
                
                if x==0.0 and y==0.0:
                        x = 1.e-6
                        y = 1.e-6
                #if x==0.0:
                #       x = 1.e-3
                #if y==0.0:
                #       y = 1.e-3
                #if z==0.0:
                #       z = 1.e-3
                
                r       = np.sqrt(x*x + y*y + z*z)
                rho     = np.sqrt(x*x + y*y)
                alpha   = np.sqrt(a[z_n]*a[z_n] + r*r - 2*a[z_n]*rho)
                beta    = np.sqrt(a[z_n]*a[z_n] + r*r + 2*a[z_n]*rho)
                k       = np.sqrt(1.0-((alpha*alpha)/(beta*beta)))
                
                if k>0.95:
                        k=0.95
                
                if alpha<0.1:
                        continue
                
                _bx += ((C[z_n]*x*z)/(2*alpha*alpha*beta*rho*rho))*((a[z_n]*a[z_n]+r*r)*scipy.special.ellipe(k*k) - alpha*alpha*scipy.special.ellipk(k*k))
                _by += ((C[z_n]*y*z)/(2*alpha*alpha*beta*rho*rho))*((a[z_n]*a[z_n]+r*r)*scipy.special.ellipe(k*k) - alpha*alpha*scipy.special.ellipk(k*k))
                _bz += (C[z_n]/(2*alpha*alpha*beta))*((a[z_n]*a[z_n]-r*r)*scipy.special.ellipe(k*k) + alpha*alpha*scipy.special.ellipk(k*k))
        
        return xi, yj, zk, _bx, _by, _bz

def multicore():
        XX = range(-50*MMT_dim,50*MMT_dim+1,5)
        YY = range(-50*MMT_dim,50*MMT_dim+1,5)
        ZZ = range(-50*MMT_dim,50*MMT_dim+1,5)
        
        paramlist = list(itertools.product(XX,YY,ZZ))
        pool = mp.Pool(processes = 4)
        res=pool.map(job, paramlist)
        print('---> '+str(len(res)))

        arr_bx = np.zeros((50*MMT_dim+1,50*MMT_dim+1,50*MMT_dim+1))
        arr_by = np.zeros((50*MMT_dim+1,50*MMT_dim+1,50*MMT_dim+1))
        arr_bz = np.zeros((50*MMT_dim+1,50*MMT_dim+1,50*MMT_dim+1))

        for i in res:
                print(str(i[0])+'     '+str(i[1])+'     '+str(i[2])+'     '+str(i[3])+'     '+str(i[4])+'     '+str(i[5]))
                arr_bx[int(i[0]), int(i[1]), int(i[2])]=i[3]
                arr_by[int(i[0]), int(i[1]), int(i[2])]=i[4]
                arr_bz[int(i[0]), int(i[1]), int(i[2])]=i[5]
        with h5py.File('MagFieldData_array_2cm_grid_gpu.hdf5','w') as hf:
                hf.create_dataset('Bx_map',data=arr_bx)
                hf.create_dataset('By_map',data=arr_by)
                hf.create_dataset('Bz_map',data=arr_bz)

if __name__=='__main__':
        tic = time.time()
        multicore()
        toc = time.time()
        print(toc - tic)
