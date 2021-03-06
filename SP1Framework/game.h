#ifndef _GAME_H
#define _GAME_H

#include "Framework\timer.h"
#include <string.h>
#include <fstream>

enum Keys
{
    K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
	K_SPACE,
    K_ESCAPE,
    K_COUNT
};

enum dimension
{
	HEIGHT,
	WIDTH,
	DIMENSION
};

enum move
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	MOVE
};

enum direction
{
	DIRECTION_X,
	DIRECTION_Y,
	DIRECTION
};

enum limit
{
	UPPER,
	LOWER,
	LIMIT
};

struct enemies
{
	COORD location;
	int size[DIMENSION];
	char** model;
	int life;
	bool alwaysMove;
	int x_maxmove;
	int y_maxmove;
	int x_limit[LIMIT];
	int y_limit[LIMIT];
	int direction[DIRECTION];

	void set(int height, int width, int l, bool AM, int xmm, int ymm, int xlu, int xll, int ylu, int yll)
	{
		size[HEIGHT] = height;
		size[WIDTH] = width;
		life = l;
		location.X = 0;
		location.Y = 0;
		alwaysMove = AM;
		x_maxmove = xmm;
		y_maxmove = ymm;
		x_limit[UPPER] = xlu;
		x_limit[LOWER] = xll;
		y_limit[UPPER] = ylu;
		y_limit[LOWER] = yll;
	}

	void setModel (char** extModel)
	{
		model = new char*[size[HEIGHT]];
		for (int count = 0; count < size[HEIGHT]; count ++)
		{
			model[count] = new char[size[WIDTH]];
			for (int count2 = 0; count2 < size[WIDTH]; count2++)
				model[count][count2] = extModel[count][count2];
		}
	}
};

struct levelinfo
{
	int level;
	int number;
	int spawnChance;
	int spawnBuffer;
	bool setSpawn;
	int spawnLocation;
	bool bullets;;
	int nextLevel;
};

void init();                // initialize your variables, allocate memory, etc
void getInput();            // get input from player
void update(double dt);     // update the game and the state of the game
void render();              // renders the current state of the game to the console
void shutdown();            // do clean up, free memory

extern COORD ConsoleSize;
extern char ship[5][20];
extern int shipLen;
extern COORD shipLocation;
extern bool** bullets;
extern levelinfo levelInfo;
extern levelinfo bulletInfo;
extern enemies* enemyArr;
extern enemies* enemyBulletArr;

bool initLevel();
void initEnemies(enemies[], char*, int);
void renderShip();
void moveBullets();
void renderBullets();
bool spawnEnemy(enemies[], int);
bool spawnEnemy(enemies&);
void renderEnemy(enemies[], int);
void renderEnemy(enemies&);
void moveEnemy(enemies[], int);
void moveEnemy(enemies&);
void moveEnemy(enemies&, int&, int&);
void moveEnemyBullet(enemies[], int);
bool checkBulletCollision(enemies[], int);
bool checkBulletCollision(enemies&);
bool checkEnemyCollision (enemies[], int);
bool checkClear(enemies[], int);
bool checkClear(enemies);
bool spawnEnemyBullet(enemies[]);

#endif // _GAME_H