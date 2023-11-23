#include "gba.h"
#include "game.h"
#include "sound.h"
#include "mode4.h"
#include "platform.h"
#include "Jump.h"
#include "player.h"
#include "Idle.h"
#include <stdio.h>
#include <stdlib.h>

int score;
int highScore;

// initialize the game state
void initGame()
{
  score = 0;

  initializePlatforms();
  initializePlayer();

  // sound settings
  REG_SND2CNT = DMG_ENV_VOL(5) | DMG_STEP_TIME(7);
  REG_SND1CNT = DMG_ENV_VOL(5) | DMG_STEP_TIME(7) | DMG_DUTY_50;
  REG_SND1SWEEP = DMG_SWEEP_NUM(7) | DMG_SWEEP_STEPTIME(7) | DMG_SWEEP_DOWN;

  DMANow(3, IdlePal, PALETTE, 150);
}

// wrapper for all updates
void updateGame()
{
  updatePlatforms();
  updatePlayer();

  // increases difficulty at score checkpoints
  switch (score)
  {
  case 5:
    difficulty = 4;
    break;
  case 10:
    difficulty = 3;
    break;
  case 15:
    difficulty = 2;
    break;
  case 20:
    difficulty = 1;
    break;
  default:
    break;
  }
}

// wrapper to draw game elements
void drawGame()
{
  fillScreen4(17);
  drawPlayer();
  drawPlatforms();
}