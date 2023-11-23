#define PLATFORMWIDTH 40
#define PLATFORMHEIGHT 5
#define PLATFORMCOUNT 5
#define PLATFORMVELOCITY_Y 1
#define PLATFORMXVELOCITY_x 2

int timer;
int difficulty;

typedef struct
{
  int x;
  int y;
  int xVelocity;
  int active;
} PLATFORM;

extern PLATFORM platforms[PLATFORMCOUNT];

// function prototypes
void initializePlatforms();
void updatePlatforms();
void drawPlatforms();