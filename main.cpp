#include <chrono>
#include <thread>
#include <array>
#include <iostream>

/**
 * @brief Conway's Game of Life on a 2D grid of variable size N x M
 * @tparam N number of rows
 * @tparam M number of columns
 */
template<size_t N, size_t M>
class GameOfLife
{
public:
    using Grid = std::array<std::array<bool, M>, N>;

    /**
     * @brief Update world according to the rules of Life
     */
    void update()
    {
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < M; ++j)
                future_grid[i][j] = evolveCell(i, j);

        current_grid = future_grid;
    }

    /**
     * @brief Set the current grid to a specific configuration
     * @param g grid to be specified
     */
    void setGrid(Grid &g) { std::copy(&g[0][0], &g[0][0] + N * M, &current_grid[0][0]); }

    /**
     * @brief Receive a reference to the current grid
     * @return reference to current grid
     */
    Grid &getGrid() { return current_grid; }

private:
    static constexpr auto NumberOfNeighbours = 8;
    std::array<std::pair<int, int>, NumberOfNeighbours> NeighbourOffsets
        {{{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};

    /**
     * @brief Evolve a single cell at point (i, j) on current grid
     * @param i i-th row
     * @param j j-th column
     * @return boolean value which indicates whether the cell is alive in future grid
     */
    bool evolveCell(const size_t i, const size_t j) const
    {
        const auto current_state = current_grid[i][j];
        const auto neighbours = numberOfAliveNeighbours(i, j);

        if (current_state)
            return !(neighbours < 2 || neighbours > 3);
        else
            return neighbours == 3;
    }

    /**
     * @brief Given point (i, j) on a grid, determine the number of alive neighbours
     * @param i i-th row
     * @param j j-th column
     * @return number of alive neighbours
     */
    size_t numberOfAliveNeighbours(const size_t i, const size_t j) const
    {
        size_t alive_neighbours = 0;
        for (const auto &offset: NeighbourOffsets) {
            const auto x = i + offset.first;
            const auto y = j + offset.second;

            if (x >= 0 && y >= 0 && x < N && y < M)
                alive_neighbours = current_grid[x][y] ? alive_neighbours + 1 : alive_neighbours;
        }

        return alive_neighbours;
    }

    Grid current_grid{}, future_grid{};
};

/**
 * @brief A program that prints the tring "Hello, World!" to the terminal and exits
 * @return 0
 */
int main()
{
    using GOL = GameOfLife<10, 10>;

    const auto print_grid = [](GOL &gol)
    {
        for (auto &outer: gol.getGrid()) {
            for (auto &cell: outer)
                std::cout << cell << " ";
            std::cout << "\n";
        }
        std::cout << "\n";
    };

    GOL gol;
    GOL::Grid glider_layout {{
         {false, true, false},
         {false, false, true},
         {true,  true,  true},
         {false, false, false}
    }};
    gol.setGrid(glider_layout);
    print_grid(gol);

    for (int i = 0; i < 100; ++i) {
        gol.update();
        print_grid(gol);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return 0;
}
