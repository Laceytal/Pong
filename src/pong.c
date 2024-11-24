// I WANT TO PLAY WITH YOU
//         YOUR FRIEND, AI

#include <stdio.h>

void clear_console();
void print_graph(int height, int rightWall, int leftWall, int aPos, int rocket_size, int rocketXOffset,
                 int bPos, int ballXPos, int ballYPos, int width, int aScore, int bScore);
char get_input(char move_top, char move_bot, char stay_char);
int calculate_rocket_pos(char move_top, char move_bot, char input, int cur_pos, int height, int rocket_size);
void play_game();

int main() {
    play_game();
    return 0;
}

// A and B are the players
void play_game() {
    const int WIDTH = 80;
    const int HEIGHT = 25;
    const int LEFT_WALL = 0;
    const int RIGHT_WALL = WIDTH + 1;
    const int ROCKET_X_OFFSET = 1;  // offset of rocket
    const int ENDGAME_POINTS = 21;
    const int ROCKET_SIZE = 3;

    const char MOVE_A_TOP = 'A';
    const char MOVE_A_BOT = 'Z';
    const char MOVE_B_TOP = 'K';
    const char MOVE_B_BOT = 'M';
    const char STAY_CHAR = ' ';

    char input;
    int a_pos, b_pos;
    a_pos = b_pos = HEIGHT / 2;
    int ball_x_pos = WIDTH / 2;
    int ball_y_pos = HEIGHT / 2;
    int a_score = 0, b_score = 0, iter = 0, vector_x = 1, vector_y = 1;

    for (; a_score < ENDGAME_POINTS && b_score < ENDGAME_POINTS;) {
        clear_console();
        // move ball
        if (iter % 2 == 0) {
            ball_x_pos += vector_x;
            ball_y_pos += vector_y;
            if (ball_x_pos <= ROCKET_X_OFFSET + 1) {
                if (ball_y_pos >= a_pos && ball_y_pos < a_pos + ROCKET_SIZE)
                    vector_x *= -1;
                else {  // B WON A POINT
                    ball_x_pos = WIDTH / 2;
                    ball_y_pos = HEIGHT / 2;
                    vector_x = 1;
                    vector_y = -1;
                    b_score++;
                }
            } else if (ball_x_pos >= RIGHT_WALL - ROCKET_X_OFFSET - 1) {
                if (ball_y_pos >= b_pos && ball_y_pos < b_pos + ROCKET_SIZE)
                    vector_x *= -1;
                else {
                    ball_x_pos = WIDTH / 2;
                    ball_y_pos = HEIGHT / 2;
                    vector_x = -1;
                    vector_y = -1;
                    a_score++;
                }
            }
            if (ball_y_pos <= 0 || ball_y_pos >= HEIGHT - 1) {
                if (ball_y_pos <= 0) ball_y_pos = 0;
                if (ball_y_pos >= HEIGHT - 1) ball_y_pos = HEIGHT - 1;
                vector_y *= -1;
                ball_y_pos += 2 * vector_y;
            }
        }
        printf("%d-%d | %d %d | %d %d\n", a_pos, b_pos, ball_x_pos, ball_y_pos, vector_x, vector_y);
        print_graph(HEIGHT, RIGHT_WALL, LEFT_WALL, a_pos, ROCKET_SIZE, ROCKET_X_OFFSET, b_pos, ball_x_pos,
                    ball_y_pos, WIDTH, a_score, b_score);

        if (iter % 2 == 0) {  // WAIT FOR INPUT A
            printf("please, input player A:");
            input = get_input(MOVE_A_TOP, MOVE_A_BOT, STAY_CHAR);
            a_pos = calculate_rocket_pos(MOVE_A_TOP, MOVE_A_BOT, input, a_pos, HEIGHT, ROCKET_SIZE);
            iter = 1;
        } else {  // WAIT FOR INPUT B
            printf("please, input player B:");
            input = get_input(MOVE_B_TOP, MOVE_B_BOT, STAY_CHAR);
            b_pos = calculate_rocket_pos(MOVE_B_TOP, MOVE_B_BOT, input, b_pos, HEIGHT, ROCKET_SIZE);
            iter = 0;
        }
    }
    clear_console();
    if (a_score == ENDGAME_POINTS) {
        printf("A won!");
    } else if (b_score == ENDGAME_POINTS) {
        printf("B won!");
    }
}

void print_graph(int height, int rightWall, int leftWall, int a_pos, int rocket_size, int rocketXOffset,
                 int b_pos, int ball_x_pos, int ball_y_pos, int width, int a_score, int b_score) {
    const char wall = '#';
    const char ball = '*';
    const char rocket = '|';
    const char middle = '.';
    for (int y = 0; y < height; y++) {
        for (int x = 0; x <= rightWall; x++) {
            if (x == leftWall || x == rightWall) {  // show side walls
                printf("%c", wall);
            } else if ((y >= a_pos && y < a_pos + rocket_size && x == rocketXOffset) ||
                       (y >= b_pos && y < b_pos + rocket_size &&
                        x == rightWall - rocketXOffset)) {  // show players
                printf("%c", rocket);
            } else if (x == ball_x_pos && y == ball_y_pos) {  // SHOW BALL
                printf("%c", ball);
            } else if (x == width / 2) {  // SHOW WALL
                printf("%c", middle);
            } else if (x == width / 2 - 3 && y == 2) {  // SHOW SCORE A
                printf("%02d", a_score);
            } else if (x == width / 2 - 2 && y == 2) {
                // skip
            } else if (x == width / 2 + 3 && y == 2) {  // SHOW SCORE B
                                                        // skip
            } else if (x == width / 2 + 2 && y == 2) {
                printf("%02d", b_score);
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

char get_input(char move_top, char move_bot, char stay_char) {
    char res;
    char inp;
    char endline;

    int diff = 'A' - 'a';

    while (1) {
        scanf("%c%c", &inp, &endline);
        if (inp == stay_char || inp == '\n') {
            res = stay_char;
            break;
        } else if (inp == move_top) {
            res = move_top;
            break;
        } else if (inp == move_top - diff) {
            res = move_top;
            break;
        } else if (inp == move_bot) {
            res = move_bot;
            break;
        } else if (inp == move_bot - diff) {
            res = move_bot;
            break;
        } else {
            printf("Wrong, use %c/%c or '%c' + \\n\n ", move_top, move_bot, stay_char);
        }
    }
    return res;
}

int calculate_rocket_pos(char move_top, char move_bot, char input, int cur_pos, int height, int rocket_size) {
    int new_pos = cur_pos;
    int speed = rocket_size / 2 + 1;
    if (cur_pos != 0 && input == move_top) {
        new_pos -= speed;
    } else if (input == move_bot && cur_pos != height - rocket_size) {
        new_pos += speed;
    }
    return new_pos;
}

void clear_console() { printf("\e[1;1H\e[2J"); }
