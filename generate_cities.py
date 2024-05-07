import sys
import random

# Check if the number of cities is provided as a command-line argument
if len(sys.argv) != 2:
    print("Usage: python generate_cities.py <num_cities>")
    sys.exit(1)

# Get the number of cities from the command-line argument
try:
    num_cities = int(sys.argv[1])
    if num_cities <= 0:
        raise ValueError
except ValueError:
    print("Error: Number of cities must be a positive integer.")
    sys.exit(1)

# Generate city coordinates
with open("cities.txt", "w") as file:
    file.write(str(num_cities) + "\n")  # Write the number of cities to the first line
    for _ in range(num_cities):
        x = random.uniform(0, 1000)  # Random x-coordinate between 0 and 1000
        y = random.uniform(0, 1000)  # Random y-coordinate between 0 and 1000
        file.write(f"{x:.2f} {y:.2f}\n")  # Write city coordinates to the file

print(f"{num_cities} cities generated and saved to cities.txt.")
