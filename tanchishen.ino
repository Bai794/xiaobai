#include <LedControl.h>

struct snake
{
    unsigned char x[255]; //蛇的x坐标  8x8x4
    unsigned char y[255];
    unsigned char length;    //蛇的长度
    unsigned char direction; //方向
} snk;
struct Food
{
    int x;
    int y;
} food;
#define DIN 7
#define CS 8
#define CLK 9

#define UP 4
#define DOWN 6
#define LEFT 3
#define RIGHT 5

#define btnUp 4
#define btnLeft 5
#define btnRight 3
#define btnDown 6

bool gameOver = true; //游戏结束标志
int snakeAdd = DOWN;  //按键标记
long t = 0;           //控制蛇的速度，也就是snakeRun的执行时间
LedControl led = LedControl(DIN, CLK, CS, 4);
byte neutral[8] = {0x3C, 0x42, 0xA5, 0x81, 0xBD, 0x81, 0x42, 0x3C}; //脸
char scoreArray[10][8] =
    {
        {0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38}, //0
        {0x00, 0x10, 0x30, 0x10, 0x10, 0x10, 0x10, 0x38}, //1
        {0x00, 0x38, 0x44, 0x04, 0x04, 0x78, 0x40, 0x7C}, //2
        {0x00, 0x38, 0x44, 0x04, 0x38, 0x04, 0x44, 0x38}, //3
        {0x00, 0x08, 0x18, 0x28, 0x48, 0x7C, 0x08, 0x08}, //4
        {0x00, 0x7C, 0x40, 0x78, 0x04, 0x04, 0x44, 0x38}, //5
        {0x00, 0x38, 0x44, 0x40, 0x78, 0x44, 0x44, 0x38}, //6
        {0x00, 0x7C, 0x04, 0x04, 0x08, 0x08, 0x08, 0x08}, //7
        {0x00, 0x38, 0x44, 0x44, 0x38, 0x44, 0x44, 0x38}, //8
        {0x00, 0x38, 0x44, 0x44, 0x3C, 0x04, 0x44, 0x38}  //9
};
void get_direction();
void snk_init();
int deadCheck(); //判断蛇撞墙或者装自己
void snakeRun();
void drawSnake();
int displyfood(int x, int y, bool state); //显示一个点

void setup()
{
    Serial.begin(9600);
    snk_init();
    pinMode(btnUp, INPUT_PULLUP); //按键按下为0
    pinMode(btnLeft, INPUT_PULLUP);
    pinMode(btnRight, INPUT_PULLUP);
    pinMode(btnDown, INPUT_PULLUP);
    for (int i = 0; i < 4; i++)
    {
        led.shutdown(i, false); //关闭省电模式，这一步必须做，否则显示不出来(因为在构造函数中，默认开启了省电模
        led.clearDisplay(i);    //清空数据
    }

    // 倒计时
    for (int k = 5; k >= 1; k--)
    {
        for (int i = 0; i < 8; i++)
        {
            led.setRow(0, i, scoreArray[k][i]);
        }
        delay(1000);
    }
    t = millis();
    led.clearDisplay(0); //清除显示
}
void loop()
{
    
    while (gameOver)
    {
        displyfood(food.x, food.y, true);
        keyscan();
        // Serial.println(snakeAdd);
        if (millis() - t > 500)
        {
            t = millis();
            snakeRun();
        }
        deadCheck(); 
        drawSnake();
        if (!gameOver)
        {
            for (int i = 0; i < 4; i++)
            {
                led.clearDisplay(i); //清空数据
            }
        }
    }

    for (int i = 0; i < 8; i++)
    {
        led.setRow(3, i, neutral[i]); //显示标准脸
    }
}
int displyfood(int x, int y, bool state)
{
    if (x <= 7 && y <= 7)
        led.setLed(3, x, y, state);
    else if (x > 7 && y <= 7)
    {
        led.setLed(1, x - 8, y, state);
    }
    else if (y > 7 && x <= 7)
    {
        led.setLed(2, x, y - 8, state);
    }
    else if (x > 7 && y > 7)
    {
        led.setLed(0, x - 8, y - 8, state);
    }
}

void drawSnake()
{

    for (int i = 0; i < snk.length; i++)
    {
        if (snk.x[i] <= 7 && snk.y[i] <= 7)
            led.setLed(3, snk.x[i], snk.y[i], 1);
        else if (snk.x[i] > 7 && snk.y[i] <= 7)
        {
            led.setLed(1, snk.x[i] - 8, snk.y[i], 1);
        }
        else if (snk.y[i] > 7 && snk.x[i] <= 7)
        {
            led.setLed(2, snk.x[i], snk.y[i] - 8, 1);
        }
        else if (snk.x[i] > 7 && snk.y[i] > 7)
        {
            led.setLed(0, snk.x[i] - 8, snk.y[i] - 8, 1);
        }
    }
    delay(100);
    displyfood(snk.x[snk.length - 1], snk.y[snk.length - 1], 0); //清除显示
}
void snk_init()
{
    snk.x[0] = 3;
    snk.y[0] = 0;
    snk.x[1] = 2;
    snk.y[1] = 0;
    snk.x[2] = 1;
    snk.y[2] = 0;
    snk.x[3] = 0;
    snk.y[3] = 0;
    snk.length = 4;
    food.x = 3;
    food.y = 7;
    snk.direction = RIGHT;
}
void snakeRun()
{
    //移动蛇的坐标
    for (int i = snk.length - 1; i > 0; i--)
    {
        snk.x[i] = snk.x[i - 1];

        snk.y[i] = snk.y[i - 1];
    }
    //设置蛇头的位置
    switch (snakeAdd)
    {
    case UP:
        snk.x[0]--;
        break;
    case DOWN:
        snk.x[0]++;
        break;
    case LEFT:
        snk.y[0]--;
        break;
    case RIGHT:
        snk.y[0]++;
        break;
    }
    //蛇吃到食物
    if (snk.x[0] == food.x && snk.y[0] == food.y)
    {
        snk.length++; //蛇身增长
        createFood(); //产生新的食物坐标 
        displyfood(food.x, food.y, true);
    }
}
void keyscan()
{
    if (!digitalRead(btnUp))
        if (snakeAdd != DOWN)
            snakeAdd = UP;
    if (!digitalRead(btnDown))
        if (snakeAdd != UP)
            snakeAdd = DOWN;
    if (!digitalRead(btnLeft))
        if (snakeAdd != RIGHT)
            snakeAdd = LEFT;
    if (!digitalRead(btnRight))
        if (snakeAdd != LEFT)
            snakeAdd = RIGHT;
}

//随机产生食物的位置
void createFood()
{
    while (snk.length < 255)
    {
        food.x = rand() % 15;
        food.y = rand() % 15;
        int i = 0;
        for (; i < snk.length; i++)
        {
            if (food.x == snk.x[i] && food.y == snk.y[i])
                break;
        }
        if (i >= snk.length)
            break;
    }
}
int deadCheck()
{
    unsigned char i;
    //如果第0个点的坐标和其他任意一个点重复，那么蛇就自己撞自己，出错
    for (i = 1; i < snk.length; i++)
    {
        if ((snk.x[0] == snk.x[i]) && (snk.y[0] == snk.y[i]))
        {

            gameOver = false;
            return 1;
        }
    }
    //如果蛇的坐标超出范围
    if (snk.x[0] > 15 || snk.y[0] > 15||snk.x[0]<0||snk.y[0]<0)
    {
        gameOver = false;
        return 1;
    }
    return 0;
}