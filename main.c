#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#define LENGTH 100
#define ROW 10

int board[LENGTH] = {};
char *showBoard[LENGTH] = {};
bool gameOver = false;
int tries = 0, position = 0, hit = 0;

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

  if (ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

char getch(void)
{
  char c;
  // system("stty raw");
  c = getchar();
  // system("stty sane");
  return (c);
}

void initBoards()
{
  system("clear");
  printf("Generando Campo de Batalla...\n");

  for (int i = 0; i < LENGTH; i++)
  {
    board[i] = 0;
  }

  for (int i = 0; i < 3; i++)
  {
    time_t t;

    bool added = false;
    while (!added)
    {
      srand((unsigned)time(&t) + i);
      int position = rand() % LENGTH;

      if (i == 0)
      {
        board[position] = 1;
        added = true;
      }
      else if (i == 1 && (position + 1) % ROW != 0 && board[position + 1] == 0)
      {
        board[position] = 2;
        board[position + 1] = 2;
        added = true;
      }
      else if (i == 2 && position < 20 && position > ROW && board[position - ROW] == 0 && board[position + ROW] == 0)
      {
        board[position] = 3;
        board[position + ROW] = 3;
        board[position - ROW] = 3;
        added = true;
      }
    }
  }


  // ESTO ES PARA DESARROLLO, EN PRODUCTION SE PONE SIMPLEMENTE EN 0
  // for (int i = 0; i < LENGTH; i++)
  // {
  //   if (board[i] == 0)
  //   {
  //     showBoard[i] = "0";
  //   }
  //   else if (board[i] == 1)
  //   {
  //     showBoard[i] = "1";
  //   }
  //   else if (board[i] == 2)
  //   {
  //     showBoard[i] = "2";
  //   }
  //   else
  //   {
  //     showBoard[i] = "3";
  //   }
  // }

  for (int i = 0; i < LENGTH; i++) {
    showBoard[i] = "0";
  }
}

void render()
{
  system("clear");

  for (int i = 0; i < LENGTH; i++)
  {
    if (i == position)
    {
      printf("\x1b[33m+\x1b[0m\t");
    }
    else
    {
      printf("%s\t", showBoard[i]);
      // printf("0\t");
    }

    if ((i + 1) % ROW == 0)
    {
      printf("\n");
    }
  }

  printf("\nINTENTOS RESTANTES: %d", 10 - tries);
  printf("\nACIERTOS: %d\n", hit);
}

void playAgain(){
  gameOver = false;
  tries = 0;
  position = 0;
  hit = 0;

  initBoards();
  render();
}

void renderWin()
{
  int e = 0;
  bool selectedOption = false;

  system("clear");
  printf("FELICIDADES HAS GANADO!!!\n");
  printf("=>JUGAR DE NUEVO\n");
  printf("  SALIR\n");

  while (!selectedOption)
  {
    if (kbhit())
    {
      char key = getch();
      system("clear");
      if (key == 'w')
      {
        e = 0;
        printf("FELICIDADES HAS GANADO!!!\n");
        printf("=>JUGAR DE NUEVO\n");
        printf("  SALIR\n");
      }
      else if (key == 's')
      {
        e = 1;
        printf("FELICIDADES HAS GANADO!!!\n");
        printf("  JUGAR DE NUEVO\n");
        printf("=>SALIR\n");
      }
      else if (key == '\n' && e == 0)
      {
        playAgain();
        selectedOption = true;
      }
      else if (key == '\n' && e == 1)
      {
        printf("ADIOS\n");
        gameOver = true;
        selectedOption = true;
      }
    }
  }
}

void renderFail()
{
  int e = 0;
  bool selectedOption = false;

  system("clear");
  printf("LOSER HAS PERDIDO\n");
  printf("=>JUGAR DE NUEVO\n");
  printf("  SALIR COMO UN BUEN PERDEDOR\n");
  while (!selectedOption)
  {
    if (kbhit())
    {
      char key = getch();
      system("clear");
      if (key == 'w')
      {
        e = 0;
        printf("LOSER HAS PERDIDO\n");
        printf("=>JUGAR DE NUEVO\n");
        printf("  SALIR COMO UN BUEN PERDEDOR\n");
      }
      else if (key == 's')
      {
        e = 1;
        printf("LOSER HAS PERDIDO\n");
        printf("  JUGAR DE NUEVO\n");
        printf("=>SALIR COMO UN BUEN PERDEDOR\n");
      }else if (key == '\n' && e == 0)
      {
        playAgain();
        selectedOption = true;
      }
      else if (key == '\n' && e == 1)
      {
        printf("ADIOS\n");
        gameOver = true;
        selectedOption = true;
      }
    }
  }
}

void shot()
{
  hit++;
  switch (board[position])
  {
  case 1:
    showBoard[position] = "*";
    board[position] = 0;
    break;

  case 2:
    showBoard[position] = "*";
    board[position] = 0;
    if (board[position + 1] == 2)
    {
      showBoard[position + 1] = "*";
      board[position + 1] = 0;
    }
    else
    {
      showBoard[position - 1] = "*";
      board[position - 1] = 0;
    }
    break;

  case 3:
    showBoard[position] = "*";
    board[position] = 0;
    if (board[position - ROW] == 3)
    {
      showBoard[position - ROW] = "*";
      board[position - ROW] = 0;

      if (board[position - ROW * 2] == 3)
      {
        showBoard[position - ROW * 2] = "*";
        board[position - ROW * 2] = 0;
      }
      else
      {
        showBoard[position + ROW] = "*";
        board[position + ROW] = 0;
      }
    }
    else
    {
      showBoard[position + ROW] = "*";
      board[position + ROW] = 0;
      showBoard[position + ROW * 2] = "*";
      board[position + ROW * 2] = 0;
    }
    break;

  default:
    hit--;
    showBoard[position] = "X";
    board[position] = 0;
    break;
  }

  position = 0;

  tries++;
  render();
}

void inputCheck()
{
  if (kbhit())
  {
    char key = getch();
    switch (key)
    {
    case 'd':
      if (position == LENGTH - 1)
      {
        position = 0;
      }
      else
      {
        position = position + 1;
      }
      render();

      break;
    case 'a':
      if (position == 0)
      {
        position = LENGTH - 1;
      }
      else
      {
        position = position - 1;
      }
      render();

      break;
    case 's':
      if (position > LENGTH - 6)
      {
        position = (position - (LENGTH - ROW));
      }
      else
      {
        position = position + ROW;
      }
      render();

      break;
    case 'w':
      if (position < ROW)
      {
        position = (position + (LENGTH - ROW));
      }
      else
      {
        position = position - ROW;
      }
      render();
      break;
    case '\n':
      shot();
      break;
    }
  }
}

void main()
{
  initBoards();
  render();
  while (!gameOver)
  {
    inputCheck();
    if (hit == 3)
    {
      renderWin();
    }
    else if (tries == 10)
    {
      renderFail();
    }
  }
}