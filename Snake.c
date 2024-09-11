#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define WIDTH 70
#define HEIGHT 30

int length, bend_no, len, life;
char key;
void Delay(long double);
void Move();
void Food();
int Score();
void Print();
void gotoxy(int x, int y);
void GotoXY(int x, int y);
void Bend();
void Boarder();
void Down();
void Left();
void Up();
void Right();
void ExitGame();
int Scoreonly();

struct coordinate {
    int x;
    int y;
    int direction;
};

typedef struct coordinate coordinate;

coordinate head, bend[500], food, body[30];

int main() {
    Print();
    system("cls");
    system("color 3f");
    length = 5;
    head.x = WIDTH / 2;
    head.y = HEIGHT / 2;
    head.direction = RIGHT;
    Boarder();
    Food();
    life = 3;
    bend[0] = head;
    Move();
    return 0;
}

void Move() {
    int a, i;
    do {
        Food();
        fflush(stdin);
        len = 0;
        for (i = 0; i < 30; i++) {
            body[i].x = 0;
            body[i].y = 0;
            if (i == length)
                break;
        }
        Delay(200000);
        Boarder();
        switch (head.direction) {
            case RIGHT: Right(); break;
            case LEFT: Left(); break;
            case DOWN: Down(); break;
            case UP: Up(); break;
        }
        ExitGame();
    } while (!kbhit());

    a = getch();
    if (a == 27) {
        system("cls");
        exit(0);
    }
    key = getch();

    if ((key == RIGHT && head.direction != LEFT) ||
        (key == LEFT && head.direction != RIGHT) ||
        (key == UP && head.direction != DOWN) ||
        (key == DOWN && head.direction != UP)) {
        bend_no++;
        bend[bend_no] = head;
        head.direction = key;
        switch (key) {
            case UP: head.y--; break;
            case DOWN: head.y++; break;
            case RIGHT: head.x++; break;
            case LEFT: head.x--; break;
        }
        Move();
    } else if (key == 27) {
        system("cls");
        exit(0);
    } else {
        printf("\a");
        Move();
    }
}


void gotoxy(int x, int y) {
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void GotoXY(int x, int y) {
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD b = {x, y};
    SetConsoleCursorPosition(a, b);
}

void Up() {
    int i;
    for (i = 0; i <= (bend[bend_no].y - head.y) && len < length; i++) {
        GotoXY(head.x, head.y + i);
        printf(len == 0 ? "^" : "*");
        body[len].x = head.x;
        body[len].y = head.y + i;
        len++;
    }
    Bend();
    if (!kbhit()) head.y--;
}

int Score() {
    static int score = 0;
    // Increase the score based on game logic
    if (head.x == food.x && head.y == food.y) {
        score += 10;  // Example score increment
    }
    return score;
}

void Down() {
    int i;
    for (i = 0; i <= (head.y - bend[bend_no].y) && len < length; i++) {
        GotoXY(head.x, head.y + i);
        printf(len == 0 ? "v" : "*");
        body[len].x = head.x;
        body[len].y = head.y + i;
        len++;
    }
    Bend();
    if (!kbhit()) head.y++;
}

void Delay(long double k) {
    Score();
    long double i;
    for (i = 0; i <= k; i++);
}

void ExitGame() {
    int i, check = 0;
    for (i = 4; i < length; i++) {
        if (body[0].x == body[i].x && body[0].y == body[i].y) {
            check++;
        }
        if (i == length || check != 0) break;
    }
    if (head.x <= 10 || head.x >= WIDTH || head.y <= 10 || head.y >= HEIGHT || check != 0) {
        life--;
        if (life >= 0) {
            head.x = WIDTH / 2;
            head.y = HEIGHT / 2;
            bend_no = 0;
            head.direction = RIGHT;
            Move();
        } else {
            system("cls");
            printf("All Lives Completed\nBetter Luck Next Time!!!\nPress Any Key To Quit The Game\n");
            exit(0);
        }
    }
}

void Food() {
    if (head.x == food.x && head.y == food.y) {
        length++;
        time_t a;
        a = time(0);
        srand(a);
        food.x = rand() % (WIDTH - 10) + 11;
        food.y = rand() % (HEIGHT - 10) + 11;
    } else if (food.x == 0) {
        food.x = rand() % (WIDTH - 10) + 11;
        food.y = rand() % (HEIGHT - 10) + 11;
    }
}

void Left() {
    int i;
    for (i = 0; i <= (bend[bend_no].x - head.x) && len < length; i++) {
        GotoXY(head.x + i, head.y);
        printf(len == 0 ? "<" : "*");
        body[len].x = head.x + i;
        body[len].y = head.y;
        len++;
    }
    Bend();
    if (!kbhit()) head.x--;
}

void Right() {
    int i;
    for (i = 0; i <= (head.x - bend[bend_no].x) && len < length; i++) {
        GotoXY(head.x - i, head.y);
        body[len].x = head.x - i;
        body[len].y = head.y;
        GotoXY(body[len].x, body[len].y);
        printf(len == 0 ? ">" : "*");
        len++;
    }
    Bend();
    if (!kbhit()) head.x++;
}

void Bend() {
    int i, j, diff;
    for (i = bend_no; i >= 0 && len < length; i--) {
        if (bend[i].x == bend[i - 1].x) {
            diff = bend[i].y - bend[i - 1].y;
            if (diff < 0) {
                for (j = 1; j <= (-diff) && len < length; j++) {
                    body[len].x = bend[i].x;
                    body[len].y = bend[i].y + j;
                    GotoXY(body[len].x, body[len].y);
                    printf("*");
                    len++;
                }
            } else if (diff > 0) {
                for (j = 1; j <= diff && len < length; j++) {
                    body[len].x = bend[i].x;
                    body[len].y = bend[i].y - j;
                    GotoXY(body[len].x, body[len].y);
                    printf("*");
                    len++;
                }
            }
        } else if (bend[i].y == bend[i - 1].y) {
            diff = bend[i].x - bend[i - 1].x;
            if (diff < 0) {
                for (j = 1; j <= (-diff) && len < length; j++) {
                    body[len].x = bend[i].x + j;
                    body[len].y = bend[i].y;
                    GotoXY(body[len].x, body[len].y);
                    printf("*");
                    len++;
                }
            } else if (diff > 0) {
                for (j = 1; j <= diff && len < length; j++) {
                    body[len].x = bend[i].x - j;
                    body[len].y = bend[i].y;
                    GotoXY(body[len].x, body[len].y);
                    printf("*");
                    len++;
                }
            }
        }
    }
}

void Boarder() {
    system("cls");
    int i;
    GotoXY(food.x, food.y);
    printf("F");
    for (i = 10; i < WIDTH; i++) {
        GotoXY(i, 10);
        printf("!");
        GotoXY(i, HEIGHT);
        printf("!");
    }
    for (i = 10; i < HEIGHT; i++) {
        GotoXY(10, i);
        printf("!");
        GotoXY(WIDTH, i);
        printf("!");
    }
}

void Print() {
    printf("\tWelcome To The Snake Game.\n");
    printf("\tGame is developed by [Yash Sinha]\n\n");
    printf("\tInstructions: \n");
    printf("\t1. Press Arrow keys to move.\n");
    printf("\t2. Collect food represented as 'F'.\n");
    printf("\t3. Avoid hitting walls or yourself.\n");
    printf("\t4. Your lives: 3.\n");
    printf("\t5. Press 'Esc' to exit the game.\n\n");
    printf("\tPress any key to start...\n");
    getch();
}
