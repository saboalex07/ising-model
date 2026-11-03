#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <fstream>
#include <chrono>


static void fillRandom(std::vector<std::vector<int> > &grid, int L, std::mt19937 &rng) {
    std::uniform_int_distribution<int> coin(0, 1);

    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < L; ++j) {
            grid[i][j] = 2 * coin(rng) - 1;
        }
    }
}

static void printGrid(const std::vector<std::vector<int> > &grid) {
    signed int L = grid.size();
    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < L; ++j) {
            std::cout << (grid[i][j] == 1 ? '+' : '-') << ' ';
        }
        std::cout << '\n'; // new line after each row
    }
}

static int total_magnetization(const std::vector<std::vector<int>>& grid) {
    int L = grid.size();
    int sum = 0;
    for (int i = 0; i < L; ++i)
        for (int j = 0; j < L; ++j)
            sum += grid[i][j];
    return sum; // total magentization
}

static int neighbourSum(const std::vector<std::vector<int> > &grid, int i, int j, int L) {
    int up = (i - 1 + L) % L; // row above
    int down = (i + 1) % L; // row below
    int left = (j - 1 + L) % L; // column to the left
    int right = (j + 1) % L; // column to the right

    return grid[up][j] + grid[down][j] + grid[i][left] + grid[i][right];
}

static int energy(const std::vector<std::vector<int> > &grid, int L) {
    int sum = 0;
    for (int i = 0; i < L; ++i)
        for (int j = 0; j < L; ++j)
            sum += grid[i][j] * neighbourSum(grid, i, j, L);
    return -sum / 2; // each bond was counted twice
}

static void sweep(std::vector<std::vector<int> > &grid, double T, std::mt19937 &rng, int& E, int& M) {
    signed int L = grid.size();

    double w[9];
    w[4] = std::exp(-4 / T); // precomputes possible positive dE
    w[8] = std::exp(-8 / T);

    std::uniform_int_distribution<int> site(0, L - 1);
    std::uniform_real_distribution<double> u(0.0, 1.0);
    for (int i = 0; i < L * L; ++i) {
        int row = site(rng); // picks at random
        int col = site(rng); // picks at random
        int dE = 2 * grid[row][col] * neighbourSum(grid, row, col, L);

        if (dE <= 0 || u(rng) < w[dE]) {
            grid[row][col] *= -1;        // flip
            E += dE;                     // energy changes by dE
            M += 2 * grid[row][col];     // spin went from -s to s, so M changes by 2s
        }
    }
}

int main() {

    auto start = std::chrono::high_resolution_clock::now();

    int L = 16;
    std::vector grid(L, std::vector<int>(L));
    std::mt19937 rng(42);
    fillRandom(grid, L, rng);

    const int equilibrationSweeps = 1000;
    const int measurementSweeps = 5000;

    std::ofstream fout("data/magnetization.csv");
    if (!fout) {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }
    fout << "T,m,e\n"; // header


    for (int k = 0; k <= 20; ++k) {
        double T = 3.5 - 0.1 * k;

        int E = energy(grid, L);
        int M = total_magnetization(grid);

        for (int i = 0; i < equilibrationSweeps; ++i) {
            sweep(grid, T, rng, E, M);
        }

        double mag_sum = 0.0;
        double energy_sum = 0.0;

        for (int i = 0; i < measurementSweeps; ++i) {
            sweep(grid, T, rng, E, M);
            mag_sum += std::abs(M); //running totals
            energy_sum += E;
        }

        double m_avg = mag_sum / measurementSweeps / (L * L);
        double e_avg = energy_sum / measurementSweeps / (L * L);   // average, then per spin

        std::cout << T << ',' << m_avg << ',' << e_avg << '\n';
        fout      << T << ',' << m_avg << ',' << e_avg << '\n';

    }
    fout.close();

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << duration.count() << '\n';

    return 0;
}
