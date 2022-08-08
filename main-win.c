#include <stdio.h>
#include <conio.h>
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

void initBoards()
{
  printf("\x1b[33mGenerando Campo de Batalla...\x1b[0m\t");

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

      if (i == 0 && (position + 1) % ROW != 0 && (position + 2) % ROW != 0)
      {
        board[position] = 3;
        board[position + 1] = 3;
        board[position + 2] = 3;
        added = true;
      }
      else if (i == 1 && board[position + ROW] == 0 && position < LENGTH - ROW)
      {
        board[position] = 2;
        board[position + ROW] = 2;
        added = true;
      }
      else if (i == 2 && board[position] == 0)
      {
        board[position] = 1;
        added = true;
      }
    }
  }

  // ESTO ES PARA DESARROLLO, EN PRODUCTION SE PONE SIMPLEMENTE EN 0
  for (int i = 0; i < LENGTH; i++)
  {
    if (board[i] == 0)
    {
      showBoard[i] = "0";
    }
    else if (board[i] == 1)
    {
      showBoard[i] = "1";
    }
    else if (board[i] == 2)
    {
      showBoard[i] = "2";
    }
    else
    {
      showBoard[i] = "3";
    }
  }

  // for (int i = 0; i < LENGTH; i++)
  // {
  //   showBoard[i] = "0";
  // }
}

void render()
{
  system("cls");

  for (int i = 0; i < LENGTH; i++)
  {
    if (i == position)
    {
      printf("\x1b[31m+\x1b[0m\t");
    }
    // else if (showBoard[i] == 4)
    // {
    //   printf("\t\x1b[36m*\x1b[0m");
    // }
    // else if (showBoard[i] == 5)
    // {
    //   printf("\t\x1b[32mX\x1b[0m");
    // } else {
    //   printf("\t0");
    // }

    else
    {
      printf("%s\t", showBoard[i]);
    }

    if ((i + 1) % ROW == 0)
    {
      printf("\n");
    }
  }

  printf("\n\x1b[33mINTENTOS RESTANTES:\x1b[0m %d", 10 - tries);
  printf("\n\x1b[33mACIERTOS:\x1b[0m %d\n", hit);
}

void playAgain()
{
  gameOver = false;
  tries = 0;
  position = 0;
  hit = 0;

  initBoards();
  render();
}

void renderOptions(int win)
{
  int e = 0;
  bool selectedOption = false;
  system("cls");

  if (win == 1)
  {
    printf("FELICIDADES HAS GANADO!!!\n");
  }
  else
  {
    printf("LOSER HAS PERDIDO\n");
  }

  printf("\x1b[33m=>\x1b[0mJUGAR DE NUEVO\n");
  printf("  SALIR\n");

  while (!selectedOption)
  {
    if (kbhit())
    {
      char key = getch();
      system("cls");
      if (key == 'w')
      {
        e = 0;
        if (win == 1)
        {
          printf("FELICIDADES HAS GANADO!!!\n");
        }
        else
        {
          printf("LOSER HAS PERDIDO\n");
        }

        printf("\x1b[33m=>\x1b[0mJUGAR DE NUEVO\n");
        printf("  SALIR\n");
      }
      else if (key == 's')
      {
        e = 1;
        if (win == 1)
        {
          printf("FELICIDADES HAS GANADO!!!\n");
        }
        else
        {
          printf("LOSER HAS PERDIDO\n");
        }
        printf("  JUGAR DE NUEVO\n");
        printf("\x1b[33m=>\x1b[0mSALIR\r");
      }
      else if (key == '\r')
      {
        selectedOption = true;
        if (e == 0)
        {
          playAgain();
        }
        else
        {
          printf("ADIOS\n");
          gameOver = true;
        }
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
    showBoard[position] = "\x1b[36m*\x1b[0m";
    board[position] = 0;
    break;

  case 2:
    showBoard[position] = "\x1b[36m*\x1b[0m";
    board[position] = 0;
    if (board[position + ROW] == 2)
    {
      showBoard[position + ROW] = "\x1b[36m*\x1b[0m";
      board[position + ROW] = 0;
    }
    else
    {
      showBoard[position - ROW] = "\x1b[36m*\x1b[0m";
      board[position - ROW] = 0;
    }
    break;

  case 3:
    showBoard[position] = "\x1b[36m*\x1b[0m";
    board[position] = 0;
    if (board[position - 1] == 3)
    {
      showBoard[position - 1] = "\x1b[36m*\x1b[0m";
      board[position - 1] = 0;

      if (board[position - 2] == 3)
      {
        showBoard[position - 2] = "\x1b[36m*\x1b[0m";
        board[position - 2] = 0;
      }
      else
      {
        showBoard[position + 1] = "\x1b[36m*\x1b[0m";
        board[position + 1] = 0;
      }
    }
    else
    {
      showBoard[position + 1] = "\x1b[36m*\x1b[0m";
      board[position + 1] = 0;
      showBoard[position + 2] = "\x1b[36m*\x1b[0m";
      board[position + 2] = 0;
    }
    break;

  default:
    hit--;
    showBoard[position] = "\x1b[32mX\x1b[0m";
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
    printf("%c", key);
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
    case '\r':
      shot();
      break;
    }
  }
}

void main()
{
  system("cls");
  initBoards();
  render();
  while (!gameOver)
  {
    inputCheck();
    if (hit == 3)
    {
      renderOptions(1);
    }
    else if (tries == 10)
    {
      renderOptions(0);
    }
  }
}