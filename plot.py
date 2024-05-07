import matplotlib.pyplot as plt

# Read the best tour from the file
with open("output.txt", "r") as file:
    best_tour_length = float(file.readline().strip())  # Read the best tour length
    tour = list(map(int, file.readline().split()))  # Read the tour sequence

# Read the city coordinates from the file
cities = []
with open("cities.txt", "r") as file:
    num_cities = int(file.readline().strip())
    for _ in range(num_cities):
        x, y = map(float, file.readline().split())
        cities.append((x, y))

# Plot the cities
plt.scatter([city[0] for city in cities], [city[1] for city in cities], color='b', label='Cities')

# Plot the best tour
x = [cities[i][0] for i in tour]
y = [cities[i][1] for i in tour]
x.append(x[0])  # Connect the last city to the first city to close the loop
y.append(y[0])
plt.plot(x, y, marker='o', linestyle='-', color='r', label='Best Tour')

plt.title("Best Tour Found by Ant Colony Optimization")
plt.xlabel("X-coordinate")
plt.ylabel("Y-coordinate")
plt.legend()
plt.grid(True)

# Save the plot to a file
fileName = 'output.png'
plt.savefig(fileName)
print('plot saved to the', fileName)
