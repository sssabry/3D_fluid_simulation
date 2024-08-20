#ifndef FLUIDSIM_H
#define FLUIDSIM_H

#include <vector>

class FluidCube;  // Forward declaration

class FluidSim {
public:
    FluidSim(FluidCube& cube);
    ~FluidSim();
    void step();

private:
    void advect(int b, std::vector<std::vector<std::vector<float>>>& d, 
                const std::vector<std::vector<std::vector<float>>>& d0, 
                const std::vector<std::vector<std::vector<float>>>& vx, 
                const std::vector<std::vector<std::vector<float>>>& vy, 
                const std::vector<std::vector<std::vector<float>>>& vz, 
                float dt, int N);
    void pressure(std::vector<std::vector<std::vector<float>>>& vx, 
                 std::vector<std::vector<std::vector<float>>>& vy, 
                 std::vector<std::vector<std::vector<float>>>& vz, 
                 std::vector<std::vector<std::vector<float>>>& p, 
                 std::vector<std::vector<std::vector<float>>>& div, 
                 int iter, int N);
    void set_walls(int b, std::vector<std::vector<std::vector<float>>>& x, int N);

    void diffuse(int b, std::vector<std::vector<std::vector<float>>> &x, const std::vector<std::vector<std::vector<float>>> &x0, float diff, float dt, float iter, int N);

    FluidCube* cube;
};

#endif // FLUIDSIM_H
