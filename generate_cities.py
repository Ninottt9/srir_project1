import random

# Generate city coordinates
num_cities = 100
with open("cities.txt", "w") as file:
    file.write(str(num_cities) + "\n")  # Write the number of cities to the first line
    for _ in range(num_cities):
        x = random.uniform(0, 1000)  # Random x-coordinate between 0 and 1000
        y = random.uniform(0, 1000)  # Random y-coordinate between 0 and 1000
        file.write(f"{x:.2f} {y:.2f}\n")  # Write city coordinates to the file
