#include <iostream>
#include <cstdlib>
#include <cmath>

int parseInt(std::string number) {
    try {
        return stoi(number);
    }
    catch(std::invalid_argument) {
        return -1;
    }
}

void drawBoard(char *board, int board_size) {
    std::system("cls");
    std::cout << std::string(board_size*6 + 1, '*') << '\n';
    for(int i = 0; i < board_size; i++) {
        std::cout << '*';
        for(int j = 0; j < board_size; j++) {
            std::cout << "     ";
            if(j != board_size - 1)
                std::cout << '|';
        }
        std::cout << '*' << '\n' << '*';
        for(int j = 0; j < board_size; j++) {
            std::cout << "  " << board[i*board_size + j] << "  ";
            if(j != board_size - 1)
                std::cout << '|';
        }
        std::cout << '*' << '\n' << '*';
        if(i != board_size - 1) {
            for(int j = 0; j < board_size; j++) {
                std::cout << "_____";
                if(j != board_size - 1)
                    std::cout << '|';
            }
            std::cout << '*' << '\n';
        }
    }
    for(int i = 0; i < board_size; i++) {
        std::cout << "     ";
        if(i != board_size - 1)
            std::cout << '|';
    }
    std::cout << '*' << '\n' << std::string(board_size*6 + 1, '*') << '\n';
}

char getTurn(int move_count) {
    return move_count % 2 == 0 ? 'X' : 'O';
}

int getRow(int pos, int board_size) {
    return std::floor(pos/board_size);
}

int getCol(int pos, int board_size) {
    return pos % board_size;
}

void getNeighbours(bool *neighbours, char *board, int pos, int board_size, char symbol) {
    int row = getRow(pos, board_size);
    int col =  getCol(pos, board_size);   
    int neighbours_pos[9] = {
        pos - board_size - 1,
        pos - board_size,
        pos - board_size + 1,
        pos - 1,
        pos,
        pos + 1,                           
        pos + board_size - 1,   
        pos + board_size,
        pos + board_size + 1
    };

    std::fill(neighbours, neighbours + 9, false);

    if(row == 0) {
        neighbours_pos[0] = -1;
        neighbours_pos[1] = -1;
        neighbours_pos[2] = -1;
    }
    if(row == board_size - 1) {
        neighbours_pos[6] = -1;
        neighbours_pos[7] = -1;
        neighbours_pos[8] = -1;
    }

    if(col == 0) {
        neighbours_pos[0] = -1;
        neighbours_pos[3] = -1;
        neighbours_pos[6] = -1;
    }
    if(col == board_size - 1) {
        neighbours_pos[2] = -1;
        neighbours_pos[5] = -1;
        neighbours_pos[8] = -1;
    }

    for(int i = 0; i < 9; i++) {
        if(neighbours_pos[i] == -1)
            continue;

        if(board[neighbours_pos[i]] == symbol)
            neighbours[i] = true;
    }
}

int countBackDiagonal(char *board, int chain_size, int board_size, char symbol, int pos, int direction_factor) {
    int current_chain_size = 0;
    int inc = (board_size - 1)*direction_factor;
    int row = getRow(pos, board_size) + 1;
    int col = getCol(pos, board_size) + 1;
    int steps = direction_factor == 1 ? std::min(board_size - row, col - 1) : std::min(row - 1, board_size - col);

    for(int i = 1; i <= steps; i++) {
        if(board[pos + i*inc] != symbol || current_chain_size >= chain_size)
            break;
        current_chain_size++;
    }
    return current_chain_size;
}

int countVertical(char *board, int chain_size, int board_size, char symbol, int pos, int direction_factor) {
    int current_chain_size = 0;
    int inc = board_size*direction_factor;
    int row = getRow(pos, board_size) + 1;
    int steps = direction_factor == 1 ? board_size - row : row - 1;

    for(int i = 1; i <= steps; i++) {
        if(board[pos + i*inc] != symbol || current_chain_size >= chain_size)
            break;
        current_chain_size++;
    }
    return current_chain_size;
}

int countForwardDiagonal(char *board, int chain_size, int board_size, char symbol, int pos, int direction_factor) {
    int current_chain_size = 0;
    int inc = (board_size + 1)*direction_factor;
    int row = getRow(pos, board_size) + 1;
    int col = getCol(pos, board_size) + 1;
    int steps = direction_factor == 1 ? std::min(board_size - row, board_size - col) : std::min(row - 1, col - 1);

    for(int i = 1; i <= steps; i++) {
        if(board[pos + i*inc] != symbol || current_chain_size >= chain_size)
            break;
        current_chain_size++;
    }
    return current_chain_size;
}

int countHorizontal(char *board, int chain_size, int board_size, char symbol, int pos, int direction_factor) {
    int current_chain_size = 0;
    int inc = direction_factor;
    int col = getCol(pos, board_size) + 1;
    int steps = direction_factor == 1 ? board_size - col : col - 1;

    for(int i = 1; i <= steps; i++) {
        if(board[pos + i*inc] != symbol || current_chain_size >= chain_size)
            break;
        current_chain_size++;
    }
    return current_chain_size;
}

bool checkWinner(char *board, int chain_size, int board_size, int pos, char symbol) {
    bool neighbours[9];
    int current_chain_size = 1;

    getNeighbours(neighbours, board, pos, board_size, symbol);
    if(neighbours[0])
        current_chain_size += countForwardDiagonal(board, chain_size - current_chain_size, board_size, symbol, pos, -1);
    if(current_chain_size >= chain_size)
        return true;

    if(neighbours[8])
        current_chain_size += countForwardDiagonal(board, chain_size - current_chain_size, board_size, symbol, pos, 1);
    if(current_chain_size >= chain_size)
        return true;

    current_chain_size = 1;

    if(neighbours[1])
        current_chain_size += countVertical(board, chain_size - current_chain_size, board_size, symbol, pos, -1);
    if(current_chain_size >= chain_size)
        return true;

    if(neighbours[7])
        current_chain_size += countVertical(board, chain_size - current_chain_size, board_size, symbol, pos, 1);
    if(current_chain_size >= chain_size)
        return true;

    current_chain_size = 1;

    if(neighbours[2])
        current_chain_size += countBackDiagonal(board, chain_size - current_chain_size, board_size, symbol, pos, -1);
    if(current_chain_size >= chain_size)
        return true;

    if(neighbours[6])
        current_chain_size += countBackDiagonal(board, chain_size - current_chain_size, board_size, symbol, pos, 1);
    if(current_chain_size >= chain_size)
        return true;

    current_chain_size = 1;

    if(neighbours[3])
        current_chain_size += countHorizontal(board, chain_size - current_chain_size, board_size, symbol, pos, -1);
    if(current_chain_size >= chain_size)
        return true;

    if(neighbours[5])
        current_chain_size += countHorizontal(board, chain_size - current_chain_size, board_size, symbol, pos, 1);
    if(current_chain_size >= chain_size)
        return true;
                
    return false;
}

int main() {
    const int BOARD_SIZE = 3;
    const int CHAIN_SIZE = 3;

    int board_elems = BOARD_SIZE*BOARD_SIZE;
    std::string get_input;
    int input_position;
    char board[board_elems];
    int move_count = 0;
    std::string mode = "pp";

    std::fill(board, board + board_elems, ' ');
    int seed = time(0);
    srand(seed);

    while(true) {
        drawBoard(board, BOARD_SIZE);

        do {
            if(mode[move_count % 2] == 'b') {
                input_position = (rand() % board_elems) + 1;
            }
            else {
                std::cout << getTurn(move_count) << "'s turn: ";
                std::getline(std::cin, get_input);
                input_position = parseInt(get_input);
            }
        } while(input_position < 1 || input_position > board_elems || board[input_position - 1] != ' ');
        input_position--;

        board[input_position] = getTurn(move_count);
        move_count++;

        if(checkWinner(board, CHAIN_SIZE, BOARD_SIZE, input_position, board[input_position])) {
            drawBoard(board, BOARD_SIZE);
            std::cout << board[input_position] << " Wins!";
            break;
        }
        
        if(move_count >= board_elems) {
            drawBoard(board, BOARD_SIZE);
            std::cout << "It's a tie!";
            break;
        }
    }
    return 0;
}