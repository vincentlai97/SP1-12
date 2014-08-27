// This is the main file to hold everthing together

#include "Framework\timer.h"
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>


StopWatch g_timer;							// Timer function to keep track of time and the frame rate
bool g_quitGame = false;					// Set to true if you want to quit the game
const unsigned char FPS = 20;				// FPS of this game
const unsigned int frameTime = 1000 / FPS;	// time for each frame

double elapsedTime;
double deltaTime;
bool keyPressed[K_COUNT];

COORD ConsoleSize = {80, 25};				// Console size, width by height

// Game specific variables here
COORD shipLocation;
char ship[5][20] =
{
	"__",
	"| \\",
	"=[_H)--.____",
	"=[+-,------'",
	" [_/"
};
int shipLen = 12;
bool** bullets;
unsigned int bulletBuffer;
enemies* enemyArr;
enemies* enemyBulletArr;
levelinfo levelInfo;
levelinfo bulletInfo;
bool levelStart;
int spawnBufferCount;
int enemiesSpawned;

void mainLoop();

// TODO:
// Bug in waitUnitil. it waits for the time from getElapsedTime to waitUntil, but should be insignificant.

int main()
{
	init();      // initialize your variables
    mainLoop();  // main loop
    shutdown();  // do clean up, if any. free memory.
	
	return 0;
}

// This main loop calls functions to get input, update and render the game
// at a specific frame rate
void mainLoop()
{
    g_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
    while (!g_quitGame)      // run this loop until user wants to quit 
	{        
        getInput();                         // get keyboard input
        update(g_timer.getElapsedTime());   // update the game
        render();                           // render the graphics output to screen
        g_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.      
	}    
}

void init()
{
    // Set precision for floating point output
    elapsedTime = 0.0;

    initConsole(ConsoleSize, "SP1 Framework");

    shipLocation.X = 0;
    shipLocation.Y = ConsoleSize.Y / 2;

	bullets = new bool*[ConsoleSize.Y];
	for (int count = 0; count < ConsoleSize.Y; count++)
	{
		bullets[count] = new bool[ConsoleSize.X];
		for (int count2 = 0; count2 < ConsoleSize.X; count2++)
			bullets[count][count2] = false;
	}
	bulletBuffer = 0;

	levelInfo.level = 11;

	spawnBufferCount = 0;
	enemiesSpawned = 0;
}

void shutdown()
{
    // Reset to white text on black background
	colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    shutDownConsole();
}

void getInput()
{    
    keyPressed[K_UP] = isKeyPressed(VK_UP);
    keyPressed[K_DOWN] = isKeyPressed(VK_DOWN);
    keyPressed[K_LEFT] = isKeyPressed(VK_LEFT);
    keyPressed[K_RIGHT] = isKeyPressed(VK_RIGHT);
	keyPressed[K_SPACE] = isKeyPressed(VK_SPACE);
    keyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);
}

void update(double dt)
{
    // get the delta time
    elapsedTime += dt;
    deltaTime = dt;

    // Updating the location of the character based on the key press
     if (keyPressed[K_UP] && shipLocation.Y > 2)
    {
        Beep(1440, 10);
        shipLocation.Y--;

    }
    if (keyPressed[K_LEFT] && shipLocation.X > 0)
    {
        Beep(1440, 10);
        shipLocation.X--;
    }
    if (keyPressed[K_DOWN] && shipLocation.Y < ConsoleSize.Y - 3)
    {
        Beep(1440, 10);
        shipLocation.Y++;
    }
    if (keyPressed[K_RIGHT] && shipLocation.X < ConsoleSize.X - strlen(ship[3]))
    {
        Beep(1440, 10);
        shipLocation.X++;
    }

	if (keyPressed[K_SPACE] && bulletBuffer == 0)
	{
        Beep(1440, 10);
		bullets[shipLocation.Y][shipLocation.X + shipLen] = true;
		bulletBuffer += 3;
	}
	else
	{
		bullets[shipLocation.Y][shipLocation.X + shipLen] = false;
		if (bulletBuffer) bulletBuffer--;
	}

    // quits the game if player hits the escape key
    if (keyPressed[K_ESCAPE])
        g_quitGame = true;    
}

void render()
{    
    // Clears the buffer with this colour attribute
    clearBuffer(0x00);

    // Set up sample colours, and output shadings
    const WORD colors[] =   {
	                        0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
	                        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
	                        };
	
    COORD c;

    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / deltaTime << "fps";
    c.X = ConsoleSize.X-9;
    c.Y = 0;
    writeToBuffer(c, ss.str());

    // displays the elapsed time
    ss.str("");
    ss << elapsedTime << "secs";
    c.X = 0;
	c.Y = ConsoleSize.Y - 1;
    writeToBuffer(c, ss.str(), 0x59);

	if (!levelStart)
		levelStart = initLevel();

	renderShip();
	moveBullets();
	checkBulletCollision(enemyArr, levelInfo.number);
	renderBullets();
	if (!spawnBufferCount && enemiesSpawned < levelInfo.number)
	{
		if (spawnEnemy(enemyArr, levelInfo.number))
		{
			spawnBufferCount += levelInfo.spawnBuffer;
			enemiesSpawned++;
		}
	}
	else if (spawnBufferCount) spawnBufferCount--;
	moveEnemy(enemyArr, levelInfo.number);
	checkBulletCollision(enemyArr, levelInfo.number);
	checkEnemyCollision(enemyArr, levelInfo.number);
	renderEnemy(enemyArr, levelInfo.number);
	if (levelInfo.bullets)
	{
		if (spawnEnemyBullet(enemyArr))
		{
				spawnBufferCount += bulletInfo.spawnBuffer;
		}
		moveEnemy(enemyBulletArr, bulletInfo.number);
		checkEnemyCollision(enemyBulletArr, bulletInfo.number);
		renderEnemy(enemyBulletArr, bulletInfo.number);
	}
	
	// Writes the buffer to the console, hence you will see what you have written
    flushBufferToConsole();

	if (enemiesSpawned == levelInfo.number) 
	{
		if (checkClear(enemyArr, levelInfo.number)) 
		{
			levelStart = false;
			enemiesSpawned = 0;
			spawnBufferCount = 0;
			delete[] enemyArr;
			delete[] enemyBulletArr;
			levelInfo.level = levelInfo.nextLevel;
		}
	}
}

