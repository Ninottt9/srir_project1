## Prepare data
`python3 generate_cities.py number_of_cities`

## Before compilation
- `source /opt/nfs/config/source_cuda121.sh`
- `source /opt/nfs/config/source_mpich420.sh`
- `/opt/nfs/config/station204_name_list.sh 1 16 > nodes`

## Compile
`mpicc main.c -lm -o main`

## RUN
`mpiexec -f nodes -n $(( 2 * $(cat nodes | wc -l) )) ./main`

## Plot results
`python3 plot.py`