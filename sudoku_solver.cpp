#include <iostream>
#include <utility>
#include <stdexcept>

#include "sudoku_field.hpp"
#include "sudoku_solver.hpp"


void SudokuSolver::reduce()
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            for (int k = 0; k < 9; k++) {
                if (horizont[i][k][j] && vertical[j][k][i] && square[SudokuField::coordinates_to_square(std::make_pair<>(i, j))][k][SudokuField::num_in_square(std::make_pair<>(i, j))])
                    continue;

                horizont[i][k][j] = false;
                vertical[j][k][i] = false;
                square[SudokuField::coordinates_to_square(std::make_pair<>(i, j))][k][SudokuField::num_in_square(std::make_pair<>(i, j))] = false;
            }
        }
    }
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
            int val = field.get(std::make_pair<>(i, j));

            if (val == 0)
                continue;

            solution[i][j] = true;
            left_to_do--;

            for (int k = 0; k < 9; k++) {
                // set all cells for this to false, except target one
                if (k != j)
                    horizont[i][val - 1][k] = false;

                if (k != i)
                    vertical[j][val - 1][k] = false;

                if (k != SudokuField::num_in_square(std::make_pair<>(i, j)))
                    square[SudokuField::coordinates_to_square(std::make_pair<>(i, j))][val - 1][k] = false;

                if (k != val - 1) {
                    // set this sell to false in all other candidates
                    horizont[i][k][j] = false;
                    vertical[j][k][i] = false;
                    square[SudokuField::coordinates_to_square(std::make_pair<>(i, j))][k][SudokuField::num_in_square(std::make_pair<>(i, j))] = false;
                }

            }
        }
    }

    reduce();
}

void SudokuSolver::solve()
{
    int cnt = 0;


    while (left_to_do > 0) {
        int cur_left = left_to_do;

        std::cout << "==============" << std::endl;
        std::cout << "Step " << cnt << std::endl;
        cnt++;

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (solution[i][j])
                    continue;

                for (int k = 0; k < 9; k++) {
                    if (!horizont[i][k][j]
                        || !vertical[j][k][i]
                        || !square[SudokuField::coordinates_to_square(std::make_pair<>(i, j))][k][SudokuField::num_in_square(std::make_pair<>(i, j))]) {
                        // if somewhere is impossible - continue
                        continue;
                    }

                    if (is_only_place_in_horizon(i, k)
                        || is_only_place_in_vertical(j, k)
                        || is_only_place_in_square(SudokuField::coordinates_to_square(std::make_pair<>(i, j)), k)) {
                        // it is the only possible place somewhere
                        // so set it!

                        field.set(std::make_pair(i, j), k + 1);
                        solution[i][j] = true;
                        left_to_do--;

                        for (int m = 0; m < 9; m++) {
                            if (m != k) {
                                // set this sell to false in all other candidates
                                horizont[i][m][j] = false;
                                vertical[j][m][i] = false;
                                square[SudokuField::coordinates_to_square(std::make_pair<>(i, j))][m][SudokuField::num_in_square(std::make_pair<>(i, j))] = false;
                            }
                            if (m != j)
                                horizont[i][k][m] = false;

                            if (m != i)
                                vertical[j][k][m] = false;

                            if (m != SudokuField::num_in_square(std::make_pair<>(i, j)))
                                square[SudokuField::coordinates_to_square(std::make_pair<>(i, j))][k][m] = false;
                        }
                    }
                }
            }
        }

        if (cur_left == left_to_do) {
            std::cout << "Invalid sudoku! Have several solutions" << std::endl;
            field.print();
            throw std::invalid_argument("Invalid sudoku!");
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
