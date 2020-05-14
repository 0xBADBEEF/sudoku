#include <iostream>
#include <memory>

#include "sudoku_field.hpp"
#include "sudoku_solver.hpp"

int main()
{
    SudokuField field;

    for (int i = 0; i < 9; i++) {
        int line[9];
        for (int j = 0; j < 9; j++)
            std::cin >> line[j];
        field.add_line(line);
    }

    std::shared_ptr<SudokuSolver> solver = std::make_shared<SudokuSolver>(field);
    solver->solve();

    std::cout << "=============" << std::endl;
    std::cout << "Result:" << std::endl;
    field.print();

    return 0;
}
