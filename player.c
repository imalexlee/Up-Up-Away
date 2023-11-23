#include "gba.h"
#include "player.h"
#include "platform.h"
#include "mode4.h"
#include "Jump.h"
#include "Idle.h"
#include "print.h"
#include "sound.h"
#include <stdio.h>
#include <stdlib.h>

PLAYER player;
int recordedScore;
int score;

void initializePlayer()
{
  player.x = platforms[1].x + 10;
  player.y = platforms[1].y - PLAYERHEIGHT;
  player.alive = 1;
  player.jumping = 0;
  player.yVelocity = 1;
  player.onPlatform = 1;
  score = 0;
  recordedScore = 1;
}

void updatePlayer()
{

  // player off screen , so dead X_X
  if (player.y > 160)
  {
    player.alive = 0;
  }

  if (BUTTON_HELD(BUTTON_LEFT))
  {
    player.x -= 1;
  }
  if (BUTTON_HELD(BUTTON_RIGHT))
  {
    player.x += 1;
  }

  // if player velocity is positive (going down)
  if (player.yVelocity >= 0)
  {
    player.jumping = 0;

    // if player is standing on platform, go the same speed as it
    // first, assume player is not on a platform
    player.onPlatform = 0;
    for (int i = 0; i < PLATFORMCOUNT; i++)
    {
      if (collision(platforms[i].x, platforms[i].y, PLATFORMWIDTH, PLATFORMHEIGHT, player.x, player.y, PLAYERWIDTH, PLAYERHEIGHT + 2) && platforms[i].active)
      {

        player.y = platforms[i].y - PLAYERHEIGHT + 2;
        player.x += !(timer % difficulty) ? platforms[i].xVelocity : 0;

        // we are on a platform, so stop looking
        player.onPlatform = 1;
        if (!recordedScore)
        {
          score++;
          REG_SND1FREQ = NOTE_E6 | SND_RESET | DMG_FREQ_TIMED;

          recordedScore = 1;
        }
        break;
      }
    }
    // player is not on a platform, so accelerate downwards
    if (!player.onPlatform)
    {
      // !(timer % 2) is simply to slow the acceleration a bit
      if (!(timer % 2))
      {
        player.y += player.yVelocity;
        player.yVelocity += 1;
      }
    }

    // check for jump action
    if (BUTTON_PRESSED(BUTTON_UP) && player.onPlatform)
    {
      player.onPlatform = 0;
      player.jumping = 1;
      player.yVelocity = -10;
      recordedScore = 0;
    }
  }
  // the player is traveling upwards (in a jump)
  else
  {
    if (!(timer % 2))
    {
      player.y += player.yVelocity;
      player.yVelocity += 1;
    }
  }
  // don't allow player to go less than y = 0 (WEIRD VISUAL EFFECTS)
  if (player.y < 0)
  {
    player.y = 0;
    player.yVelocity = 1;
  }
}

void drawPlayer()
{
  // switch between jumping and idle image
  if (player.jumping)
  {

    drawImage4(player.x, player.y, PLAYERWIDTH, PLAYERHEIGHT, JumpBitmap);
  }
  else
  {
    drawImage4(player.x, player.y, PLAYERWIDTH, PLAYERHEIGHT, IdleBitmap);
  }
}