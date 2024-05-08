.PHONY: all clean prepare_data generate_nodes generate_data

# Default value for NUM_CITIES
NUM_CITIES ?= 100
NUM_ANTS ?= 10

# Define variables
STATION_SCRIPT := /opt/nfs/config/station204_name_list.sh
NODES_FILE := nodes

# Define targets
all: main generate_data
	mpiexec -f $(NODES_FILE) -n $$(($(shell cat $(NODES_FILE) | wc -l) * 2)) ./main $(NUM_ANTS)
	python3 plot.py

main: main.c
	mpicc main.c -lm -o main

generate_data:
	python3 generate_cities.py $(NUM_CITIES)

clean:
	rm -f main nodes cities.txt output.txt output.png

prepare_data:
	python3 generate_cities.py $(NUM_CITIES)

generate_nodes:
	$(STATION_SCRIPT) 1 16 > $(NODES_FILE)
