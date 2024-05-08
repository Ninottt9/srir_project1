#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <float.h>
#include <mpi.h>

// Define structures and global variables here
#define MAX_CITIES 10000 // Maximum number of cities
#define MAX_ANTS 1000    // Maximum number of ants

// Define structures
typedef struct
{
    int x;
    int y;
} City;

typedef struct
{
    int tour[MAX_CITIES];    // Ant's tour
    double tour_length;      // Length of the tour
    int visited[MAX_CITIES]; // Visited cities
} Ant;

double pheromones[MAX_CITIES][MAX_CITIES]; // Pheromone trails
City *cities;                              // Array to hold cities
Ant ants[MAX_ANTS];                        // Array to hold ants

int num_cities = 100; // fallvack value
int num_ants = 10;    // fallback value

// Constants for ACO parameters
#define ALPHA 1.0 // Pheromone importance
#define BETA 2.0  // Heuristic information importance
#define Q 10.0    // Pheromone deposit constant
#define RHO 0.5   // Pheromone evaporation rate

// Function declarations
void initialize();
void ant_movement();
void update_pheromones();
void communicate();
void terminate();
int select_next_city(int ant_index, int current_city);
double distance(int city1, int city2);
bool termination_condition_met();

int iteration = 0;
const int max_iterations = 10000; // Maximum number of iterations (adjust as needed)
int rank, size;


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <num_ants>\n", argv[0]);
        return EXIT_FAILURE;
    }
    num_ants = atoi(argv[1]);
    double start_time, end_time;
    double best_tour_length = DBL_MAX;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Initialize ACO algorithm
    initialize();

    // Start timer
    start_time = MPI_Wtime();

    // Main loop of ACO algorithm
    while (!termination_condition_met())
    {
        // Simulate ant movement
        ant_movement();
        // Update pheromones
        update_pheromones();
        // Update best tour length found so far
        for (int i = 0; i < num_ants; i++)
        {
            if (ants[i].tour_length < best_tour_length)
            {
                best_tour_length = ants[i].tour_length;
            }
        }

        // Print intermediate output (optional)
        if (rank == 0 && iteration % 10 == 0)
        {
            printf("Iteration %d: Best tour length so far: %.2f\n", iteration, best_tour_length);
        }

        iteration++;
    }

    // Stop timer
    end_time = MPI_Wtime();

    // Output results
    if (rank == 0)
    {
        printf("Best tour found:\n");
        printf("Total runtime: %.2f seconds\n", end_time - start_time);
        printf("Number of iterations: %d\n", iteration);

        // Write the best tour to a file
        FILE *fp = fopen("output.txt", "w");
        if (fp == NULL)
        {
            printf("Error: Unable to open file for writing.\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        fprintf(fp, "%.2f\n", best_tour_length);
        for (int i = 0; i < num_cities; i++)
        {
            fprintf(fp, "%d ", ants[0].tour[i]); // Assume first ant has the best tour
        }
        fprintf(fp, "\n");
        fclose(fp);
        printf("Best tour written to file: best_tour.txt\n");
    }

    MPI_Finalize();
    return 0;
}

void initialize()
{
    // Read city coordinates from file
    FILE *fp = fopen("cities.txt", "r");
    if (fp == NULL)
    {
        printf("Error: Unable to open file for reading.\n");
        exit(EXIT_FAILURE);
    }
    // Read number of cities
    fscanf(fp, "%d", &num_cities);
    // Allocate memory for city coordinates
    cities = (City *)malloc(num_cities * sizeof(City));
    if (cities == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    // Read city coordinates
    for (int i = 0; i < num_cities; i++)
    {
        fscanf(fp, "%lf %lf", &cities[i].x, &cities[i].y);
    }
    fclose(fp);

    // Initialize pheromone trails
    for (int i = 0; i < num_cities; i++)
    {
        for (int j = 0; j < num_cities; j++)
        {
            pheromones[i][j] = 1.0; // Initialize pheromones to a constant value
        }
    }

    // Initialize ants
    for (int i = 0; i < num_ants; i++)
    {
        ants[i].tour_length = 0.0;
        for (int j = 0; j < num_cities; j++)
        {
            ants[i].tour[j] = -1;   // Initialize ant's tour to -1 (unvisited)
            ants[i].visited[j] = 0; // Initialize visited array to 0
        }
        // Place each ant in a random city
        int start_city = rand() % num_cities;
        ants[i].tour[0] = start_city;
        ants[i].visited[start_city] = 1;
    }
}

void ant_movement()
{
    for (int i = 0; i < num_ants; i++)
    {
        // Reset ant's tour, visited cities, and tour length
        ants[i].tour_length = 0.0;
        // Reset ant's tour and visited cities
        for (int j = 0; j < num_cities; j++)
        {
            ants[i].tour[j] = -1;   // Reset tour
            ants[i].visited[j] = 0; // Reset visited cities
        }
        // Place ant in a random city to start its tour
        int start_city = rand() % num_cities;
        ants[i].tour[0] = start_city;
        ants[i].visited[start_city] = 1;

        // Construct ant's tour
        for (int j = 1; j < num_cities; j++)
        {
            int current_city = ants[i].tour[j - 1];
            int next_city = select_next_city(i, current_city);
            ants[i].tour[j] = next_city;
            ants[i].visited[next_city] = 1;
            ants[i].tour_length += distance(current_city, next_city);
        }
        // Add distance from last city back to start city to complete the tour
        ants[i].tour_length += distance(ants[i].tour[num_cities - 1], ants[i].tour[0]);
    }
}

int select_next_city(int ant_index, int current_city)
{
    double total_probability = 0.0;
    double probabilities[num_cities];
    int unvisited_count = 0;
    int unvisited_cities[num_cities];

    // Calculate total probability and identify unvisited cities
    for (int i = 0; i < num_cities; i++)
    {
        if (ants[ant_index].visited[i] == 0)
        {
            double pheromone = pheromones[current_city][i];
            double heuristic = 1.0 / distance(current_city, i); // Heuristic information
            probabilities[i] = pow(pheromone, ALPHA) * pow(heuristic, BETA);
            total_probability += probabilities[i];
            unvisited_cities[unvisited_count++] = i;
        }
        else
        {
            probabilities[i] = 0.0; // Ant has visited this city already
        }
    }

    // If all cities have been visited, return the current city
    if (unvisited_count == 0)
    {
        return current_city;
    }

    // Select next city based on probabilities
    double random_value = (double)rand() / RAND_MAX;
    double cumulative_probability = 0.0;
    for (int i = 0; i < unvisited_count; i++)
    {
        int city = unvisited_cities[i];
        cumulative_probability += probabilities[city] / total_probability;
        if (random_value <= cumulative_probability)
        {
            return city;
        }
    }

    // If no city is selected (due to precision issues), return the last unvisited city
    return unvisited_cities[unvisited_count - 1];
}

double distance(int city1, int city2)
{
    // Euclidean distance between two cities
    return sqrt(pow(cities[city1].x - cities[city2].x, 2) + pow(cities[city1].y - cities[city2].y, 2));
}

void update_pheromones()
{
    // Evaporate pheromones locally
    for (int i = 0; i < num_cities; i++)
    {
        for (int j = 0; j < num_cities; j++)
        {
            pheromones[i][j] *= (1.0 - RHO); // Local evaporation
            // Ensure pheromone values are within a certain range (optional)
            if (pheromones[i][j] < 0.0)
            {
                pheromones[i][j] = 0.0;
            }
            else if (pheromones[i][j] > 100.0)
            { // Maximum pheromone value (adjust as needed)
                pheromones[i][j] = 100.0;
            }
        }
    }

    // Allocate memory for the local pheromone matrix segment
    double local_pheromones[num_cities][num_cities];

    // Scatter the pheromones array among processes
    MPI_Scatter(pheromones, num_cities * num_cities / size, MPI_DOUBLE, local_pheromones, num_cities * num_cities / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Deposit pheromones locally by ants
    for (int i = 0; i < num_ants; i++)
    {
        double tour_length = ants[i].tour_length;
        // Deposit pheromones along ant's tour
        for (int j = 0; j < num_cities - 1; j++)
        {
            int city1 = ants[i].tour[j];
            int city2 = ants[i].tour[j + 1];
            // Update local pheromone trail for this edge
            local_pheromones[city1][city2] += Q / tour_length;
            local_pheromones[city2][city1] += Q / tour_length; // Trails are symmetric
        }
        // Update local pheromone trail for the last edge back to the start city
        local_pheromones[ants[i].tour[num_cities - 1]][ants[i].tour[0]] += Q / tour_length;
        local_pheromones[ants[i].tour[0]][ants[i].tour[num_cities - 1]] += Q / tour_length; // Trails are symmetric
    }

    // Gather updated local pheromones from all processes
    MPI_Gather(local_pheromones, num_cities * num_cities / size, MPI_DOUBLE, pheromones, num_cities * num_cities / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

bool termination_condition_met()
{
    // Implement your termination criterion here
    // For example, you could check if a maximum number of iterations has been reached
    return (iteration >= max_iterations);
}

void terminate()
{
    // Implement the termination logic here
    if (termination_condition_met())
    {
        // Terminate the algorithm
        // Optionally, you can perform additional tasks before terminating
        printf("Termination condition met. Algorithm terminated.\n");
        exit(EXIT_SUCCESS);
    }
}
