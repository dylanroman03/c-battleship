#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define LENGTH 100
#define ROW 10

int board[LENGTH] = {};
int gameOver = 0;
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

    int added = 0;
    while (added == 0)
    {
      srand((unsigned)time(&t) + i);
      int position = rand() % LENGTH;

      if (i == 0 && (position + 1) % ROW != 0 && (position + 2) % ROW != 0)
      {
        board[position] = 3;
        board[position + 1] = 3;
        board[position + 2] = 3;
        added = 1;
      }
      else if (i == 1 && board[position + ROW] == 0 && position < LENGTH - ROW)
      {
        board[position] = 2;
        board[position + ROW] = 2;
        added = 1;
      }
      else if (i == 2 && board[position] == 0)
      {
        board[position] = 1;
        added = 1;
      }
    }
  }
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
    else if (board[i] == 4)
    {
      printf("\x1b[36m*\x1b[0m\t");
    }
    else if (board[i] == 5)
    {
      printf("\x1b[32mX\x1b[0m\t");
      // } else {
      //   printf("%d\t", board[i]);
      // }
    }
    else
    {
      printf("0\t");
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
  gameOver = 0;
  tries = 0;
  position = 0;
  hit = 0;

  initBoards();
  render();
}

void renderOptions(int win)
{
  int e = 0;
  int selectedOption = 0;
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

  while (selectedOption == 0)
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
        selectedOption = 1;
        if (e == 0)
        {
          playAgain();
        }
        else
        {
          printf("ADIOS\n");
          gameOver = 1;
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
    board[position] = 4;
    break;

  case 2:
    board[position] = 4;
    if (board[position + ROW] == 2)
    {
      board[position + ROW] = 4;
    }
    else
    {
      board[position - ROW] = 4;
    }
    break;

  case 3:
    board[position] = 4;
    if (board[position - 1] == 3)
    {
      board[position - 1] = 4;

      if (board[position - 2] == 3)
      {
        board[position - 2] = 4;
      }
      else
      {
        board[position + 1] = 4;
      }
    }
    else
    {
      board[position + 1] = 4;
      board[position + 2] = 4;
    }
    break;

  default:
    hit--;
    board[position] = 5;
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
    // printf("%c", key);
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
  while (gameOver == 0)
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