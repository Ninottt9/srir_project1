# Traveling Salesman Problem with ant colony optimization algorithm

## Prepare environment
Before running the code the environment has to be prepeared. (cuda and mpich have to activated and nodes file generated). To do that use
- `source /opt/nfs/config/source_cuda121.sh`
- `source /opt/nfs/config/source_mpich420.sh`
And to generate the nodes file use
- `make generate_nodes`

## Run
- `make` to generate the cities data, compile the code and run it.
- `make NUM_CITIES=<number of cities> NUM_ANTS=<number of ants> MAX_ITERATIONS=<maximum number of iterations>` to specify the number of cities, to be generated, default is **100** and the number of ants, default is **10** and number of iterations, default is **10000**. Then it compiles the code and runs it.


## Results
The output plot of the best found route is saved in hte `output.png` file.