#include "gba.h"
#include "platform.h"
#include "mode4.h"
#include "print.h"
#include <stdio.h>
#include <stdlib.h>

PLATFORM platforms[PLATFORMCOUNT];

int yVelocity;
int rseed;
int timer;
// difficulty = "how many frames do I wait before I update position"
int difficulty;

void initializePlatforms()
{
  yVelocity = 1;
  rseed = 0;
  timer = 0;
  difficulty = 5;
  for (int i = 0; i < PLATFORMCOUNT; i++)
  {
    rseed += rand();
    platforms[i].x = rand() % (SCREENWIDTH - PLATFORMWIDTH);
    platforms[i].y = i * 55;
    platforms[i].xVelocity = 4;
    platforms[i].active = i * 55 >= 160 ? 0 : 1;
  }
}

void updatePlatforms()
{
  rseed += rand();
  timer++;
  // reduces updates based on difficulty
  if (!(timer % difficulty))
  {
    for (int i = 0; i < PLATFORMCOUNT; i++)
    {
      // platform not active. make ready at top and set random x and direction
      if (!platforms[i].active)
      {
        platforms[i].y = 0;
        platforms[i].x = rand() % (160 - PLATFORMWIDTH);
        platforms[i].xVelocity = rand() % 2 ? 4 : -4;
      }
      // platform is touching right wall, flip direction
      if (platforms[i].x + platforms[i].xVelocity + PLATFORMWIDTH >= 240 && platforms[i].xVelocity > 0)
      {

        platforms[i].xVelocity = -platforms[i].xVelocity;
      }
      // platform is touching left wall, flip direction
      if (platforms[i].x + platforms[i].xVelocity <= 0 && platforms[i].xVelocity < 0)
      {

        platforms[i].xVelocity = -platforms[i].xVelocity;
      }

      platforms[i].x += platforms[i].xVelocity;

      // deactivate platform once low enough
      if (platforms[i].y + PLATFORMHEIGHT > 164)
      {

        platforms[i].active = 0;
        platforms[(i + 2) % PLATFORMCOUNT].active = 1;
      }
      // else, move the platform down still
      else
      {
        platforms[i].y = (platforms[i].y + yVelocity) % 165;
      }
    }
  }
}

void drawPlatforms()
{
  for (int i = 0; i < PLATFORMCOUNT; i++)
  {
    if (platforms[i].active)
    {
      drawRect4(platforms[i].x, platforms[i].y, PLATFORMWIDTH, PLATFORMHEIGHT, 18);
    }
  }
}