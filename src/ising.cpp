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

int main() {
       int L = 8;
       std::vector<std::vector<int>> grid(L, std::vector<int>(L));  // L rows, each L ints
       std::mt19937 rng(42);
       fillRandom(grid, L, rng);
       printGrid(grid);
   }
