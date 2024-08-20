#ifndef FLUIDCUBE_H
#define FLUIDCUBE_H

#include <vector>

/*
    *Attributes:*
        size [int]: length of each side, hence: cube = size * size * size
        d_time [float]: derivative of time, hence: rate of time change in sim
        d_diffusion [float]: rate of diffusion, how quickly substances spread in fluid
        fluid_viscosity [float]: controls how thick/sticky fluid is (higher->more resistance)
        s [*array]: temporary storage data - FIXME
        density [*array]: desnity @ each point in fluid
*/

class FluidCube {
public:
    int size;
    float d_time;
    float d_diffusion;
    float fluid_viscosity;

    std::vector<std::vector<std::vector<float>>> s;
    std::vector<std::vector<std::vector<float>>> density;
    std::vector<std::vector<std::vector<float>>> Vx;
    std::vector<std::vector<std::vector<float>>> Vy;
    std::vector<std::vector<std::vector<float>>> Vz;
    std::vector<std::vector<std::vector<float>>> Vx0;
    std::vector<std::vector<std::vector<float>>> Vy0;
    std::vector<std::vector<std::vector<float>>> Vz0;

    FluidCube(int size, float dt, float diff, float visc);
    ~FluidCube();

    void add_density(int x, int y, int z, float amount);
    void add_velocity(int x, int y, int z, float amountX, float amountY, float amountZ);
    void step();
};

#endif // FLUIDCUBE_H
