#include <iostream>
#include "FluidCube.h"
#include "FluidSim.h"

// print a slice of the density array for debugging
void print_density_slice(const FluidCube& cube, int z_slice) {
    int size = cube.size;
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            std::cout << cube.density[x][y][z_slice] << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    int size = 10;  
    float dt = 0.1f; 
    float diffusion = 0.1f; 
    float viscosity = 0.1f;  

    FluidCube cube(size, dt, diffusion, viscosity);
    FluidSim sim(cube);

    cube.add_density(size / 2, size / 2, size / 2, 10.0f);  // Add density in the center
    cube.add_velocity(size / 2, size / 2, size / 2, 1.0f, 0.0f, 0.0f);  // Add initial velocity

    int num_steps = 10;

    for (int step = 0; step < num_steps; ++step) {
        std::cout << "Step " << step << std::endl;
        sim.step(); 

        // Print a slice of the density array for debugging
        std::cout << "Density slice at z=" << size / 2 << ":" << std::endl;
        print_density_slice(cube, size / 2);

        std::cout << std::endl;
    }

    return 0;
}
