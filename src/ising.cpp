#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <fstream>
#include <chrono>

// the grid is one flat vector of N = L * L spins.
// site (i, j) at index s = i * L + j.

// neighbour table

static std::vector<int> makeNeighbourTable(int L) {
    std::vector<int> nbr(4 * L * L);
    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < L; ++j) {
            int s = i * L + j;
            nbr[4 * s + 0] = ((i - 1 + L) % L) * L + j; // up
            nbr[4 * s + 1] = ((i + 1) % L) * L + j;     // down
            nbr[4 * s + 2] = i * L + (j - 1 + L) % L;   // left
            nbr[4 * s + 3] = i * L + (j + 1) % L;       // right
        }
    }
    return nbr;
}

static void fillRandom(std::vector<int>& grid, std::mt19937& rng) {
    std::uniform_int_distribution coin(0, 1);
    for (int s = 0; s < static_cast<int>(grid.size()); ++s) {
        grid[s] = 2 * coin(rng) - 1;
    }
}



// sum of all spins
static int total_Magnetization(const std::vector<int>& grid) {
    int sum = 0;
    for (int s = 0; s < static_cast<int>(grid.size()); ++s) {
        sum += grid[s];
    }
    return sum;
}

// sum of the four neighbours of s
static int neighbourSum(const std::vector<int>& grid, const std::vector<int>& nbr, int s) {
    return grid[nbr[4 * s + 0]] + grid[nbr[4 * s + 1]] + grid[nbr[4 * s + 2]] + grid[nbr[4 * s + 3]];
}

// total energy
static int total_Energy(const std::vector<int>& grid, const std::vector<int>& nbr) {
    int sum = 0;
    for (int s = 0; s < static_cast<int>(grid.size()); ++s) {
        sum += grid[s] * neighbourSum(grid, nbr, s);
    }
    return -sum / 2;
}


static void sweep(std::vector<int>& grid, const std::vector<int>& nbr, double T, std::mt19937& rng, int& E, int& M) {
    const int N = static_cast<int>(grid.size());
    std::uniform_int_distribution site(0, N - 1); // picks site
    std::uniform_real_distribution u(0.0, 1.0);

    // precalc of possible positive dE
    double w[9] = {};
    w[4] = std::exp(-4.0 / T);
    w[8] = std::exp(-8.0 / T);

    for (int attempt = 0; attempt < N; ++attempt) {
        int s = site(rng);
        int dE = 2 * grid[s] * neighbourSum(grid, nbr, s);
        if (dE <= 0 || u(rng) < w[dE]) {
            grid[s] = -grid[s];          // flip
            E += dE;                     // energy changes by dE
            M += 2 * grid[s];            // M changes by 2s
        }
    }
}

int main() {
    auto start = std::chrono::steady_clock::now();

    const int L = 16;
    const int N = L * L;
    const int equilibrationSweeps = 1000;
    const int measurementSweeps = 5000;

    std::mt19937 rng(42);
    std::vector<int> grid(N);
    fillRandom(grid, rng);
    const std::vector<int> nbr = makeNeighbourTable(L);

    std::ofstream fout("data/magnetization.csv");
    if (!fout) {
        std::cerr << "Unable to open data/magnetization.csv\n";
        return 1;
    }
    fout << "T,m,e\n"; // header

    for (int k = 0; k <= 20; ++k) {
        const double T = 3.5 - 0.1 * k;
        int E = total_Energy(grid, nbr);           // start values for this temperature
        int M = total_Magnetization(grid);

        for (int i = 0; i < equilibrationSweeps; ++i) {
            sweep(grid, nbr, T, rng, E, M);
        }

        double mag_sum = 0.0;
        double energy_sum = 0.0;
        for (int i = 0; i < measurementSweeps; ++i) {
            sweep(grid, nbr, T, rng, E, M);
            mag_sum += std::abs(M);          // running totals
            energy_sum += E;
        }

        const double m_avg = mag_sum / measurementSweeps / N;
        const double e_avg = energy_sum / measurementSweeps / N;

        std::cout << T << ',' << m_avg << ',' << e_avg << '\n';
        fout << T << ',' << m_avg << ',' << e_avg << '\n';
    }
    fout.close();

    auto end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double>(end - start).count() << " s\n";
    return 0;
}