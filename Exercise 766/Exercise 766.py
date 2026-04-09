import timeit

field = [[0, 2, 2, 3, 2, 2],
         [0, 2, 3, 3, 2, 4],
         [1, 1, 6, 6, 4, 4],
         [1, 1, 6, 6, 4, 3],
         [1, 1, 5, 5, 3, 3]]

configs = set()
HEIGHT = len(field)
WIDTH = len(field[0])
EMPTY = 0
PURPLE = 1
RED = 2
GREEN = 3
YELLOW = 4
CYAN = 5
BLUE = 6

to_do = [field]


def config_to_int(c):
    return int("".join([str(n) for row in c for n in row]))


def find_empty_fields(c):
    empty_fields = []
    for row in range(len(c)):
        for col in range(len(c[0])):
            if c[row][col] == EMPTY:
                empty_fields.append([row, col])
    return empty_fields


def find_red_piece(c, p):
    if p[0] < HEIGHT - 1 and p[1] < WIDTH - 1:
        if c[p[0]][p[1] + 1] == RED and c[p[0] + 1][p[1]] == RED:
            # p is the top-left block
            return p
    if p[0] < HEIGHT - 1 and p[1] > 0:
        if c[p[0]][p[1] - 1] == RED and c[p[0] + 1][p[1] - 1] == RED:
            # p is the top-right block
            return [p[0], p[1] - 1]
    if p[0] > 0 and p[1] < WIDTH - 1:
        if c[p[0] - 1][p[1]] == RED and c[p[0] - 1][p[1] + 1] == RED:
            # p is the bottom-left block
            return [p[0] - 1, p[1]]


def find_green_piece(c, p):
    # For the green piece, we return the empty block top-left
    if p[0] < HEIGHT - 1 and p[1] > 0:
        if c[p[0] + 1][p[1] - 1] == GREEN and c[p[0] + 1][p[1]] == GREEN:
            # p is the top-right block
            return [p[0], p[1] - 1]
    if p[0] > 0 and p[1] < WIDTH - 1:
        if c[p[0] - 1][p[1] + 1] == GREEN and c[p[0]][p[1] + 1] == GREEN:
            # p is the bottom-left block
            return [p[0] - 1, p[1]]
    if p[0] > 0 and p[1] > 0:
        if c[p[0] - 1][p[1]] == GREEN and c[p[0]][p[1] - 1] == GREEN:
            # p is the bottom-right block
            return [p[0] - 1, p[1] - 1]


def find_yellow_piece(c, p):
    if p[0] == 0:
        # p is the top block
        return p
    if p[0] == HEIGHT - 1:
        # p is the bottom block
        return [p[0] - 1, p[1]]
    if c[p[0] - 1][p[1]] != YELLOW:
        # p is the top block
        return p
    if c[p[0] + 1][p[1]] != YELLOW:
        # p is the bottom block
        return [p[0] - 1, p[1]]
    # Blocks form a line
    if c[0][p[1]] == YELLOW:
        return [2 * (p[0] // 2), p[1]]
    else:
        return [2 * (p[0] // 2) + 1, p[1]]


def find_unique_piece(c, color):
    for row in range(len(c)):
        for col in range(len(c[0])):
            if c[row][col] == color:
                return [row, col]


def perform_moves(c):
    # Check if we already covered that configuration
    prev_len = len(configs)
    config_int = config_to_int(c)
    configs.add(config_int)
    if prev_len == len(configs):
        return

    empty_fields = find_empty_fields(c)

    for e in empty_fields:

        # Move piece down
        if e[0] > 0:
            c_copy = [row[:] for row in c]
            piece = c_copy[e[0] - 1][e[1]]
            if piece == PURPLE:
                c_copy[e[0]][e[1]] = PURPLE
                c_copy[e[0] - 1][e[1]] = EMPTY
                to_do.append(c_copy)
            elif piece == RED:
                red = find_red_piece(c, [e[0] - 1, e[1]])
                if ([red[0] + 1, red[1] + 1] == empty_fields[0]) and ([red[0] + 2, red[1]] == empty_fields[1]):
                    c_copy[red[0]][red[1]] = EMPTY
                    c_copy[red[0]][red[1] + 1] = EMPTY
                    c_copy[red[0] + 2][red[1]] = RED
                    c_copy[red[0] + 1][red[1] + 1] = RED
                    to_do.append(c_copy)
            elif piece == GREEN:
                green = find_green_piece(c, [e[0] - 1, e[1]])
                if ([green[0] + 2, green[1]] == empty_fields[0]) and ([green[0] + 2, green[1] + 1] == empty_fields[1]):
                    c_copy[green[0]][green[1] + 1] = EMPTY
                    c_copy[green[0] + 1][green[1]] = EMPTY
                    c_copy[green[0] + 2][green[1]] = GREEN
                    c_copy[green[0] + 2][green[1] + 1] = GREEN
                    to_do.append(c_copy)
            elif piece == YELLOW:
                yellow = find_yellow_piece(c, [e[0] - 1, e[1]])
                c_copy[yellow[0]][yellow[1]] = EMPTY
                c_copy[yellow[0] + 2][yellow[1]] = YELLOW
                to_do.append(c_copy)
            elif piece == CYAN:
                cyan = find_unique_piece(c, CYAN)
                if ([cyan[0] + 1, cyan[1]] == empty_fields[0]) and ([cyan[0] + 1, cyan[1] + 1] == empty_fields[1]):
                    c_copy[cyan[0]][cyan[1]] = EMPTY
                    c_copy[cyan[0]][cyan[1] + 1] = EMPTY
                    c_copy[cyan[0] + 1][cyan[1]] = CYAN
                    c_copy[cyan[0] + 1][cyan[1] + 1] = CYAN
                    to_do.append(c_copy)
            elif piece == BLUE:
                blue = find_unique_piece(c, BLUE)
                if ([blue[0] + 2, blue[1]] == empty_fields[0]) and ([blue[0] + 2, blue[1] + 1] == empty_fields[1]):
                    c_copy[blue[0]][blue[1]] = EMPTY
                    c_copy[blue[0]][blue[1] + 1] = EMPTY
                    c_copy[blue[0] + 2][blue[1]] = BLUE
                    c_copy[blue[0] + 2][blue[1] + 1] = BLUE
                    to_do.append(c_copy)

        # Move piece up
        if e[0] < HEIGHT - 1:
            c_copy = [row[:] for row in c]
            piece = c_copy[e[0] + 1][e[1]]
            if piece == PURPLE:
                c_copy[e[0]][e[1]] = PURPLE
                c_copy[e[0] + 1][e[1]] = EMPTY
                to_do.append(c_copy)
            elif piece == RED:
                red = find_red_piece(c, [e[0] + 1, e[1]])
                if ([red[0] - 1, red[1]] == empty_fields[0]) and ([red[0] - 1, red[1] + 1] == empty_fields[1]):
                    c_copy[red[0] + 1][red[1]] = EMPTY
                    c_copy[red[0]][red[1] + 1] = EMPTY
                    c_copy[red[0] - 1][red[1]] = RED
                    c_copy[red[0] - 1][red[1] + 1] = RED
                    to_do.append(c_copy)
            elif piece == GREEN:
                green = find_green_piece(c, [e[0] + 1, e[1]])
                if ([green[0], green[1]] == empty_fields[1]) and ([green[0] - 1, green[1] + 1] == empty_fields[0]):
                    c_copy[green[0] + 1][green[1]] = EMPTY
                    c_copy[green[0] + 1][green[1] + 1] = EMPTY
                    c_copy[green[0]][green[1]] = GREEN
                    c_copy[green[0] - 1][green[1] + 1] = GREEN
                    to_do.append(c_copy)
            elif piece == YELLOW:
                yellow = find_yellow_piece(c, [e[0] + 1, e[1]])
                c_copy[yellow[0] + 1][yellow[1]] = EMPTY
                c_copy[yellow[0] - 1][yellow[1]] = YELLOW
                to_do.append(c_copy)
            elif piece == CYAN:
                cyan = find_unique_piece(c, CYAN)
                if ([cyan[0] - 1, cyan[1]] == empty_fields[0]) and ([cyan[0] - 1, cyan[1] + 1] == empty_fields[1]):
                    c_copy[cyan[0]][cyan[1]] = EMPTY
                    c_copy[cyan[0]][cyan[1] + 1] = EMPTY
                    c_copy[cyan[0] - 1][cyan[1]] = CYAN
                    c_copy[cyan[0] - 1][cyan[1] + 1] = CYAN
                    to_do.append(c_copy)
            elif piece == BLUE:
                blue = find_unique_piece(c, BLUE)
                if ([blue[0] - 1, blue[1]] == empty_fields[0]) and ([blue[0] - 1, blue[1] + 1] == empty_fields[1]):
                    c_copy[blue[0] + 1][blue[1]] = EMPTY
                    c_copy[blue[0] + 1][blue[1] + 1] = EMPTY
                    c_copy[blue[0] - 1][blue[1]] = BLUE
                    c_copy[blue[0] - 1][blue[1] + 1] = BLUE
                    to_do.append(c_copy)

        # Move piece right
        if e[1] > 0:
            c_copy = [row[:] for row in c]
            piece = c_copy[e[0]][e[1] - 1]
            if piece == PURPLE:
                c_copy[e[0]][e[1]] = PURPLE
                c_copy[e[0]][e[1] - 1] = EMPTY
                to_do.append(c_copy)
            elif piece == RED:
                red = find_red_piece(c, [e[0], e[1] - 1])
                if ([red[0], red[1] + 2] == empty_fields[0]) and ([red[0] + 1, red[1] + 1] == empty_fields[1]):
                    c_copy[red[0]][red[1]] = EMPTY
                    c_copy[red[0] + 1][red[1]] = EMPTY
                    c_copy[red[0]][red[1] + 2] = RED
                    c_copy[red[0] + 1][red[1] + 1] = RED
                    to_do.append(c_copy)
            elif piece == GREEN:
                green = find_green_piece(c, [e[0], e[1] - 1])
                if ([green[0], green[1] + 2] == empty_fields[0]) and ([green[0] + 1, green[1] + 2] == empty_fields[1]):
                    c_copy[green[0]][green[1] + 1] = EMPTY
                    c_copy[green[0] + 1][green[1]] = EMPTY
                    c_copy[green[0]][green[1] + 2] = GREEN
                    c_copy[green[0] + 1][green[1] + 2] = GREEN
                    to_do.append(c_copy)
            elif piece == YELLOW:
                yellow = find_yellow_piece(c, [e[0], e[1] - 1])
                if ([yellow[0], yellow[1] + 1] == empty_fields[0]) and (
                        [yellow[0] + 1, yellow[1] + 1] == empty_fields[1]):
                    c_copy[yellow[0]][yellow[1]] = EMPTY
                    c_copy[yellow[0] + 1][yellow[1]] = EMPTY
                    c_copy[yellow[0]][yellow[1] + 1] = YELLOW
                    c_copy[yellow[0] + 1][yellow[1] + 1] = YELLOW
                    to_do.append(c_copy)
            elif piece == CYAN:
                cyan = find_unique_piece(c, CYAN)
                c_copy[cyan[0]][cyan[1]] = EMPTY
                c_copy[cyan[0]][cyan[1] + 2] = CYAN
                to_do.append(c_copy)
            elif piece == BLUE:
                blue = find_unique_piece(c, BLUE)
                if ([blue[0], blue[1] + 2] == empty_fields[0]) and ([blue[0] + 1, blue[1] + 2] == empty_fields[1]):
                    c_copy[blue[0]][blue[1]] = EMPTY
                    c_copy[blue[0] + 1][blue[1]] = EMPTY
                    c_copy[blue[0]][blue[1] + 2] = BLUE
                    c_copy[blue[0] + 1][blue[1] + 2] = BLUE
                    to_do.append(c_copy)

        # Movie piece left
        if e[1] < WIDTH - 1:
            c_copy = [row[:] for row in c]
            piece = c_copy[e[0]][e[1] + 1]
            if piece == PURPLE:
                c_copy[e[0]][e[1]] = PURPLE
                c_copy[e[0]][e[1] + 1] = EMPTY
                to_do.append(c_copy)
            elif piece == RED:
                red = find_red_piece(c, [e[0], e[1] + 1])
                if ([red[0], red[1] - 1] == empty_fields[0]) and ([red[0] + 1, red[1] - 1] == empty_fields[1]):
                    c_copy[red[0]][red[1] + 1] = EMPTY
                    c_copy[red[0] + 1][red[1]] = EMPTY
                    c_copy[red[0]][red[1] - 1] = RED
                    c_copy[red[0] + 1][red[1] - 1] = RED
                    to_do.append(c_copy)
            elif piece == GREEN:
                green = find_green_piece(c, [e[0], e[1] + 1])
                if ([green[0], green[1]] == empty_fields[0]) and ([green[0] + 1, green[1] - 1] == empty_fields[1]):
                    c_copy[green[0]][green[1] + 1] = EMPTY
                    c_copy[green[0] + 1][green[1] + 1] = EMPTY
                    c_copy[green[0]][green[1]] = GREEN
                    c_copy[green[0] + 1][green[1] - 1] = GREEN
                    to_do.append(c_copy)
            elif piece == YELLOW:
                yellow = find_yellow_piece(c, [e[0], e[1] + 1])
                if ([yellow[0], yellow[1] - 1] == empty_fields[0]) and (
                        [yellow[0] + 1, yellow[1] - 1] == empty_fields[1]):
                    c_copy[yellow[0]][yellow[1]] = EMPTY
                    c_copy[yellow[0] + 1][yellow[1]] = EMPTY
                    c_copy[yellow[0]][yellow[1] - 1] = YELLOW
                    c_copy[yellow[0] + 1][yellow[1] - 1] = YELLOW
                    to_do.append(c_copy)
            elif piece == CYAN:
                cyan = find_unique_piece(c, CYAN)
                c_copy[cyan[0]][cyan[1] + 1] = EMPTY
                c_copy[cyan[0]][cyan[1] - 1] = CYAN
                to_do.append(c_copy)
            elif piece == BLUE:
                blue = find_unique_piece(c, BLUE)
                if ([blue[0], blue[1] - 1] == empty_fields[0]) and ([blue[0] + 1, blue[1] - 1] == empty_fields[1]):
                    c_copy[blue[0]][blue[1] + 1] = EMPTY
                    c_copy[blue[0] + 1][blue[1] + 1] = EMPTY
                    c_copy[blue[0]][blue[1] - 1] = BLUE
                    c_copy[blue[0] + 1][blue[1] - 1] = BLUE
                    to_do.append(c_copy)


start_time = timeit.default_timer()
while len(to_do) > 0:
    perform_moves(to_do.pop())
end_time = timeit.default_timer()

print(len(configs))
print('Time: ', end_time - start_time)