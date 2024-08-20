#include "FluidSim.h"
#include "FluidCube.h"
#include <algorithm>

FluidSim::FluidSim(FluidCube& cube) : cube(&cube) {}

FluidSim::~FluidSim() {}

void FluidSim::set_walls (int b, std::vector<std::vector<std::vector<float>>>& x, int N){
    // For each dimension (except edges):
    for (int i = 1; i < N - 1; ++i) {
        for (int j = 1; j < N - 1; ++j) {
            for (int k = 1; k < N - 1; ++k) {

                if (b == 1) { // x-direction boundaries
                    x[0][j][k] = -x[1][j][k];
                    x[N - 1][j][k] = -x[N - 2][j][k];
                
                } else if (b == 2) { // y-direction boundaries
                    x[i][0][k] = -x[i][1][k];
                    x[i][N - 1][k] = -x[i][N - 2][k];
                
                } else if (b == 3) { // z-direction boundaries
                    x[i][j][0] = -x[i][j][1];
                    x[i][j][N - 1] = -x[i][j][N - 2];
                }

            }
        }
    }
}

// Jacobian method of approximating solution to any linear eq:
void FluidSim::diffuse (int b, std::vector<std::vector<std::vector<float>>>& x, // new 3D float matrix
    const std::vector<std::vector<std::vector<float>>>& x0, // old as baseline
    float diff, float dt, float iter, int N) {
    
    float coeff = dt * diff * (N-2) * (N-2); // coeffient in diffusion equation
    std::vector<std::vector<std::vector<float>>> xNew = x; 
    
    // Jacobi Method to approximate solution:
    for (int k = 0; k < iter; ++k) { 
        // Each Dimension
        for (int i = 1; i<N -1; ++i) { 
            for (int j = 1; j<N-1; ++j){ // iterate thru all except boundaries
                for (int m = 1; m < N - 1; ++m){
                    // Average of six neighbours
                    auto neighbours_avg = (
                        x[i-1][j][m] + x[i+1][j][m] +
                        x[i][j-1][m] + x[i][j+1][m] +
                        x[i][j][m-1] + x[i][j][m+1] );
                    
                    xNew[i][j][m] = (x0[i][j][m] + coeff * neighbours_avg) / (1 + 6 * coeff); // diffuse + normalize
                }
            }
        }
        std::swap(x, xNew);
    }
    set_walls(b, x, N);

}

void FluidSim::pressure(std::vector<std::vector<std::vector<float>>>& vx, 
                        std::vector<std::vector<std::vector<float>>>& vy, 
                        std::vector<std::vector<std::vector<float>>>& vz, 
                        std::vector<std::vector<std::vector<float>>>& p, 
                        std::vector<std::vector<std::vector<float>>>& div, 
                        int iter, int N) {
    // Calculate divergence: for each on each dimension
    for (int i = 1; i < N - 1; ++i) {
        for (int j = 1; j < N - 1; ++j) {
            for (int l = 1; l < N - 1; ++l) {
                div[i][j][l] = -0.5f * (
                    vx[i + 1][j][l] - vx[i - 1][j][l] +
                    vy[i][j + 1][l] - vy[i][j - 1][l] +
                    vz[i][j][l + 1] - vz[i][j][l - 1]
                ); // Removed / N
                p[i][j][l] = 0.0f; // Initialize pressure
            }
        }
    }
    
    // Solve the Poisson equation using diffusion
    diffuse(0, p, div, 1.0f, 1.0f, iter, N);  // Assuming p is diffused from divergence

    // Update velocities to enforce incompressibility condition
    for (int i = 1; i < N - 1; ++i) {
        for (int j = 1; j < N - 1; ++j) {
            for (int l = 1; l < N - 1; ++l) {
                vx[i][j][l] -= 0.5f * (p[i + 1][j][l] - p[i - 1][j][l]) * N;
                vy[i][j][l] -= 0.5f * (p[i][j + 1][l] - p[i][j - 1][l]) * N;
                vz[i][j][l] -= 0.5f * (p[i][j][l + 1] - p[i][j][l - 1]) * N;
            }
        }
    }
    
    // Set boundaries for velocity fields
    set_walls(1, vx, N);
    set_walls(2, vy, N);
    set_walls(3, vz, N);
}


void FluidSim::advect(int b, std::vector<std::vector<std::vector<float>>>& d, const std::vector<std::vector<std::vector<float>>>& d0,
            const std::vector<std::vector<std::vector<float>>>& Vx, const std::vector<std::vector<std::vector<float>>>& Vy,
            const std::vector<std::vector<std::vector<float>>>& Vz,
            float dt, int N) {
    float i0, i1, j0, j1, k0, k1;
    float s0, s1, t0, t1, u0, u1;
    float d0_val;

    // Iterate over entire grid: 
    for (int i = 1; i < N - 1; ++i) {
        for (int j = 1; j < N - 1; ++j) {
            for (int k = 1; k < N - 1; ++k) {
                
                // Find position of source, using current velocity: 
                float x = i - dt * N * Vx[i][j][k];
                float y = j - dt * N * Vy[i][j][k];
                float z = k - dt * N * Vz[i][j][k];

                // ensure within bounds:
                x = std::max(0.5f, std::min(x, N - 1.5f));
                y = std::max(0.5f, std::min(y, N - 1.5f));
                z = std::max(0.5f, std::min(z, N - 1.5f));

                int i0 = (int)x;
                int i1 = i0 + 1;
                int j0 = (int)y;
                int j1 = j0 + 1;
                int k0 = (int)z;
                int k1 = k0 + 1;

                // interpolate values from surrounding cells
                float s1 = x - i0;
                float s0 = 1.0f - s1;
                float t1 = y - j0;
                float t0 = 1.0f - t1;
                float u1 = z - k0;
                float u0 = 1.0f - u1;

                d[i][j][k] = 
                    s0 * (t0 * (u0 * d0[i0][j0][k0] + u1 * d0[i0][j0][k1]) + t1 * (u0 * d0[i0][j1][k0] + u1 * d0[i0][j1][k1])) +
                    s1 * (t0 * (u0 * d0[i1][j0][k0] + u1 * d0[i1][j0][k1]) + t1 * (u0 * d0[i1][j1][k0] + u1 * d0[i1][j1][k1]));
            }
        }
    }
    set_walls(b, d, N);
}

void FluidSim::step() {
    int size = cube->size;
    float d_time = cube->d_time;
    float fluid_viscosity = cube->fluid_viscosity;
    float d_diffusion = cube->d_diffusion;

    diffuse(1, cube->Vx0, cube->Vx, fluid_viscosity, d_time, 4, size);
    diffuse(2, cube->Vy0, cube->Vy, fluid_viscosity, d_time, 4, size);
    diffuse(3, cube->Vz0, cube->Vz, fluid_viscosity, d_time, 4, size);

    pressure(cube->Vx0, cube->Vy0, cube->Vz0, cube->Vx, cube->Vy, 4, size);

    advect(1, cube->Vx, cube->Vx0, cube->Vx0, cube->Vy0, cube->Vz0, d_time, size);
    advect(2, cube->Vy, cube->Vy0, cube->Vx0, cube->Vy0, cube->Vz0, d_time, size);
    advect(3, cube->Vz, cube->Vz0, cube->Vx0, cube->Vy0, cube->Vz0, d_time, size);

    pressure(cube->Vx, cube->Vy, cube->Vz, cube->Vx0, cube->Vy0, 4, size);

    diffuse(0, cube->s, cube->density, d_diffusion, d_time, 4, size);

    advect(0, cube->density, cube->s, cube->Vx, cube->Vy, cube->Vz, d_time, size);
}
