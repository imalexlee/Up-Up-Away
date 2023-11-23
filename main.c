#include "gba.h"
#include "mode4.h"
#include "print.h"
#include "sound.h"
#include "game.h"
#include "player.h"
#include "upupaway2.h"
#include <stdlib.h>
#include <stdio.h>

// function prototypes
void initialize();
void goToStart();
void start();
void goToGame();
void game();
void goToPause();
void pause();
void goToLose();
void lose();
void goToScoreboard();
void scoreboard();

// buffer for score count
char buffer[41];

unsigned short oldButtons;
unsigned short buttons;

// tracks page before scoreboard
int prevPage;

// state enums
enum
{
    START,
    GAME,
    PAUSE,
    LOSE,
    SCOREBOARD
};
int state;

int main()
{
    initialize();
    while (1)
    {
        oldButtons = buttons;
        buttons = REG_BUTTONS;
        switch (state)
        {
        case START:
            start();
            break;
        case GAME:
            game();
            break;
        case PAUSE:
            pause();
            break;
        case LOSE:
            lose();
            break;
        case SCOREBOARD:
            scoreboard();
            break;
        }
    }
}

void initialize()
{
    mgba_open();
    REG_DISPCTL = MODE(4) | BG_ENABLE(2) | DISP_BACKBUFFER;
    buttons = REG_BUTTONS;
    oldButtons = 0;
    prevPage = 0;

    REG_SOUNDCNT_X = SND_ENABLED;

    REG_SOUNDCNT_L = DMG_VOL_LEFT(5) |
                     DMG_VOL_RIGHT(5) |
                     DMG_SND1_LEFT |
                     DMG_SND1_RIGHT |
                     DMG_SND2_LEFT |
                     DMG_SND2_RIGHT |
                     DMG_SND3_LEFT |
                     DMG_SND3_RIGHT |
                     DMG_SND4_LEFT |
                     DMG_SND4_RIGHT;
    REG_SOUNDCNT_H = DMG_MASTER_VOL(2);

    goToStart();
}

void goToStart()
{

    DMANow(3, upupaway2Pal, PALETTE, 256);
    drawFullscreenImage4(upupaway2Bitmap);

    drawString4(15, 10, "UP,", 40);
    drawString4(15, 20, "UP,", 40);
    drawString4(15, 30, "AWAY", 40);

    waitForVBlank();
    flipPage();

    state = START;
}

void start()
{
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START))
    {
        goToGame();
        initGame();
    }
    else if (BUTTON_PRESSED(BUTTON_B))
    {
        prevPage = START;
        goToScoreboard();
    }
}

void goToGame()
{
    state = GAME;
}

void game()
{
    updateGame();
    drawGame();

    drawString4(180, 1, "score: ", 7);
    sprintf(buffer, "%d", score);
    drawString4(220, 1, buffer, 7);

    waitForVBlank();
    flipPage();
    if (BUTTON_PRESSED(BUTTON_START))
    {
        goToPause();
    }
    if (!player.alive)
    {
        highScore = score > highScore ? score : highScore;
        goToLose();
    }
}

void goToPause()
{
    fillScreen4(7);
    drawString4(75, 72, "Don't give up!", 254);
    waitForVBlank();
    flipPage();

    state = PAUSE;
}

void pause()
{
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START))
        goToGame();
    else if (BUTTON_PRESSED(BUTTON_SELECT))
        goToStart();
    else if (BUTTON_PRESSED(BUTTON_B))
    {
        prevPage = PAUSE;
        goToScoreboard();
    }
}

void goToLose()
{
    REG_SND2FREQ = NOTE_A2 | SND_RESET;
    fillScreen4(9);
    drawString4(22, 72, "You climbed as far as you could!", 255);
    waitForVBlank();
    flipPage();

    state = LOSE;
}

void lose()
{
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START))
    {
        goToStart();
    }
}

void goToScoreboard()
{
    highScore = score > highScore ? score : highScore;
    fillScreen4(186);
    drawString4(80, 72, "High Score: ", 255);
    sprintf(buffer, "%d", highScore);
    drawString4(150, 72, buffer, 255);
    waitForVBlank();
    flipPage();

    state = SCOREBOARD;
}

void scoreboard()
{
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_B))
    {
        mgba_printf("prev page is %d", prevPage);
        switch (prevPage)
        {
        case PAUSE:
            goToPause();
            break;
        case START:
            goToStart();
            break;
        default:
            break;
        }
    }
}