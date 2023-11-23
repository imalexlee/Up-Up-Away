#define PLAYERWIDTH 16
#define PLAYERHEIGHT 24

typedef struct
{
  int x;
  int y;
  int alive;
  int jumping;
  int yVelocity;
  int onPlatform;
} PLAYER;

extern PLAYER player;
extern int score;

// function prototypes
void initializePlayer();
void updatePlayer();
void drawPlayer();