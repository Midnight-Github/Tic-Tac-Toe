class Board():
    def __init__(self, board_size):
        self.board_size = board_size
        self.board = [['.' for _ in range(board_size)] for _ in range(board_size)]
        
    def display(self):
        for row in self.board:
            print('|     '*self.board_size)
            print('| ', '  |  '.join(row), ' |')
            print('|     '*self.board_size)
            print('-' * (self.board_size * 5 - 1))