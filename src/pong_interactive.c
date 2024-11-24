
#include <ncurses.h>
#include <stdio.h>

void print_graph(int height, int right_wall, int left_wall, int aPos, int rocket_size, int rocket_x_offset,
                 int b_pos, int ball_x_pos, int ball_y_pos, int width, int a_score, int b_score);
char GetInput(char move_a_top, char move_a_bot, char move_b_top, char move_b_bot, char inp);
int calculate_rocket_pos(char move_top, char move_bot, char input, int cur_pos, int height, int rocket_size);
void play_game();

int main() {
    play_game();
    return 0;
}


void play_game() {
    initscr();
    cbreak();
    noecho();
    timeout(100);  

    const int WIDTH = 80;
    const int HEIGHT = 25;
    const int LEFT_WALL = 0;
    const int RIGHT_WALL = WIDTH + 1;
    const int ROCKET_X_OFFSET = 1;  
    const int ENDGAME_POINTS = 21;
    const int ROCKET_SIZE = 3;

    const char MOVE_A_TOP = 'A';
    const char MOVE_A_BOT = 'Z';
    const char MOVE_B_TOP = 'K';
    const char MOVE_B_BOT = 'M';

    int a_pos, b_pos;
    a_pos = b_pos = HEIGHT / 2;
    int ball_x_pos = WIDTH / 2;
    int ball_y_pos = HEIGHT / 2;
    int a_score = 0, b_score = 0, vector_x = 1, vector_y = 1;

    for (; a_score < ENDGAME_POINTS && b_score < ENDGAME_POINTS;) {
        char input;
        // move ball
        ball_x_pos += vector_x;
        ball_y_pos += vector_y;
        if ((ball_x_pos == ROCKET_X_OFFSET + 1 && ball_y_pos >= a_pos && ball_y_pos < a_pos + ROCKET_SIZE) ||
            (ball_x_pos == RIGHT_WALL - ROCKET_X_OFFSET - 1 && ball_y_pos >= b_pos &&
             ball_y_pos < b_pos + ROCKET_SIZE)) {
            vector_x *= -1;
        }
        if (ball_x_pos < ROCKET_X_OFFSET) {
            ball_x_pos = WIDTH / 2;
            ball_y_pos = HEIGHT / 2;
            vector_x = 1;
            b_score++;
        } else if (ball_x_pos > RIGHT_WALL - ROCKET_X_OFFSET - 1) {
            ball_x_pos = WIDTH / 2;
            ball_y_pos = HEIGHT / 2;
            vector_x = -1;
            a_score++;
        }
        if (ball_y_pos <= 0 || ball_y_pos >= HEIGHT - 1) {
            if (ball_y_pos <= 0) ball_y_pos = 0;
            if (ball_y_pos >= HEIGHT - 1) ball_y_pos = HEIGHT - 1;
            vector_y *= -1;
            ball_y_pos += 2 * vector_y;
        }
        print_graph(HEIGHT, RIGHT_WALL, LEFT_WALL, a_pos, ROCKET_SIZE, ROCKET_X_OFFSET, b_pos, ball_x_pos,
                    ball_y_pos, WIDTH, a_score, b_score);
        input = getch();
        if (input != ERR) {
            input = GetInput(MOVE_A_TOP, MOVE_A_BOT, MOVE_B_TOP, MOVE_B_BOT, input);
            a_pos = calculate_rocket_pos(MOVE_A_TOP, MOVE_A_BOT, input, a_pos, HEIGHT, ROCKET_SIZE);
            b_pos = calculate_rocket_pos(MOVE_B_TOP, MOVE_B_BOT, input, b_pos, HEIGHT, ROCKET_SIZE);
        }
    }
    endwin();
    clear();
    if (a_score == ENDGAME_POINTS) {
        printf("A won!");
    } else if (b_score == ENDGAME_POINTS) {
        printf("B won!");
    }
}

void print_graph(int height, int right_wall, int left_wall, int aPos, int rocket_size, int rocket_x_offset,
                 int b_pos, int ball_x_pos, int ball_y_pos, int width, int a_score, int b_score) {
    const char wall = '#';
    const char ball = '*';
    const char rocket = '|';
    const char middle = '.';
    clear();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x <= right_wall; x++) {
            if (x == left_wall || x == right_wall) {  // show side walls
                mvprintw(y, x, "%c", wall);
            } else if ((y >= aPos && y < aPos + rocket_size && x == rocket_x_offset) ||
                       (y >= b_pos && y < b_pos + rocket_size &&
                        x == right_wall - rocket_x_offset)) {  // show players
                mvprintw(y, x, "%c", rocket);
            } else if (x == ball_x_pos && y == ball_y_pos) {  // SHOW BALL
                mvprintw(y, x, "%c", ball);
            } else if (x == width / 2) {  // SHOW WALL
                mvprintw(y, x, "%c", middle);
            } else if (x == width / 2 - 3 && y == 2) {  // SHOW SCORE A
                mvprintw(y, x, "%02d", a_score);
            } else if (x == width / 2 - 2 && y == 2) {
                // skip
            } else if (x == width / 2 + 3 && y == 2) {  // SHOW SCORE B
                // skip
            } else if (x == width / 2 + 2 && y == 2) {
                mvprintw(y, x, "%02d", b_score);
            } else {
                mvprintw(y, x, " ");
            }
        }
        mvprintw(y, right_wall + 1, "\n");
    }
}

char GetInput(char move_a_top, char move_a_bot, char move_b_top, char move_b_bot, char inp) {
    char res = ' ';
    int diff = 'A' - 'a';

    if (inp == move_a_top || inp == move_a_top - diff) {
        res = move_a_top;
    } else if (inp == move_b_top || inp == move_b_top - diff) {
        res = move_b_top;
    } else if (inp == move_a_bot || inp == move_a_bot - diff) {
        res = move_a_bot;
    } else if (inp == move_b_bot || inp == move_b_bot - diff) {
        res = move_b_bot;
    }
    return res;
}

int calculate_rocket_pos(char move_top, char move_bot, char input, int cur_pos, int height, int rocket_size) {
    int newPos = cur_pos;
    int speed = rocket_size / 2 + 1;
    if (cur_pos != 0 && input == move_top) {
        newPos -= speed;
    } else if (input == move_bot && cur_pos != height - rocket_size) {
        newPos += speed;
    }
    return newPos;
}
