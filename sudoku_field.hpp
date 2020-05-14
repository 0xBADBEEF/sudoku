class SudokuField {
    int sudoku_field[9][9];
    int line_create;

public:
    SudokuField();

    void add_line(int line[9]);
    void print();

    int get(const std::pair<int, int>& coord);
    void set(const std::pair<int, int>& coord, int val);

    static int coordinates_to_square(const std::pair<int, int>& coord);
    static int num_in_square(const std::pair<int, int>& coord);
};
