#include <vector>
#include <iostream>
#include <string>
#include "FluidSim.h"
#include "FluidCube.h"


FluidCube::FluidCube(int size, float dt, float diff, float visc)
    : size(size), d_time(dt), d_diffusion(diff), fluid_viscosity(visc),
      s(size, std::vector<std::vector<float>>(size, std::vector<float>(size, 0.0f))),
      density(size, std::vector<std::vector<float>>(size, std::vector<float>(size, 0.0f))),
      Vx(size, std::vector<std::vector<float>>(size, std::vector<float>(size, 0.0f))),
      Vy(size, std::vector<std::vector<float>>(size, std::vector<float>(size, 0.0f))),
      Vz(size, std::vector<std::vector<float>>(size, std::vector<float>(size, 0.0f))),
      Vx0(size, std::vector<std::vector<float>>(size, std::vector<float>(size, 0.0f))),
      Vy0(size, std::vector<std::vector<float>>(size, std::vector<float>(size, 0.0f))),
      Vz0(size, std::vector<std::vector<float>>(size, std::vector<float>(size, 0.0f))) {
    std::cout << "FluidCube created with size " << size << "." << std::endl;
}
FluidCube::~FluidCube() {
    std::cout << "FluidCube destroyed" << std::endl;
}

void FluidCube::add_density(int x, int y, int z, float amount) {
    density[x][y][z] += amount;
}

// Add velocity @ position
void FluidCube::add_velocity(int x, int y, int z, float amountX, float amountY, float amountZ){
    Vx[x][y][z] += amountX;
    Vy[x][y][z] += amountY;
    Vz[x][y][z] += amountZ;

}
