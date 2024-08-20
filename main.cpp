#include <iostream>
#include <fstream>  
#include "FluidCube.h"
#include "FluidSim.h"

void print_density_slice(std::ofstream& file, const FluidCube& cube, int z_slice) {
    int size = cube.size;
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            file << cube.density[x][y][z_slice] << " ";
        }
        file << std::endl;
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
    std::ofstream file("density_output.txt");
    
    for (int step = 0; step < num_steps; ++step) {
        sim.step();
        file << "Step " << step << std::endl;

        print_density_slice(file, cube, 5); 
        file << std::endl;  
    }

    file.close(); 
    return 0;
}
