CXXFLAGS = \
  -std=c++17 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wno-c99-extensions \
  $(OPTFLAGS) \
  $(INCLUDES)

OPTFLAGS = \
  -O2 \
  -march=native

INCLUDES = \
  -isystem ../submodules/github.com/snsinfu/h5/include \
  -isystem ../submodules/github.com/snsinfu/micromd/include

LIBRARIES = \
  -lhdf5

OBJECTS = \
  main.o \
  system_builder.o

ARTIFACTS = \
  $(OBJECTS) \
  main

.PHONY: all clean

all: main
	@:

clean:
	rm -f $(ARTIFACTS)

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBRARIES)

main.o: particle_data.hpp simulation_config.hpp system_builder.hpp
system_builder.o: particle_data.hpp simulation_config.hpp system_builder.hpp system_forcefield.hpp
