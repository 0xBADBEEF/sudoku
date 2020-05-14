#include <iostream>
#include <utility>
#include <stdexcept>

#include "sudoku_field.hpp"
#include "sudoku_solver.hpp"


void SudokuSolver::reduce()
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            auto coord = std::make_pair(i, j);
            for (int k = 0; k < 9; k++) {
                if (horizont[i][k][j]
                    && vertical[j][k][i]
                    && square[SudokuField::coordinates_to_square(coord)][k][SudokuField::num_in_square(coord)])
                    continue;

                horizont[i][k][j] = false;
                vertical[j][k][i] = false;
                square[SudokuField::coordinates_to_square(coord)][k][SudokuField::num_in_square(coord)] = false;
            }
        }
    }
}

void SudokuSolver::set_value(const std::pair<int, int>& coord, int val)
{
    int x = std::get<1>(coord);
    int y = std::get<0>(coord);

    solution[y][x] = true;
    left_to_do--;

    for (int k = 0; k < 9; k++) {
        // set all cells for this to false, except target one
        if (k != x)
            horizont[y][val - 1][k] = false;

        if (k != y)
            vertical[x][val - 1][k] = false;

        if (k != SudokuField::num_in_square(coord))
            square[SudokuField::coordinates_to_square(coord)][val - 1][k] = false;

        if (k != val - 1) {
            // set this sell to false in all other candidates
            horizont[y][k][x] = false;
            vertical[x][k][y] = false;
            square[SudokuField::coordinates_to_square(coord)][k][SudokuField::num_in_square(coord)] = false;
        }

    }
}

void SudokuSolver::handle_bad_sudoku()
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (solution[i][j])
                continue;

            for (int k = 0; k < 9; k++) {
                auto coord = std::make_pair(i, j);

                if (!horizont[i][k][j]
                    || !vertical[j][k][i]
                    || !square[SudokuField::coordinates_to_square(coord)][k][SudokuField::num_in_square(coord)])
                    // if somewhere is impossible - continue
                    continue;

                if (check_new_field_with_value(coord, k + 1)) {
                    field.set(coord, k + 1);
                    set_value(coord, k + 1);

                    // Crisis was resolved :)
                    return;
                }
            }
        }
    }
    // Nothing helped
    std::cout << "Invalid sudoku! Have several solutions. Cells left: " << left_to_do << std::endl;
    field.print();
    throw std::invalid_argument("Invalid sudoku!");
}

bool SudokuSolver::check_new_field_with_value(const std::pair<int, int>& coord, int val)
{
    bool res = true;

    SudokuField new_field = field;
    new_field.set(coord, val);

    SudokuSolver solver = SudokuSolver(new_field);
    try {
        solver.solve();
    } catch (...) {
        res = false;
    }

    return res;
}

bool SudokuSolver::is_only_place_in_horizon(int horizon, int num)
{
    int cnt = 0;

    for (int i = 0; i < 9; i++)
        if (horizont[horizon][num][i])
            cnt++;

    return cnt == 1;
}

bool SudokuSolver::is_only_place_in_vertical(int vert, int num)
{
    int cnt = 0;

    for (int i = 0; i < 9; i++)
        if (vertical[vert][num][i])
            cnt++;

    return cnt == 1;
}

bool SudokuSolver::is_only_place_in_square(int sq, int num)
{
    int cnt = 0;

    for (int i = 0; i < 9; i++)
        if (square[sq][num][i])
            cnt++;

    return cnt == 1;
}

SudokuSolver::SudokuSolver(SudokuField &field) : field(field)
{
    left_to_do = 81; // Init with all cells count

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            solution[i][j] = false;
            for (int k = 0; k < 9; k++) {
                horizont[i][j][k] = true;
                vertical[i][j][k] = true;
                square[i][j][k] = true;
            }
        }
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int val = field.get(std::make_pair(i, j));

            if (val == 0)
                continue;

            set_value(std::make_pair(i, j), val);
        }
    }

    reduce();
}

void SudokuSolver::solve()
{
    while (left_to_do > 0) {
        int cur_left = left_to_do;

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (solution[i][j])
                    continue;

                for (int k = 0; k < 9; k++) {
                    auto coord = std::make_pair(i, j);

                    if (!horizont[i][k][j]
                        || !vertical[j][k][i]
                        || !square[SudokuField::coordinates_to_square(coord)][k][SudokuField::num_in_square(coord)]) {
                        // if somewhere is impossible - continue
                        continue;
                    }

                    if (is_only_place_in_horizon(i, k)
                        || is_only_place_in_vertical(j, k)
                        || is_only_place_in_square(SudokuField::coordinates_to_square(coord), k)) {
                        // it is the only possible place somewhere
                        // so set it!

                        field.set(coord, k + 1);
                        set_value(coord, k + 1);
                    }
                }
            }
        }

        if (cur_left == left_to_do) {
            handle_bad_sudoku();
        }

        reduce();
    }
}

void SudokuSolver::print()
{
    // debug
    std::cout << "Solution:" << std::endl;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++)
            std::cout << solution[i][j] << " ";
        std::cout << std::endl;
    }

    std::cout << "Horizonts:" << std::endl;

    for (int i = 0; i < 9; i++) {
        std::cout << " " << i << ":" << std::endl;
        for (int j = 0; j < 9; j++) {
            std::cout << j << ": ";
            for (int k = 0; k < 9; k++)
                std::cout << horizont[i][j][k] << " ";
            std::cout << std::endl;
        }
    }

    std::cout << "Verticals:" << std::endl;

    for (int i = 0; i < 9; i++) {
        std::cout << " " << i << ":" << std::endl;
        for (int j = 0; j < 9; j++) {
            std::cout << j << ": ";
            for (int k = 0; k < 9; k++)
                std::cout << vertical[i][j][k] << " ";
            std::cout << std::endl;
        }
    }

    std::cout << "Squares:" << std::endl;

    for (int i = 0; i < 9; i++) {
        std::cout << " " << i << ":" << std::endl;
        for (int j = 0; j < 9; j++) {
            std::cout << j << ": ";
            for (int k = 0; k < 9; k++)
                std::cout << square[i][j][k] << " ";
            std::cout << std::endl;
        }
    }
}
