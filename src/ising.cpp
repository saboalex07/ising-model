#include <iostream>
#include <random>
#include <vector>


   static void fillRandom(std::vector<std::vector<int>> & grid, int L, std::mt19937& rng) {

        std::uniform_int_distribution<int> coin(0, 1);

        for (int i = 0; i < L; i++) {
            for (int j = 0; j < L; j++) {
                grid[i][j] = 2 * coin(rng) - 1;
            }
        }
    }

    static void printGrid(const std::vector<std::vector<int>>& grid) {
        signed int L = grid.size();
        for (int i = 0; i < L; i++) {
            for (int j = 0; j < L; j++) {
                std::cout << (grid[i][j] == 1 ? '+' : '-') << ' ';
            }
            std::cout << '\n';   // new line after each row
       }
   }

    static double magnetisation(const std::vector<std::vector<int>>& grid) {
       signed int L = grid.size();
       double sum = 0;
       for (int i = 0; i < L; i++) {
           for (int j = 0; j < L; j++) {
               sum+=grid[i][j];
           }
       }
       return sum / (L * L); //formula for magnetization
   }

    static int neighbourSum(const std::vector<std::vector<int>>& grid, int i, int j, int L) {
       int up    = (i - 1 + L) % L;   // row above
       int down  = (i + 1) % L;       // row below
       int left  = (j - 1 + L) % L;   // column to the left
       int right = (j + 1) % L;       // column to the right

       return grid[up][j] + grid[down][j] + grid[i][left] + grid[i][right];
   }

static int energy(const std::vector<std::vector<int>>& grid, int L) {
       int sum = 0;
       for (int i = 0; i < L; i++)
           for (int j = 0; j < L; j++)
               sum += grid[i][j] * neighbourSum(grid, i, j, L);
       return -sum / 2;   // each bond was counted twice
   }

int main() {
       int L = 8;
       std::vector<std::vector<int>> grid(L, std::vector<int>(L));  // L rows, each L ints
       std::mt19937 rng(42);
       fillRandom(grid, L, rng);
       printGrid(grid);
       std::cout << magnetisation(grid) << '\n';
       std::cout << energy(grid, L) << '\n';
   }
