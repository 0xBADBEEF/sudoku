#include <iostream>
#include <utility>

#include "sudoku_field.hpp"

SudokuField::SudokuField()
{
    line_create = 0;
}

void SudokuField::add_line(int line[9])
{
    for (int i = 0; i < 9; i++)
        sudoku_field[line_create][i] = line[i];

    line_create++;
}

void SudokuField::print()
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            std::cout << sudoku_field[i][j];
            if ((j + 1) % 3 == 0)
                std::cout << "|";
            else
                std::cout << " ";
        }
        std::cout << std::endl;
        if ((i + 1) % 3 == 0)
            std::cout << "------------------" << std::endl;
    }
}

int SudokuField::get(std::pair<int, int> coord)
{
    return sudoku_field[std::get<0>(coord)][std::get<1>(coord)];
}

void SudokuField::set(std::pair<int, int> coord, int val)
{
    sudoku_field[std::get<0>(coord)][std::get<1>(coord)] = val;
}

int SudokuField::coordinates_to_square(std::pair<int, int> coord)
{
    if (std::get<1>(coord) < 3) {
        // 1, 4, 7
        if (std::get<0>(coord) < 3)
            return 0;
        else if (std::get<0>(coord) < 6)
            return 3;
        else
            return 6;
    } else if (std::get<1>(coord) < 6) {
        // 2, 5, 8
        if (std::get<0>(coord) < 3)
            return 1;
        else if (std::get<0>(coord) < 6)
            return 4;
        else
            return 7;
    } else {
        // 3, 6, 9
        if (std::get<0>(coord) < 3)
            return 2;
        else if (std::get<0>(coord) < 6)
            return 5;
        else
            return 8;
    }
}

int SudokuField::num_in_square(std::pair<int, int> coord)
{
    int x = std::get<1>(coord);
    int y = std::get<0>(coord);

    while (x > 2) {
        x -= 3;
    }

    while (y > 2) {
        y -= 3;
    }

    if (x == 0) {
        // 0, 3, 6
        if (y == 0)
            return 0;
        else if (y == 1)
            return 3;
        else
            return 6;
    } else if (x == 1) {
        // 1, 4, 7
        if (y == 0)
            return 1;
        else if (y == 1)
            return 4;
        else
            return 7;
    } else {
        // 2, 5, 8
        if (y == 0)
            return 2;
        else if (y == 1)
            return 5;
        else
            return 8;
    }
}
