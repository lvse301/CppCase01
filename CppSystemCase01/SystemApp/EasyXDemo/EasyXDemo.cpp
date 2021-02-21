// EasyXDemo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <graphics.h>
#include <ctime>
#include <conio.h>
#include <vector>
using namespace std;



// 枚举蛇的方向
enum position { Up=1, Down, Left, Right };		

// 坐标属性
struct point
{
	int x;
	int y;
};



// 蛇的属性
struct Snake
{
	vector <point> xy;			// 每节坐标
	point next;					// 为下一节预留的位置
	vector <COLORREF> color;	// 每节颜色
	int num;					// 长度
	int position;				// 方向
}snake;

// 食物的属性
struct Food
{
	point fdxy[10];			// 坐标
	int grade;				// 分数
	int num = 1;			// 食物总数
	COLORREF color[10];		// 食物颜色
}food;



// 初始化蛇
void initSnake()
{
	point xy;
	xy.x = 20;
	xy.y = 0;
	snake.xy.push_back(xy);
	snake.color.push_back(RGB(rand() % 256, rand() % 256, rand() % 256));	// 设置一个随机颜色
	xy.x = 10;
	xy.y = 0;
	snake.xy.push_back(xy);
	snake.color.push_back(RGB(rand() % 256, rand() % 256, rand() % 256));	// 设置一个随机颜色
	xy.x = 0;
	xy.y = 0;
	snake.xy.push_back(xy);
	snake.color.push_back(RGB(rand() % 256, rand() % 256, rand() % 256));	// 设置一个随机颜色
	snake.num = 3;
	snake.position = Right;
}

// 画蛇
void drawSnake()
{
	for (int i = 0; i < snake.num; i++)
	{
		setfillcolor(snake.color[i]);
		fillrectangle(snake.xy[i].x, snake.xy[i].y, snake.xy[i].x + 10, snake.xy[i].y + 10);
	}
}

// 移动蛇
void moveSnake()
{
	// 将预留节设置为未移动前的尾节
	snake.next = snake.xy[snake.num - 1];
	// 将除蛇头以外的节移动到它的前面一节
	for (int i = snake.num - 1; i >= 1; i--)
		snake.xy[i] = snake.xy[i - 1];
	// 根据当前移动方向移动蛇头
	switch (snake.position)
	{
	case Right:
		snake.xy[0].x += 10;
		break;
	case Left:
		snake.xy[0].x -= 10;
		break;
	case Up:
		snake.xy[0].y -= 10;
		break;
	case Down:
		snake.xy[0].y += 10;
	}
}

// 按键交互
void keyDown()
{
	char userKey = _getch();
	if (userKey == -32)			// 表明这是方向键
		userKey = -_getch();	// 获取具体方向，并避免与其他字母的 ASCII 冲突
	switch (userKey)
	{
	case 'w':
	case 'W':
	case -72:
		if (snake.position != Down)
			snake.position = Up;
		break;
	case 's':
	case 'S':
	case -80:
		if (snake.position != Up)
			snake.position = Down;
		break;
	case 'a':
	case 'A':
	case -75:
		if (snake.position != Right)
			snake.position = Left;
		break;
	case 'd':
	case 'D':
	case -77:
		if (snake.position != Left)
			snake.position = Right;
		break;
	}
}

// 初始化食物
void initFood(int num /* 食物编号 */)
{
	food.fdxy[num].x = rand() % 80 * 10;
	food.fdxy[num].y = rand() % 60 * 10;
	for (int i = 0; i < snake.num; i++)
		if (food.fdxy[num].x == snake.xy[i].x && food.fdxy[num].y == snake.xy[i].y)		// 避免食物生成在蛇身上
		{
			food.fdxy[num].x = rand() % 80 * 10;
			food.fdxy[num].y = rand() % 60 * 10;
		}
}

// 画食物
void drawFood()
{
	for (int i = 0; i <= food.num - 1; i++)
	{
		setfillcolor(food.color[i] = RGB(rand() % 256, rand() % 256, rand() % 256));	// 每次重新赋予食物一个随机的颜色
		fillrectangle(food.fdxy[i].x, food.fdxy[i].y, food.fdxy[i].x + 10, food.fdxy[i].y + 10);
	}
}

// 吃食物
void eatFood()
{
	for (int i = 0; i <= food.num - 1; i++)
		if (snake.xy[0].x == food.fdxy[i].x && snake.xy[0].y == food.fdxy[i].y)
		{
			snake.num++;
			snake.xy.push_back(snake.next);					// 新增一个节到预留位置
			snake.color.push_back(food.color[i]);			// 将新增节的颜色设置为当前吃掉食物的颜色
			food.grade += 100;
			initFood(i);
			if (food.num < 10 && food.grade % 500 == 0 && food.grade != 0)
			{
				food.num++;									// 每得 500 分，增加一个食物，但食物总数不超过 10 个
				initFood(food.num - 1);						// 初始化新增加的食物
			}
			break;
		}
}

// 分数显示
void showgrade()
{
	wchar_t grade[20] = L"";
	swprintf_s(grade, L"分数:%d", food.grade);
	outtextxy(650, 50, grade);
}

// 游戏结束
bool gameOver()
{
	// 撞墙，将墙向外扩展一圈（否则蛇无法到达地图边缘）
	if (snake.xy[0].y <= -10 && snake.position == Up)			return true;
	if (snake.xy[0].y + 10 >= 610 && snake.position == Down)	return true;
	if (snake.xy[0].x <= -10 && snake.position == Left)			return true;
	if (snake.xy[0].x + 10 >= 810 && snake.position == Right)	return true;
	// 撞自己
	for (int i = 1; i < snake.num; i++)
	{
		if (snake.xy[0].x <= snake.xy[i].x + 10 && snake.xy[0].x >= snake.xy[i].x && snake.xy[0].y == snake.xy[i].y && snake.position == Left)
			return true;
		if (snake.xy[0].x + 10 >= snake.xy[i].x && snake.xy[0].x + 10 <= snake.xy[i].x + 10 && snake.xy[0].y == snake.xy[i].y && snake.position == Right)
			return true;
		if (snake.xy[0].y <= snake.xy[i].y + 10 && snake.xy[0].y >= snake.xy[i].y && snake.xy[0].x == snake.xy[i].x && snake.position == Up)
			return true;
		if (snake.xy[0].y + 10 >= snake.xy[i].y && snake.xy[0].y + 10 <= snake.xy[i].y + 10 && snake.xy[0].x == snake.xy[i].x && snake.position == Down)
			return true;
	}
	return false;
}



int main()
{
	std::cout << "Hello World!\n";


	initgraph(800, 600);
	setbkcolor(RGB(95, 183, 72));
	srand((unsigned)time(NULL));
	settextcolor(BLUE);
	setbkmode(TRANSPARENT);			// 设置文字输出模式为透明
	initSnake();
	initFood(0);
	drawSnake();
	while (!gameOver())
	{
		Sleep(150);
		BeginBatchDraw();			// 开始批量绘图，作用是避免闪烁
		cleardevice();
		if (_kbhit()) keyDown();
		moveSnake();
		eatFood();
		drawFood();
		drawSnake();
		showgrade();
		EndBatchDraw();				// 结束批量绘图
	}
	// 如果因为撞墙死亡则退回到撞墙前的状态以便将蛇完整显示出来
	if (snake.xy[0].y <= -10 && snake.position == Up ||
		snake.xy[0].y + 10 >= 610 && snake.position == Down ||
		snake.xy[0].x <= -10 && snake.position == Left ||
		snake.xy[0].x + 10 >= 810 && snake.position == Right)
	{
		for (int i = 0; i <= snake.num - 2; i++)
			snake.xy[i] = snake.xy[i + 1];
		snake.xy[snake.num - 1] = snake.next;
		drawSnake();
	}
	_getch();						// 按任意键退出
	return 0;




}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
