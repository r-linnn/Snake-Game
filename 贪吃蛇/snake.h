#ifndef __SNAKE_FULL_H__
#define __SNAKE_FULL_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

#define WIDE 60   // 宽度
#define HIGH 20   // 高度
#define MAX_USERS 100
#define MAX_NAME_LEN 20
#define MAX_PASS_LEN 20
#define DATA_FILE "snake_data.dat"

// 蛇身体的对象
struct BODY {
    int X;
    int Y;
};

// 蛇对象
struct SNAKE {
    struct BODY body[WIDE * HIGH];  // 蛇身
    int size;                       // 蛇的大小
} snake;

// 食物对象
struct FOOD {
    int X;
    int Y;
} food;

// 用户对象
struct USER {
    char username[MAX_NAME_LEN];
    char password[MAX_PASS_LEN];
    int high_score;
    time_t last_play_time;
};

// 用户系统
struct USER_SYSTEM {
    struct USER users[MAX_USERS];
    int user_count;
} user_system;

// 全局变量
int score = 0;          // 当前分数
int kx = 0;             // 移动方向X
int ky = 0;             // 移动方向Y
int sleepSecond = 300;  // 游戏速度
char current_user[MAX_NAME_LEN] = "";  // 当前登录用户

// 游戏功能函数
void initSnake(void);
void initFood(void);
void initUI(void);
void gotoxy(int x, int y);
void initWall(void);
void showScore(void);
void playGame(void);
void startGame(void);

// 账户和排名功能函数
void initUserSystem(void);
void clearInputBuffer(void);
int registerUser(void);
int loginUser(void);
void saveData(void);
void loadData(void);
void updateScore(void);
void showRanking(void);
void showMenu(void);

#endif
