#!/bin/bash
#SBATCH -A "RDNS1"
#SBATCH --time=4-00:00:00
#SBATCH -o slurm-%A_%a.out
#SBATCH -e slurm-%A_%a.err
#SBATCH -p gpu
#SBATCH -N 1

module load python/3.7.2
#2.7.3
source /people/bhat731/CosmicMuTrap/SimBoxMMT/env.sh

python MagField8.py 10
#python MagArray.py 'MagFieldData_list_3D_gpu.hdf5'
