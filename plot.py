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

plt.scatter([city[0] for city in cities], [city[1] for city in cities], color='b', label='Cities', zorder=5)


for i in range(num_cities):
    for j in range(i+1, num_cities):
        plt.plot([cities[i][0], cities[j][0]], [cities[i][1], cities[j][1]], color='lightgray', linewidth=0.5, zorder=0)


# Plot the best tour
x = [cities[i][0] for i in tour]
y = [cities[i][1] for i in tour]
x.append(x[0])  
y.append(y[0])
plt.plot(x, y,  linestyle='-', color='r', label='Best Tour', zorder=10)

plt.annotate("start and finish", xy=(cities[tour[0]][0], cities[tour[0]][1]), xytext=(0, 0), size=15, color='g',
            arrowprops=dict(facecolor='g', width=1), zorder=20)
# plt.text(, , "start and finish", color='g', size=20, zorder=20)

for i in range(len(tour)-1):
    x = (cities[tour[i + 1]][0] + cities[tour[i]][0])/2 
    y = (cities[tour[i + 1]][1] + cities[tour[i]][1])/2
    max_len = max(abs(x - y), abs(y - x))
    dx = (cities[tour[i + 1]][0] -  cities[tour[i]][0]) / max_len 
    dy = (cities[tour[i + 1]][1] -  cities[tour[i]][1]) / max_len
    plt.arrow(x , y , dx, dy,  color='r', head_width=7, head_length=10, zorder=15)

x = (cities[tour[-1]][0] + cities[tour[0]][0])/2 
y = (cities[tour[-1]][1] + cities[tour[0]][1])/2
max_len = max(abs(x - y), abs(y - x))
dx = (cities[tour[0]][0] - cities[tour[-1]][0]) / max_len 
dy = (cities[tour[0]][1] - cities[tour[-1]][1]) / max_len

plt.arrow(x, y, dx, dy,
          color='r',  head_width=7, head_length=10, zorder=15)

plt.title("Best Tour Found by Ant Colony Optimization\nThe best length = " +  str(best_tour_length))
plt.xlabel("X-coordinate")
plt.ylabel("Y-coordinate")
plt.legend()
plt.grid(True)

plt.gcf().set_size_inches(20, 16)

# Save the plot to a file
fileName = 'output.png'
plt.savefig(fileName)
print('plot saved to the', fileName)
