#include "snake.h"

// ==================== 基础游戏函数 ====================

// 定义光标位置
void gotoxy(int x, int y) 
{
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 画墙
void initWall(void)
   {
    // 绘制右边界
    for (int y = 0; y <= HIGH; y++) {
        gotoxy(WIDE, y);
        printf("|");
    }

    // 绘制下边界
    gotoxy(0, HIGH);
    for (int x = 0; x <= WIDE; x++)
    {
        printf("-");
    }
    printf("\n");
}

// 定义初始化蛇函数
void initSnake(void)
{
    snake.size = 2;  // 初始化长度
    snake.body[0].X = WIDE / 2;
    snake.body[0].Y = HIGH / 2;     // 初始化蛇头
    snake.body[1].X = WIDE / 2 - 1;
    snake.body[1].Y = HIGH / 2;     // 初始化一节蛇身
}

// 定义初始化食物函数
void initFood(void)
{
    food.X = rand() % WIDE;  // 0-59
    food.Y = rand() % HIGH;  // 0-59
}

// 初始化界面控件
void initUI(void)
{
    COORD coord = { 0 };  // 光标移动的位置

    // 清空游戏区域
    for (int y = 0; y < HIGH; y++)
    {
        gotoxy(0, y);
        for (int x = 0; x < WIDE; x++)
        {
            putchar(' ');  // 填充空格
        }
    }

    // 画蛇
    for (int i = 0; i < snake.size; i++)
    {
        coord.X = snake.body[i].X;
        coord.Y = snake.body[i].Y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        if (i == 0)
            putchar('@');  // 蛇头
        else
            putchar('*');  // 蛇身
    }

    // 画食物
    coord.X = food.X;
    coord.Y = food.Y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    putchar('#');

    // 将光标默认位置移动至不干扰游戏的位置
    coord.X = 0;
    coord.Y = HIGH + 2;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 打印分数
void showScore(void)
{
    printf("\nGame Over!!!\n");
    printf("本次得分: %d\n", score);
}

// 游戏主逻辑
void playGame(void)
{
    char key = 'd';  // 默认蛇向右移动

    // 隐藏光标，减少闪烁
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);

    // 判断蛇撞墙
    while (snake.body[0].X >= 0 && snake.body[0].X < WIDE &&
        snake.body[0].Y >= 0 && snake.body[0].Y < HIGH)
        {
        // 更新蛇
        initUI();

        // 接受用户按键输入 asdw
        if (_kbhit())
        {  // 为真时，说明用户按下按键
            key = _getch();
        }
        switch (key)
        {
        case 'w': kx = 0; ky = -1; break;
        case 's': kx = 0; ky = +1; break;
        case 'd': kx = +1; ky = 0; break;
        case 'a': kx = -1; ky = 0; break;
        default: break;
        }

        // 蛇撞身体：蛇头==任意一节身体
        for (int i = 1; i < snake.size; i++)
        {
            if (snake.body[0].X == snake.body[i].X &&
                snake.body[0].Y == snake.body[i].Y)
            {
                return;  // 游戏结束
            }
        }

        // 蛇头撞食物
        if (snake.body[0].X == food.X && snake.body[0].Y == food.Y)
        {
            initFood();      // 食物消失（初始化一个新食物）
            snake.size++;    // 身体增长
            score++;         // 加分
            if (sleepSecond > 50)
            {
                sleepSecond -= 10;  // 加速，但有限制
            }
        }

        // 蛇移动，前一节身体给后一节身体赋值
        for (int i = snake.size - 1; i > 0; i--)
        {
            snake.body[i].X = snake.body[i - 1].X;
            snake.body[i].Y = snake.body[i - 1].Y;
        }
        snake.body[0].X += kx;  // 蛇头坐标根据用户按键进行修改
        snake.body[0].Y += ky;

        Sleep(sleepSecond);  // 增加延时
    }
}

// 启动游戏
void startGame(void)
{
    system("cls");
    initSnake();   // 初始化蛇
    initFood();    // 初始化食物
    initUI();      // 画蛇和食物
    initWall();    // 画墙
    playGame();    // 启动游戏
}

// ==================== 账户和排名功能 ====================

// 清除输入缓冲区
void clearInputBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 初始化用户系统
void initUserSystem(void)
{
    user_system.user_count = 0;
    loadData();
}

// 保存数据到文件
void saveData(void)
{
    FILE* file = NULL;
    errno_t err = fopen_s(&file, "贪吃蛇account", "wb");
    if (err != 0 || file == NULL)
    {
        printf("无法保存数据！\n");
        return;
    }

    fwrite(&(user_system.user_count), sizeof(int), 1, file);
    fwrite(user_system.users, sizeof(struct USER), user_system.user_count, file);

    fclose(file);
}

// 从文件加载数据
void loadData(void)
    {
    FILE* file = NULL;
    errno_t err = fopen_s(&file, "贪吃蛇account", "rb");
    if (err != 0 || file == NULL)
    {
        printf("首次运行，创建数据文件...\n");
        user_system.user_count = 0;
        return;
    }

    fread(&(user_system.user_count), sizeof(int), 1, file);
    fread(user_system.users, sizeof(struct USER), user_system.user_count, file);

    fclose(file);
}

// 注册新用户
int registerUser(void)
   {
    if (user_system.user_count >= MAX_USERS)
    {
        printf("用户数量已达上限！\n");
        return 0;
    }

    char username[MAX_NAME_LEN];
    char password[MAX_PASS_LEN];
    char confirm_pass[MAX_PASS_LEN];

    printf("\n=== 用户注册 ===\n");

    // 输入用户名
    while (1)
    {
        printf("请输入用户名（%d个字符以内）：", MAX_NAME_LEN - 1);
        fgets(username,MAX_NAME_LEN-1,stdin);
        clearInputBuffer();

        // 检查用户名是否已存在
        int exists = 0;
        for (int i = 0; i < user_system.user_count; i++)
        {
            if (strcmp(user_system.users[i].username, username) == 0)
            {
                printf("用户名已存在，请重新输入！\n");
                exists = 1;
                break;
            }
        }

        if (!exists)
        {
            if (strlen(username) < 1)
            {
                printf("用户名不能为空！\n");
                continue;
            }
            break;
        }
    }

    // 输入密码
    while (1)
    {
        printf("请输入密码（%d个字符以内）：", MAX_PASS_LEN - 1);
        fgets(password,MAX_PASS_LEN-1,stdin);
        clearInputBuffer();

        if (strlen(password) < 4)
        {
            printf("密码长度至少4位！\n");
            continue;
        }

        printf("请确认密码：");
        fgets(confirm_pass,MAX_PASS_LEN-1,stdin);
        clearInputBuffer();

        if (strcmp(password, confirm_pass) != 0)
        {
            printf("两次输入的密码不一致！\n");
        }
        else
        {
            break;
        }
    }

    // 保存用户信息
    strcpy_s(user_system.users[user_system.user_count].username, username);
    strcpy_s(user_system.users[user_system.user_count].password, password);
    user_system.users[user_system.user_count].high_score = 0;
    user_system.users[user_system.user_count].last_play_time = time(NULL);
    user_system.user_count++;

    saveData();

    printf("注册成功！欢迎 %s！\n", username);
    return 1;
}

// 用户登录
int loginUser(void)
{
    char username[MAX_NAME_LEN];
    char password[MAX_PASS_LEN];

    printf("\n=== 用户登录 ===\n");

    int attempts = 3;
    while (attempts > 0) {
        printf("请输入用户名：");
        fgets(username,MAX_NAME_LEN,stdin);
        clearInputBuffer();

        printf("请输入密码：");
        fgets(password,MAX_PASS_LEN,stdin);
        clearInputBuffer();

        // 查找用户
        for (int i = 0; i < user_system.user_count; i++)
        {
            if (strcmp(user_system.users[i].username, username) == 0)
            {
                if (strcmp(user_system.users[i].password, password) == 0)
                {
                    strcpy_s(current_user, username);
                    printf("登录成功！欢迎回来，%s！\n", username);
                    return 1;
                }
                else
                {
                    printf("密码错误！\n");
                    attempts--;
                    printf("剩余尝试次数：%d\n", attempts);
                    break;
                }
            }
        }

        // 如果没找到用户
        int found = 0;
        for (int i = 0; i < user_system.user_count; i++)
        {
            if (strcmp(user_system.users[i].username, username) == 0)
            {
                found = 1;
                break;
            }
        }

        if (!found)
        {
            printf("用户不存在！\n");
            attempts--;
            printf("剩余尝试次数：%d\n", attempts);
        }
    }

    if (attempts == 0)
    {
        printf("登录失败！\n");
    }

    return 0;
}

// 更新用户分数
void updateScore(void)
{
    for (int i = 0; i < user_system.user_count; i++)
    {
        if (strcmp(user_system.users[i].username, current_user) == 0)
        {
            if (score > user_system.users[i].high_score)
            {
                user_system.users[i].high_score = score;
                printf("恭喜！创造了新的最高记录：%d分！\n", score);
            }
            else
            {
                printf("本次得分：%d，最高记录：%d\n",
                    score, user_system.users[i].high_score);
            }
            user_system.users[i].last_play_time = time(NULL);
            saveData();
            return;
        }
    }
}

// 显示排行榜
void showRanking(void)
{
    printf("\n=== 贪吃蛇排行榜 ===\n");
    printf("排名\t用户名\t\t最高分\t\t最后游戏时间\n");
    printf("------------------------------------------------\n");

    if (user_system.user_count == 0)
    {
        printf("暂无数据！\n");
        return;
    }

    // 创建临时数组用于排序
    struct USER temp_users[MAX_USERS];
    memcpy(temp_users, user_system.users, sizeof(struct USER) * user_system.user_count);

    // 按分数降序排序（冒泡排序）
    for (int i = 0; i < user_system.user_count - 1; i++)
    {
        for (int j = 0; j < user_system.user_count - i - 1; j++)
        {
            if (temp_users[j].high_score < temp_users[j + 1].high_score)
            {
                struct USER temp = temp_users[j];
                temp_users[j] = temp_users[j + 1];
                temp_users[j + 1] = temp;
            }
        }
    }

    // 显示前10名
    int display_count = (user_system.user_count > 10) ? 10 : user_system.user_count;
    for (int i = 0; i < display_count; i++)
    {
        char time_str[20];
        struct tm timeinfo;
        if (localtime_s(&timeinfo, &temp_users[i].last_play_time) == 0)
        {
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M",&timeinfo);
        }
        else
        {
            strcpy_s(time_str, "时间获取失败");
        }
        printf("%-4d\t%-12s\t%-8d\t%s\n",
            i + 1,
            temp_users[i].username,
            temp_users[i].high_score,
            time_str);
    }
}

// 显示主菜单
void showMenu(void)
{
    while (1)
    {
        system("cls");
        printf("================================\n");
        printf("       贪吃蛇游戏 v2.0         \n");
        printf("================================\n");

        if (strlen(current_user) > 0)
        {
            printf("当前用户：%s\n", current_user);
        }

        printf("\n请选择操作：\n");
        printf("1. 登录\n");
        printf("2. 注册\n");
        printf("3. 开始游戏\n");
        printf("4. 查看排行榜\n");
        printf("5. 退出游戏\n");

        if (strlen(current_user) > 0)
        {
            printf("6. 注销\n");
        }

        printf("\n请选择：");

        int choice;
        scanf_s("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:  // 登录
            if (strlen(current_user) > 0)
            {
                printf("请先注销当前用户！\n");
            }
            else
            {
                loginUser();
            }
            system("pause");
            break;

        case 2:  // 注册
            if (strlen(current_user) > 0)
            {
                printf("请先注销当前用户！\n");
            }
            else
            {
                registerUser();
            }
            system("pause");
            break;

        case 3:  // 开始游戏
            if (strlen(current_user) == 0)
            {
                printf("请先登录！\n");
                system("pause");
                break;
            }

            // 重置游戏参数
            score = 0;
            sleepSecond = 300;
            kx = 0;
            ky = 0;

            // 启动游戏
            startGame();

            // 游戏结束后更新分数
            updateScore();
            showScore();
            system("pause");
            break;

        case 4:  // 查看排行榜
            showRanking();
            system("pause");
            break;

        case 5:  // 退出游戏
            printf("感谢游玩！再见！\n");
            saveData();
            return;

        case 6:  // 注销
            if (strlen(current_user) > 0)
            {
                printf("用户 %s 已注销\n", current_user);
                current_user[0] = '\0';
            }
            else
            {
                printf("尚未登录！\n");
            }
            system("pause");
            break;

        default:
            printf("无效的选择！\n");
            system("pause");
            break;
        }
    }
}

// ==================== 主函数 ====================

int main(void)
{
    // 初始化
    srand(time(NULL));  // 播种随机数种子
    initUserSystem();   // 初始化用户系统

    // 显示主菜单
    showMenu();

    return 0;
}