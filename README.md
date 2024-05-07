# Traveling Salesman Problem with ant colony optimization algorithm

## Run
- `make` to generate the cities data, compile the code and run it.
- `make NUM_CITIES=<number of cities> NUM_ANTS=<number of ants>` to specify the number of cities, to be generated, default is **100** and the number of ants, default is **10**. Then it compiles the code and runs it.

## First run
Before running the code the environment has to be prepeared. (cuda and mpich have to activated and nodes file generated). To do that automatically use
- `make before_compile`