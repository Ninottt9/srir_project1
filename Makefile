.PHONY: all clean prepare_data before_compile generate_data

# Default value for NUM_CITIES
NUM_CITIES ?= 100

# Define variables
SOURCE_CUDA := /opt/nfs/config/source_cuda121.sh
SOURCE_MPICH := /opt/nfs/config/source_mpich420.sh
STATION_SCRIPT := /opt/nfs/config/station204_name_list.sh
NODES_FILE := nodes

# Define targets
all: main generate_data
	mpiexec -f $(NODES_FILE) -n $$(($(shell cat $(NODES_FILE) | wc -l) * 2)) ./main
	python3 plot.py

main: main.c
	mpicc main.c -lm -o main

generate_data:
	python3 generate_cities.py $(NUM_CITIES)

clean:
	rm -f main nodes cities.txt output.txt output.png

prepare_data:
	python3 generate_cities.py $(NUM_CITIES)

before_compile:
	. $(SOURCE_CUDA)
	. $(SOURCE_MPICH)
	$(STATION_SCRIPT) 1 16 > $(NODES_FILE)
