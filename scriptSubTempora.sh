#!/bin/bash

#SBATCH --job-name SubTempora_job1
#SBATCH --error /home/cw24wity/Repos/SubTempora-latest/SubTempora.%j.err
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 01
#SBATCH --mem-per-cpu=5000
#SBATCH --time 01:00:00
#SBATCH --mail-type=END

echo "hello"

#SBATCH --exclusive
module purge
module load gcc

OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
export OMP_NUM_THREADS

cd /home/cw24wity/Repos/SubTempora-latest/
srun SubTempora.exe
EXITCODE=$?

# any cleanup and copy commands:
# end this job script with precisely the exit status of your scientific program above:
exit $EXITCODE