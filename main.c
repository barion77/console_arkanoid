#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define screen_x 60
#define screen_y 30

#define width 52
#define height 20

char map[screen_y][screen_x];

int score = 0;
int max_score = 0;

typedef struct {
   int x;
   int y;
   int length;
} Platform;
Platform pl;

void init_platform()
{
    pl.x = (width - pl.length) / 2;
    pl.y = height-1;
    pl.length = 8;
}

void put_platform()
{
    for (int i = pl.x; i < pl.x + pl.length; i++)
        map[pl.y][i] = '=';
}

void move_platform(int x)
{
    pl.x = x;
    if (pl.x < 1)
        pl.x = 1;
    if (pl.x + pl.length >= width)
        pl.x = width - 1 - pl.length;
}

typedef struct {
    float x, y;
    int bx, by;
    float alfa;
    float speed;
} Ball;
Ball bl;

typedef struct {
    int x1;
    int x2;
    int x3;
    int x4;
    int x5;
    int x6;
    int x7;
    int x8;
    int x9;

    int y;

    int length;

    BOOL destroy1;
    BOOL destroy2;
    BOOL destroy3;
    BOOL destroy4;
    BOOL destroy5;
    BOOL destroy6;
    BOOL destroy7;
    BOOL destroy8;
    BOOL destroy9;
} Brick;
Brick br;

void move_ball(float x, float y);

void init_ball()
{
    move_ball(2, 2);
    bl.alfa = -1;
    bl.speed = 0.7;
}

void put_ball()
{
    map[bl.by][bl.bx] = '*';
}

void move_ball(float x, float y)
{
    bl.x = x;
    bl.y = y;
    bl.bx = (int)round(bl.x);
    bl.by = (int)round(bl.y);
}

void init_brick()
{
    br.length = 3;

    br.x1 = 3;
    br.x2 = 9;
    br.x3 = 14;
    br.x4 = 20;
    br.x5 = 25;
    br.x6 = 31;
    br.x7 = 35;
    br.x8 = 41;
    br.x9 = 47;

    br.y = 4;

    br.destroy1 = TRUE;
    br.destroy2 = TRUE;
    br.destroy3 = TRUE;
    br.destroy4 = TRUE;
    br.destroy5 = TRUE;
    br.destroy6 = TRUE;
    br.destroy7 = TRUE;
    br.destroy8 = TRUE;
    br.destroy9 = TRUE;
}

void put_brick(int x, BOOL destroy)
{
    if (destroy)
        for (int i = x; i < x + br.length; i++)
            map[br.y][i] = '@';
}

void hit_brick(int x, BOOL *destroy)
{
    for (int i = x; i < x+br.length; i++)
        if (i == bl.bx)
            if (bl.by-2 == br.y-2)
                *destroy = FALSE;
}

void ball_physics()
{
    if (bl.alfa < 0) bl.alfa += M_PI * 2;
    if (bl.alfa > M_PI * 2) bl.alfa -= M_PI * 2;

    int new_by = bl.by - 1;
    Ball old_bl = bl;
    move_ball(bl.x + cos(bl.alfa) * bl.speed, bl.y + sin(bl.alfa) * bl.speed);

    if ((map[bl.by][bl.bx] == '#') || (map[bl.by][old_bl.bx] == '=') || (map[bl.by][bl.bx] == '@'))
    {
        if ((map[bl.by][bl.bx] == '@'))
        {
            hit_brick(br.x1, &br.destroy1);
            hit_brick(br.x2, &br.destroy2);
            hit_brick(br.x3, &br.destroy3);
            hit_brick(br.x4, &br.destroy4);
            hit_brick(br.x5, &br.destroy5);
            hit_brick(br.x6, &br.destroy6);
            hit_brick(br.x7, &br.destroy7);
            hit_brick(br.x8, &br.destroy8);
            hit_brick(br.x9, &br.destroy9);

            score += 1;

        }

        if ((bl.bx != old_bl.bx) && (bl.by != old_bl.by))
        {
            if (map[old_bl.by][bl.bx] == map[bl.by][old_bl.bx])
                old_bl.alfa = old_bl.alfa + M_PI;
            else
            {
                if (map[old_bl.by][bl.bx] == '#')
                    old_bl.alfa = (2 * M_PI - old_bl.alfa) + M_PI;
                else
                    old_bl.alfa = (2 * M_PI - old_bl.alfa);

            }
        }
        else if (bl.by == old_bl.by)
            old_bl.alfa = (2 * M_PI - old_bl.alfa) + M_PI;
        else
            old_bl.alfa = (2 * M_PI - old_bl.alfa);

        bl = old_bl;
        ball_physics();
    }
}

void short_brick()
{

    put_brick(br.x1, br.destroy1);

    put_brick(br.x2, br.destroy2);

    put_brick(br.x3, br.destroy3);

    put_brick(br.x4, br.destroy4);

    put_brick(br.x5, br.destroy5);

    put_brick(br.x6, br.destroy6);

    put_brick(br.x7, br.destroy7);

    put_brick(br.x8, br.destroy8);

    put_brick(br.x9, br.destroy9);

}

void init_map()
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            map[i][j] = '#';
        }
    }
    for (int k = 1; k < width-1; k++)
        for (int l = 1; l < height; l++)
            map[l][k] = ' ';

}

void show()
{
    for (int i = 0; i < height; i++)
    {
        printf("%s", map[i]);

        if (i == 3)
            printf("   score: %d", score);

        if (i == 4)
            printf("   max score: %d", max_score);

        if (i == 10)
            printf("   start up arrow");

        if (i == 11)
            printf("   move <-- -->");

        if (i == 17)
            printf("   By Kipchak *_*");

        if (i < height-1)
            printf("\n");
    }
}


void set_cursor(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main()
{
    BOOL run = FALSE;
    int speed = 1;

    init_platform();
    init_ball();
    init_brick();
    do
    {
        set_cursor(0, 0);

        if (run)
            ball_physics();

        if (bl.by > height)
        {
            if (score > max_score)
            {
                max_score = score;
                score = 0;
            }
            score = 0;
            system("cls");
            run = FALSE;

            br.destroy1 = TRUE;
            br.destroy2 = TRUE;
            br.destroy3 = TRUE;
            br.destroy4 = TRUE;
            br.destroy5 = TRUE;
            br.destroy6 = TRUE;
            br.destroy7 = TRUE;
            br.destroy8 = TRUE;
            br.destroy9 = TRUE;
        }

        init_map();

        if (GetKeyState(VK_RIGHT) < 0)
            move_platform(pl.x + speed);

        if (GetKeyState(VK_LEFT) < 0)
            move_platform(pl.x - speed);

        if (GetKeyState(VK_UP) < 0) run = TRUE;

        put_platform();
        put_ball();

        if (!run)
            move_ball(pl.x + pl.length / 2, pl.y - 1);

        short_brick();
        if (br.destroy1 == FALSE && br.destroy2 == FALSE && br.destroy3 == FALSE && br.destroy4 == FALSE &&
            br.destroy5 == FALSE && br.destroy6 == FALSE && br.destroy7 == FALSE && br.destroy8 == FALSE && br.destroy9 == FALSE)
        {
            br.destroy1 = TRUE;
            br.destroy2 = TRUE;
            br.destroy3 = TRUE;
            br.destroy4 = TRUE;
            br.destroy5 = TRUE;
            br.destroy6 = TRUE;
            br.destroy7 = TRUE;
            br.destroy8 = TRUE;
            br.destroy9 = TRUE;

            short_brick();
        }

        show();
        Sleep(10);

    }
    while(GetKeyState(VK_ESCAPE) >= 0);

    return 0;
}
