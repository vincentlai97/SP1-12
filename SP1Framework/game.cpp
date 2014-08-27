#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>

void renderShip ()
{
	for (int count = -2; count <= 2; count++)
	{
		colour(0x0C);
		std::ostringstream ss;
		for (int count2 = 0; ship[count][count2] != '\n'; count2++)
			ss << ship[count + 2][count2];
		writeToBuffer(shipLocation.X, shipLocation.Y + count, ss.str());
	}
}

void moveBullets ()
{
	for (int county = ConsoleSize.Y; county > 0; county--)
		for (int countx = ConsoleSize.X; countx > 1; countx--)
			bullets[county - 1][countx - 1] = bullets[county - 1][countx - 2];
}

void renderBullets ()
{
	for (int county = 0; county < ConsoleSize.Y; county++)
		for (int countx = 0; countx < ConsoleSize.X; countx++)
			if (bullets[county][countx])
				writeToBuffer(countx, county, '-');
}

bool initLevel ()
{
	char buffer[10] = {"LEVEL "}, buffer2[10];
	char enemy[100];
	char enemyBullets[100];

	std::ifstream fs;
	fs.open("Levels.txt");

	char data[100];
	strcat(buffer, itoa(levelInfo.level, buffer2, 10));

	while (fs.getline(data, 100))
	{
		if (strstr(data, buffer))
		{
			fs.getline(data, 100, '=');
			if (strstr(data, "enemy"))
			{
				fs >> std::ws;
				fs.getline(data, 100);
				strcpy(enemy, data);
				fs.getline(data, 100, '=');
			}
			if (strstr(data, "number"))
			{
				fs >> levelInfo.number;
				fs.getline(data, 100, '=');
			}
			if (strstr(data, "spawnchance"))
			{
				fs >> levelInfo.spawnChance;
				fs.getline(data, 100, '=');
			}
			if (strstr(data, "spawnbuffer"))
			{
				fs >> levelInfo.spawnBuffer;
				fs.getline(data, 100, '=');
			}
			if (strstr(data, "setspawn"))
			{
				fs >> std::boolalpha >> levelInfo.setSpawn;
				if (levelInfo.setSpawn)
				{
					fs.getline(data, 100, '=');
					if (strstr(data, "spawnlocation"))
						fs >> levelInfo.spawnLocation;
				}
				fs.getline(data, 100, '=');
			}
			if (strstr(data, "bullets"))
			{
				fs >> std::boolalpha >> levelInfo.bullets;
				fs.getline(data, 100, '=');
				if (levelInfo.bullets)
				{
					if (strstr(data, "enemy"))
					{
						fs >> std::ws;
						fs.getline(data, 100);
						strcpy(enemyBullets, data);
						fs.getline(data, 100, '=');
					}
					if (strstr(data, "number"))
					{
						fs >> bulletInfo.number;
						fs.getline(data, 100, '=');
					}
					if (strstr(data, "spawnchance"))
					{
						fs >> bulletInfo.spawnChance;
						fs.getline(data, 100, '=');
					}
					if (strstr(data, "spawnbuffer"))
					{
						fs >> bulletInfo.spawnBuffer;
						fs.getline(data, 100, '=');
					}
					if (strstr(data, "setspawn"))
					{
						fs >> std::boolalpha >> bulletInfo.setSpawn;
						if (bulletInfo.setSpawn)
						{
							fs.getline(data, 100, '=');
							if (strstr(data, "spawnlocation"))
								fs >> bulletInfo.spawnLocation;
						}
						fs.getline(data, 100, '=');
					}
				}
			}
			if (strstr(data, "nextlevel"))
			{
				fs >> levelInfo.nextLevel;
				fs.getline(data, 100, '=');
			}
			break;
		}
	}

	fs.close();

	if (levelInfo.bullets)
	{
		enemyBulletArr = new enemies[bulletInfo.number];
		initEnemies(enemyBulletArr, enemyBullets, bulletInfo.number);
	}

	enemyArr = new enemies[levelInfo.number];
	initEnemies(enemyArr, enemy, levelInfo.number);

	return 1;
}

void initEnemies (enemies enemyArr[], char* enemy, int number)
{
	int height, width, life;
	bool alwaysMove; int x_maxmove, y_maxmove;
	bool limit; int x_limit[LIMIT] = {ConsoleSize.Y - 2, 0}, y_limit[LIMIT] = {ConsoleSize.Y - 1, 0};
	char** model;

	std::ifstream fs;
	fs.open("Models.txt");

	char data[100];

	while (fs.getline(data, 100))
	{
		if (strstr(data, enemy))
		{
			fs.getline(data, 100, '=');
			if (strstr (data, "height"))
			{
				fs >> height;
				fs.getline(data, 100, '=');
			}
			if (strstr (data, "width"))
			{
				fs >> width;
				fs.getline(data, 100, '=');
			}
			if (strstr (data, "life"))
			{
				fs >> life;
				fs.getline(data, 100, '=');
			}
			if (strstr (data, "model"))
			{
				fs.getline(data, 100);
				model = new char*[height];
				for (int count = 0; count < height; count++)
				{
					model[count] = new char[width];
					fs.getline(data, 100);
					for (int count2 = 0; count2 < width; count2++)
						model[count][count2] = data[count2];
				}
				fs.getline(data, 100, '=');
			}
			if (strstr (data, "alwaysmove"))
			{
				fs >> std::boolalpha >> alwaysMove;
				fs.getline(data, 100, '=');
			}
			if (strstr(data, "x_maxmove"))
			{
				fs >> x_maxmove;
				fs.getline(data, 100, '=');
			}
			if (strstr(data, "y_maxmove"))
			{
				fs >> y_maxmove;
				fs.getline(data, 100, '=');
			}
			if (strstr(data, "limit"))
			{
				fs >> std::boolalpha >> limit;
				if (limit)
				{
					fs.getline(data, 100, '=');
					if (strstr(data, "x_limit"))
					{
						fs >> x_limit[UPPER] >> x_limit[LOWER];
						fs.getline(data, 100, '=');
					}
					if (strstr(data, "y_limit"))
					{
						fs >> y_limit[UPPER] >> y_limit[LOWER];
						fs.getline(data, 100, '=');
					}
				}
			}
		}
	}

	fs.close();

	for (int count = 0; count < number; count++)
	{
		enemyArr[count].set(height, width, life, alwaysMove, x_maxmove, y_maxmove, x_limit[UPPER], x_limit[LOWER], y_limit[UPPER], y_limit[LOWER]);
		enemyArr[count].setModel(model);
	}
}

void renderEnemy (enemies enemyArr[], int size)
{
	for (int count = 0; count < size; count++)
	{
		renderEnemy(enemyArr[count]);
	}
}

void renderEnemy (enemies& enemy)
{
	if (enemy.location.X && enemy.location.Y)
	{
		for (int county = 0; county < enemy.size[HEIGHT]; county++)
		{
			std::ostringstream ss;
			for (int countx = 0; countx < enemy.size[WIDTH]; countx++)
			{
				if (enemy.location.X + countx < 80)
					ss << enemy.model[county][countx];
			}
			writeToBuffer(enemy.location.X, enemy.location.Y + county, ss.str());
		}

		if(!enemy.location.X)
		{
			enemy.location.Y = 0;
		}
	}
}

bool spawnEnemy(enemies enemyArr[], int size)
{
	if (rand()%100 < levelInfo.spawnChance)
		for (int count = 0; count < size; count++)
			if (!(enemyArr[count].location.X || enemyArr[count].location.Y))
				if (spawnEnemy(enemyArr[count]))
					return 1;
	return 0;
}

bool spawnEnemy (enemies& enemy)
{
	if (levelInfo.setSpawn)
	{
		enemy.location.X = 80;
		enemy.location.Y = levelInfo.spawnLocation;
	}
	else
	{
		enemy.location.X = 80;
		enemy.location.Y = rand() % 22 + 1;
	}
	enemy.direction[DIRECTION_X] = enemy.x_maxmove;
	enemy.direction[DIRECTION_Y] = enemy.y_maxmove;
	return 1;
}

void moveEnemy (enemies enemyArr[], int size)
{
	for (int count = 0; count < size; count++)
		moveEnemy(enemyArr[count]);
}

void moveEnemy (enemies& enemy)
{
	if (enemy.location.X && enemy.location.Y)
	{
		if (enemy.alwaysMove)
		{
			moveEnemy(enemy, enemy.direction[DIRECTION_X], enemy.direction[DIRECTION_Y]);
			if (enemy.location.Y == 1 || enemy.location.Y == 21)
				enemy.direction[DIRECTION_Y] *= -1;
		}
		else
		{
			if (!(enemy.direction[DIRECTION_X] || enemy.direction[DIRECTION_Y]))
			{
				switch (rand()%4)
				{
				case UP: enemy.direction[DIRECTION_Y] += rand()%enemy.y_maxmove + 1;
					break;
				case DOWN: enemy.direction[DIRECTION_Y] -= rand()%enemy.y_maxmove + 1;
					break;
				case LEFT: enemy.direction[DIRECTION_X] += rand()%enemy.x_maxmove + 1;
					break;
				case RIGHT: enemy.direction[DIRECTION_X] -= rand()%enemy.x_maxmove + 1;
					break;
				}
			}
			moveEnemy(enemy, enemy.direction[DIRECTION_X], enemy.direction[DIRECTION_Y]);
			if (enemy.direction[DIRECTION_X] > 0) enemy.direction[DIRECTION_X]--;
			else if (enemy.direction[DIRECTION_X] < 0) enemy.direction[DIRECTION_X]++;
			if (enemy.direction[DIRECTION_Y] > 0) enemy.direction[DIRECTION_Y]--;
			else if (enemy.direction[DIRECTION_Y] < 0) enemy.direction[DIRECTION_Y]++;
		}
	}
}

void moveEnemy (enemies& enemy, int& x, int& y)
{
	if (x > 0)
	{
		if (enemy.location.X > enemy.x_limit[LOWER])
			enemy.location.X--;
	}
	else if (x < 0)
	{
		if (enemy.location.X < enemy.x_limit[UPPER])
			enemy.location.X++;
	}
	if (y > 0)
	{
		if (enemy.location.Y < enemy.y_limit[UPPER])
			enemy.location.Y++;
	}
	else if (y < 0)
	{
		if (enemy.location.Y > enemy.y_limit[LOWER])
			enemy.location.Y--;
	}
	if (enemy.location.X <= 0)
	{
		enemy.location.Y = 0;
	}
}

bool checkBulletCollision(enemies enemyArr[], int size)
{
	for (int count = 0; count < size; count++)
	{
		checkBulletCollision(enemyArr[count]);
	}
	return 0;
}


bool checkBulletCollision(enemies& enemy)
{
	for (int count = 0; count < enemy.size[HEIGHT]; count++)
		if (bullets[enemy.location.Y + count][enemy.location.X])
		{
			bullets[enemy.location.Y + count][enemy.location.X] = false;
			if (--enemy.life <= 0)
			{
				enemy.location.X = 0;
				enemy.location.Y = 0;
			}
		}
	return 0;
}

bool checkEnemyCollision (enemies enemyArr[], int size)
{
	for (int count = 0; count < size; count++)
	{
		if (enemyArr[count].size[0] && enemyArr[count].size[1])
		{
			if (enemyArr[count].location.X >= 1 - enemyArr[count].size[0] + shipLocation.X && 
				enemyArr[count].location.X <= shipLen + shipLocation.X &&
				enemyArr[count].location.Y >= -enemyArr[count].size[1] + shipLocation.Y &&
				enemyArr[count].location.Y <= 2 + shipLocation.Y)
			{
				enemyArr[count].location.X = 0;
				enemyArr[count].location.Y = 0;
				return 1;
			}
		}
	}
	return 0;
}

bool checkClear (enemies enemyArr[], int size)
{
	for (int count = 0; count < size; count++)
	{
		if (!checkClear(enemyArr[count]))
			return 0;
	}
	return 1;
}

bool checkClear (enemies enemy)
{
	if (enemy.location.X || enemy.location.Y)
		return 0;
	return 1;
}

bool spawnEnemyBullet (enemies enemyArr[])
{
	if (rand()%100 < bulletInfo.spawnChance)
	{
		for (int count = 0; count < bulletInfo.number; count++)
		{
			if (!(enemyBulletArr[count].location.X || enemyBulletArr[count].location.Y))
			{
				enemyBulletArr[count].location.X = enemyArr[0].location.X;
				enemyBulletArr[count].location.Y = enemyArr[0].location.Y + 9/2;
				enemyBulletArr[count].direction[DIRECTION_X] = enemyBulletArr[count].x_maxmove;
				enemyBulletArr[count].direction[DIRECTION_Y] = enemyBulletArr[count].y_maxmove;
				return 1;
			}
		}
	}
	return 0;
}