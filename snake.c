#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<termios.h>
int headX,headY,fruitX,fruitY,width=30,height=30,gameOver=0,direction=2,speeds=0.8i,size=0,score=0,speed=100000;
int bodyX[30],bodyY[30];
void render(void);
void setupGame(void);
void inputCheck(void);
void moveSnake(void);
void gameOverCheck(void);
int kbhit(void);
char getch(void);
void placeFruit(void);
void fruitCheck(void);

int main()
{
  setupGame();
  placeFruit();
  while(!gameOver)
  {
   render();
   inputCheck();
   moveSnake();
   fruitCheck();
   gameOverCheck();
   usleep(speed);
  }
  usleep(speed);
  system("clear");
  printf("\n\n\n\n\t\t\tYour final Score : %5d\n\n\t\t\tThank You for playing!! (press any key to exit)\n\n\n\n",score);
  while(!kbhit());
  system("clear");
  return 0;
}

void render(void)
{
 system("clear");
 int i,j,k,p;
 for(j=0;j<=height;j++)
 {
  for(i=0;i<=width;i++)
  {
    p=1;
    if(i==0||j==0||j==height||i==width)
      printf("*");
    else if(i==fruitX&&j==fruitY)
      printf("0");
    else if(i==headX&&j==headY)
      printf("o");
    else
    {
     for(k=0;k<size;k++)
     {
      if(i==bodyX[k] && j==bodyY[k])
      {
       printf("+");
       p=0;
      }
     }
      if(p)
      printf(" ");
    }
  }
  printf("\n");
 }
 printf("Scored : %5d",score);
}

void placeFruit(void)
{ 
 resetfruitX:fruitX=rand()%20;
 if(fruitX==0||fruitX==width)
 goto resetfruitX;

 resetfruitY:fruitY=rand()%20;
 if(fruitY==0||fruitY==height)
 goto resetfruitY;
}

void setupGame(void)
{
 headX=height/2;
 headY=width/2;
}

void fruitCheck(void)
{
 if(headX==fruitX && headY==fruitY)
 {
  score+=10;
  size++;
  if(speed>50000)
  speed-=500;
  placeFruit();
 }
}

void inputCheck(void)
{
 if(kbhit())
 {
  char key=getch();
  switch (key)
  {
   case 'w':
    if(direction!=3)
    direction=1;
    break;
   case 'd':
    if(direction!=4)
    direction=2;
    break;
   case 's':
    if(direction!=1)
    direction=3;
    break;
   case 'a':
    if(direction!=2)
    direction=4;
  }
 }
}

void moveSnake(void)
{
 int x1,x2,y1,y2,i;
 if(size==1)
 {
  bodyX[0]=headX;
  bodyY[0]=headY;
 }
 else
 {
  x1=headX;
  y1=headY;
  for(i=0;i<size;i++)
  {
   x2=bodyX[i];
   y2=bodyY[i];
   bodyX[i]=x1;
   bodyY[i]=y1;
   x1=x2;
   y1=y2;
  }
 }
 switch (direction)
 {
   case 1:
    headY--;
    break;
   case 2:
    headX++;
    break;
   case 3:
    headY++;
    break;
   case 4:
    headX--;
 }
}

void gameOverCheck(void)
{
 int i;
 for(i=0;i<size;i++)
 {
  if(headX==bodyX[i] && headY==bodyY[i])
    gameOver=1;
 }
 if(headX==width||headX==0||headY==height||headY==0)
   gameOver=1;
}

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

char getch(void)
{
    char c;
    system("stty raw");
    c= getchar();
    system("stty sane");
    return(c);
}