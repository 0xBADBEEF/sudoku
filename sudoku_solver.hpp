class SudokuSolver {
    SudokuField &field;

    bool solution[9][9];

    bool horizont[9][9][9];
    bool vertical[9][9][9];
    bool square[9][9][9];

    int left_to_do;

    void reduce();
    void set_value(const std::pair<int, int>& coord, int val);
    void handle_bad_sudoku();
    bool check_new_field_with_value(const std::pair<int, int>& coord, int val);

    bool is_only_place_in_horizon(int horizon, int num);
    bool is_only_place_in_vertical(int vert, int num);
    bool is_only_place_in_square(int sq, int num);

public:
    SudokuSolver(SudokuField &field);

    void solve();
    void print();
};
