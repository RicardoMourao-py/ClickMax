#!/bin/bash
#SBATCH --job-name=clique_exaustivo_mpi
#SBATCH --nodes=1
#SBATCH --partition=normal
#SBATCH --mem=500M

mpirun --allow-run-as-root ./clique_exaustivo_mpi