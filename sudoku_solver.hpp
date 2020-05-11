class SudokuSolver {
    SudokuField &field;

    bool solution[9][9];

    bool horizont[9][9][9];
    bool vertical[9][9][9];
    bool square[9][9][9];

    int left_to_do;

    void reduce();
    bool is_only_place_in_horizon(int horizon, int num);
    bool is_only_place_in_vertical(int vert, int num);
    bool is_only_place_in_square(int sq, int num);

public:
    SudokuSolver(SudokuField &field);

    void solve();
    void print();
};
