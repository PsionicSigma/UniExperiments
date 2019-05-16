#include "pch.h"
#include <SDL.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>

using namespace std;

#define PI 3.14159

//					CONSTANTS AND VARIABLES

//Window height and width
const int MAP_WIDTH = 5000;
const int MAP_HEIGHT = 5000;
const int CAMERA_WIDTH = 1000;
const int CAMERA_HEIGHT = 1000;
bool DEJA_VU = false;
bool GOTTA_GO_FAST = false;
bool STORMTROOPER = false;
int angle_mod;
int speed_mod;
int acc_mod;
time_t currenttime;

//Game window, Textures, Enums
enum ALL_IMAGES
{
	TURRET_TORPEDO,
	TURRET_SMALL,
	TURRET_MEDIUM,
	TURRET_BIG,
	SHIP_SMALL,
	SHIP_SMALL_DEAD,
	SHIP_MEDIUM,
	SHIP_MEDIUM_DEAD,
	SHIP_BIG,
	SHIP_BIG_DEAD,
	BULLET,
	TORPEDO,
	BIG_MAP,
	BIG_MAP_OVERLAY,
	DIRECTOR,
	TOTAL_N_OF_IMAGES
};
//Enumeration of ship classes
enum SHIP_CLASSES
{
	SHIP_DESTROYER,
	SHIP_CRUISER,
	SHIP_BATTLESHIP
};

SDL_Window* Main_Game_Window = NULL;
SDL_Renderer* Main_Renderer = NULL;
SDL_Texture* Main_Texture = NULL;
SDL_Texture* IMAGES[TOTAL_N_OF_IMAGES];
const Uint8* Keyboard_State = NULL;
SDL_Rect Camera_Rect;

//					FUNCTIONS

//Start SDL
void Init_SDL()
{
	SDL_Init(SDL_INIT_VIDEO);
	Main_Game_Window = SDL_CreateWindow("Ocean King", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CAMERA_WIDTH, CAMERA_HEIGHT, SDL_WINDOW_SHOWN);
	Main_Renderer = SDL_CreateRenderer(Main_Game_Window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(Main_Renderer, 0x00, 0x00, 0xff, 0x00);
}

//Quit SDL
void Quit_SDL()
{
	SDL_DestroyWindow(Main_Game_Window);
	Main_Game_Window = NULL;
	SDL_DestroyRenderer(Main_Renderer);
	Main_Renderer = NULL;
	SDL_DestroyTexture(Main_Texture);
	Main_Texture = NULL;

	SDL_Quit();
}

//Load surface
SDL_Surface* Load_Surface(std::string path)
{
	SDL_Surface* LoadedSurface = SDL_LoadBMP( path.c_str() );
	return LoadedSurface;
}

//Load texture
SDL_Texture* Load_Texture(std::string path)
{ 
	SDL_Surface* Loaded_Surface = SDL_LoadBMP( path.c_str() );
	if (Loaded_Surface == NULL) cout << "Surface fail\n";
	SDL_SetColorKey(Loaded_Surface,SDL_TRUE,SDL_MapRGB(Loaded_Surface->format,255,255,255));
	SDL_Texture* Loaded_Texture = SDL_CreateTextureFromSurface( Main_Renderer, Loaded_Surface );
	if (Loaded_Texture == NULL) cout << "Texture fail\n" << SDL_GetError();
	SDL_FreeSurface( Loaded_Surface );
	return Loaded_Texture; 
}

//Load images of ships and turrets
void Load_All_Images()
{
	IMAGES[TURRET_TORPEDO] = Load_Texture("Images/TURRET_TORPEDO.bmp");
	IMAGES[TURRET_SMALL] = Load_Texture("Images/TURRET_SMALL.bmp");
	IMAGES[TURRET_MEDIUM] = Load_Texture("Images/TURRET_MEDIUM.bmp");
	IMAGES[TURRET_BIG] = Load_Texture("Images/TURRET_BIG.bmp");
	IMAGES[SHIP_SMALL] = Load_Texture("Images/SHIP_SMALL.bmp");
	IMAGES[SHIP_SMALL_DEAD] = Load_Texture("Images/SHIP_SMALL_DEAD.bmp");
	IMAGES[SHIP_MEDIUM] = Load_Texture("Images/SHIP_MEDIUM.bmp");
	IMAGES[SHIP_MEDIUM_DEAD] = Load_Texture("Images/SHIP_MEDIUM_DEAD.bmp");
	IMAGES[SHIP_BIG] = Load_Texture("Images/SHIP_BIG.bmp");
	IMAGES[SHIP_BIG_DEAD] = Load_Texture("Images/SHIP_BIG_DEAD.bmp");
	IMAGES[BULLET] = Load_Texture("Images/BULLET.bmp");
	IMAGES[TORPEDO] = Load_Texture("Images/TORPEDO.bmp");
	IMAGES[BIG_MAP] = Load_Texture("Images/BIG_MAP.bmp");
	IMAGES[BIG_MAP_OVERLAY] = Load_Texture("Images/MAP_OVERLAY.bmp");
	IMAGES[DIRECTOR] = Load_Texture("Images/DIRECTOR.bmp");
}

//					CLASSES AND FUNCTIONS

bool Is_Inrange(int x1, int y1, int x2, int y2, int range)
{
	int x = x1 - x2; int y = y1 - y2;
	if (x*x + y * y < range*range) return true;
	else return false;
};

double Get_Target_Angle(int tx, int ty, int sx, int sy)
{
	double angle, diag, sinstat, cosstat;
	sinstat = tx - sx; cosstat = ty - sy;
	diag = sqrt(sinstat*sinstat + cosstat * cosstat);
	angle = asin(sinstat*1.0 / diag)*180.0 / PI;
	if (ty <= sy) angle = 180.0 + asin(sinstat*1.0 / diag)*180.0 / PI;
	if (ty > sy) angle = -1 * asin(sinstat*1.0 / diag)*180.0 / PI;
	return angle;
};

struct turret_struct
{
	int dist, CenterX, CenterY;
	SDL_Texture *TurretTexture;
	SDL_Rect DestRectTurret;
};

class Shot
{
private:

	int dmg, distance, shotCenterX, shotCenterY, velocity, ShotCollRange, destination;
	SDL_Rect DestRectShot;
	double shotangle;
	bool Inair;

public:

	Shot(double angle, int shotdamage, int shotvelocity, int targetdistance, int CenterX, int CenterY)
	{
		shotangle = angle;
		dmg = shotdamage;
		velocity = shotvelocity;
		distance = destination = targetdistance;
		shotCenterX = CenterX;
		shotCenterY = CenterY;
		Inair = true;
	}

	int GetCenterX() { return shotCenterX; }; int GetCenterY() { return shotCenterY; }; double GetAX() { return shotangle; }; int GetCollRange() { return ShotCollRange; }; bool GetStatus() { return Inair; }; int GetDamage() { return dmg; }; int GetDist() { return distance; }; int GetDest() { return destination; };

	void Hit() { Inair = false; };
	
	void Render_Shot()
	{
		SDL_RenderCopyEx(Main_Renderer, IMAGES[BULLET], NULL, &DestRectShot, shotangle, NULL, SDL_FLIP_NONE);
	}
	
	void Update_Shot()
	{		
		shotCenterX += velocity * sin(-1 * shotangle*PI / 180);
		shotCenterY += velocity * cos(-1 * shotangle*PI / 180);
		
		SDL_QueryTexture(IMAGES[BULLET], NULL, NULL, &DestRectShot.w, &DestRectShot.h);
		ShotCollRange = DestRectShot.w / 2;

		distance -= velocity;
		
		if (shotCenterX > MAP_WIDTH || shotCenterX < 0 || shotCenterY > MAP_HEIGHT || shotCenterY < 0 || distance < 0)Inair = false;
		DestRectShot.x = shotCenterX - DestRectShot.w / 2 - Camera_Rect.x; DestRectShot.y = shotCenterY - DestRectShot.h / 2 - Camera_Rect.y;
	}

	~Shot() {};
};

vector<Shot> Shots_To_Render;

Shot Create_Shot(double angle, int shotdamage, int shotvelocity, int targetdistance, int CenterX, int CenterY)
{
	Shot new_shot(angle, shotdamage, shotvelocity, targetdistance, CenterX, CenterY);
	return new_shot;
	cout << shotdamage << "\n";
};

class Ship
{
private:

	int hp, shipclass, shipCenterX, shipCenterY, shipCollRange, dot;
	SDL_Rect DestRectShip;
	SDL_Texture *ShipTexture, *DeadTexture;
	turret_struct tempTurret;
	vector<turret_struct> ShipTurrets;
	double shipAx, turretAx, turretrot, shiptraverse, shiprot, topspeed, speed, targetAx;
	bool IsAlive;
	time_t shotcooldown, actiontime, checktime;

public:

	friend class Shot;
	
	Ship(int clasnum)
	{
		SDL_Rect tempRect;
		shipclass = clasnum;
		checktime = time(NULL);
		switch (clasnum)
		{
		case SHIP_DESTROYER:
			hp = 1000; topspeed = 10.0; speed = 0.0; turretrot = 3.0; shiptraverse = 0.0; shiprot = 2.0; ShipTexture = IMAGES[SHIP_SMALL], DeadTexture=IMAGES[SHIP_SMALL_DEAD], dot=0, shotcooldown=5;
			tempTurret.TurretTexture = IMAGES[TURRET_SMALL];
			SDL_QueryTexture(tempTurret.TurretTexture, NULL, NULL, &tempRect.w, &tempRect.h); tempTurret.DestRectTurret.w = tempRect.w; tempTurret.DestRectTurret.h = tempRect.h;
			tempTurret.dist = -28;
			ShipTurrets.push_back(tempTurret);
			tempTurret.dist = 26;
			ShipTurrets.push_back(tempTurret);
			tempTurret.TurretTexture = IMAGES[TURRET_TORPEDO];
			SDL_QueryTexture(tempTurret.TurretTexture, NULL, NULL, &tempRect.w, &tempRect.h); tempTurret.DestRectTurret.w = tempRect.w; tempTurret.DestRectTurret.h = tempRect.h;
			tempTurret.dist = -6;
			ShipTurrets.push_back(tempTurret);
			break;
		case SHIP_CRUISER:
			hp = 1500; topspeed = 7.5; speed = 0.0; turretrot = 2.0; shiptraverse = 0.0; shiprot = 1.5; ShipTexture = IMAGES[SHIP_MEDIUM], DeadTexture = IMAGES[SHIP_MEDIUM_DEAD], dot=0, shotcooldown = 10;
			tempTurret.TurretTexture = IMAGES[TURRET_MEDIUM];
			SDL_QueryTexture(tempTurret.TurretTexture, NULL, NULL, &tempRect.w, &tempRect.h); tempTurret.DestRectTurret.w = tempRect.w; tempTurret.DestRectTurret.h = tempRect.h;
			tempTurret.dist = -34;
			ShipTurrets.push_back(tempTurret);
			tempTurret.dist = 20;
			ShipTurrets.push_back(tempTurret);
			tempTurret.dist = 38;
			ShipTurrets.push_back(tempTurret);
			tempTurret.TurretTexture = IMAGES[TURRET_TORPEDO];
			SDL_QueryTexture(tempTurret.TurretTexture, NULL, NULL, &tempRect.w, &tempRect.h); tempTurret.DestRectTurret.w = tempRect.w; tempTurret.DestRectTurret.h = tempRect.h;
			tempTurret.dist = 8;
			ShipTurrets.push_back(tempTurret);
			tempTurret.dist = 10;
			ShipTurrets.push_back(tempTurret);
			break;
		case SHIP_BATTLESHIP:
			hp = 2000; topspeed = 5.0; speed = 0.0; turretrot = 1.0; shiptraverse = 0.0; shiprot = 1.0; ShipTexture = IMAGES[SHIP_BIG], DeadTexture = IMAGES[SHIP_BIG_DEAD], dot=0, shotcooldown = 15;
			tempTurret.TurretTexture = IMAGES[TURRET_BIG];
			SDL_QueryTexture(tempTurret.TurretTexture, NULL, NULL, &tempRect.w, &tempRect.h); tempTurret.DestRectTurret.w = tempRect.w; tempTurret.DestRectTurret.h = tempRect.h;
			tempTurret.dist = -42;
			ShipTurrets.push_back(tempTurret);
			tempTurret.dist = 10;
			ShipTurrets.push_back(tempTurret);
			tempTurret.dist = 30;
			ShipTurrets.push_back(tempTurret);
			break;
		}
		SDL_QueryTexture(ShipTexture, NULL, NULL, &DestRectShip.w, &DestRectShip.h);
		shipCenterX = CAMERA_WIDTH / 2; shipCenterY = CAMERA_HEIGHT / 2;
		DestRectShip.x = shipCenterX - DestRectShip.w / 2; DestRectShip.y = shipCenterY - DestRectShip.h / 2;
		shipAx = turretAx = 0.0;
		shipCollRange = DestRectShip.w / 2;
		IsAlive = true;
	}

	Ship(int clasnum, int randx, int randy, double randdir)
	{
		SDL_Rect tempRect;
		shipclass = clasnum;
		checktime = time(NULL);
		switch (clasnum)
		{
		case SHIP_DESTROYER:
			hp = 1000; topspeed = 10.0; speed = 0.0; turretrot = 3.0; shiptraverse = 0.0; shiprot = 2.0; ShipTexture = IMAGES[SHIP_SMALL], DeadTexture = IMAGES[SHIP_SMALL_DEAD], dot=0, shotcooldown = 5;
			tempTurret.TurretTexture = IMAGES[TURRET_SMALL];
			SDL_QueryTexture(tempTurret.TurretTexture, NULL, NULL, &tempRect.w, &tempRect.h); tempTurret.DestRectTurret.w = tempRect.w; tempTurret.DestRectTurret.h = tempRect.h;
			tempTurret.dist = -28;
			ShipTurrets.push_back(tempTurret);
			tempTurret.dist = 26;
			ShipTurrets.push_back(tempTurret);
			tempTurret.TurretTexture = IMAGES[TURRET_TORPEDO];
			SDL_QueryTexture(tempTurret.TurretTexture, NULL, NULL, &tempRect.w, &tempRect.h); tempTurret.DestRectTurret.w = tempRect.w; tempTurret.DestRectTurret.h = tempRect.h;
			tempTurret.dist = -6;
			ShipTurrets.push_back(tempTurret);
			break;
		case SHIP_CRUISER:
			hp = 1500; topspeed = 7.5; speed = 0.0; turretrot = 2.0; shiptraverse = 0.0; shiprot = 1.5; ShipTexture = IMAGES[SHIP_MEDIUM], DeadTexture = IMAGES[SHIP_MEDIUM_DEAD], dot=0, shotcooldown = 10;
			tempTurret.TurretTexture = IMAGES[TURRET_MEDIUM];
			SDL_QueryTexture(tempTurret.TurretTexture, NULL, NULL, &tempRect.w, &tempRect.h); tempTurret.DestRectTurret.w = tempRect.w; tempTurret.DestRectTurret.h = tempRect.h;
			tempTurret.dist = -34;
			ShipTurrets.push_back(tempTurret);
			tempTurret.dist = 20;
			ShipTurrets.push_back(tempTurret);
			tempTurret.dist = 38;
			ShipTurrets.push_back(tempTurret);
			tempTurret.TurretTexture = IMAGES[TURRET_TORPEDO];
			SDL_QueryTexture(tempTurret.TurretTexture, NULL, NULL, &tempRect.w, &tempRect.h); tempTurret.DestRectTurret.w = tempRect.w; tempTurret.DestRectTurret.h = tempRect.h;
			tempTurret.dist = 8;
			ShipTurrets.push_back(tempTurret);
			tempTurret.dist = 10;
			ShipTurrets.push_back(tempTurret);
			break;
		case SHIP_BATTLESHIP:
			hp = 2000; topspeed = 5.0; speed = 0.0; turretrot = 1.0; shiptraverse = 0.0; shiprot = 1.0; ShipTexture = IMAGES[SHIP_BIG], DeadTexture = IMAGES[SHIP_BIG_DEAD], dot=0, shotcooldown = 15;
			tempTurret.TurretTexture = IMAGES[TURRET_BIG];
			SDL_QueryTexture(tempTurret.TurretTexture, NULL, NULL, &tempRect.w, &tempRect.h); tempTurret.DestRectTurret.w = tempRect.w; tempTurret.DestRectTurret.h = tempRect.h;
			tempTurret.dist = -42;
			ShipTurrets.push_back(tempTurret);
			tempTurret.dist = 10;
			ShipTurrets.push_back(tempTurret);
			tempTurret.dist = 30;
			ShipTurrets.push_back(tempTurret);
			break;
		}
		SDL_QueryTexture(ShipTexture, NULL, NULL, &DestRectShip.w, &DestRectShip.h);
		shipCenterX = randx; shipCenterY = randy;
		DestRectShip.x = shipCenterX - DestRectShip.w / 2; DestRectShip.y = shipCenterY - DestRectShip.h / 2;
		shipAx = randdir;
		turretAx = 0.0;
		shipCollRange = DestRectShip.w / 2;
		IsAlive = true;
	}

	int GetCenterX() { return shipCenterX; }; int GetCenterY() { return shipCenterY; }; int GetCollRange() { return shipCollRange; }; int GetHP() { return hp; }; double GetAX() { return shipAx; };

	void TakeDamage(int dmg) { hp -= dmg; };

	void Render_Ship()
	{
		if(IsAlive)	SDL_RenderCopyEx(Main_Renderer, ShipTexture, NULL, &DestRectShip, shipAx + shiptraverse * angle_mod, NULL, SDL_FLIP_NONE);
		else SDL_RenderCopyEx(Main_Renderer, DeadTexture, NULL, &DestRectShip, shipAx + shiptraverse * angle_mod, NULL, SDL_FLIP_NONE);
		switch (shipclass)
		{
		case SHIP_DESTROYER: Render_DESTROYER(); break;
		case SHIP_CRUISER: Render_CRUISER(); break;
		case SHIP_BATTLESHIP: Render_BATTLESHIP(); break;
		}
	};

	void Update_Player()
	{
		actiontime = time(NULL);
		if (IsAlive)
		{
			Keyboard_State = SDL_GetKeyboardState(NULL);
			//	needs tweaking
			if (Keyboard_State[SDL_SCANCODE_W]) { if (speed < topspeed)speed += 0.5; }
			else { if (speed > 0.0)speed -= 0.5; }
			if (Keyboard_State[SDL_SCANCODE_S]) { if (speed > -1.0*topspeed)speed -= 0.5; }
			else { if (speed < -0.1)speed += 0.5; }
			//
			if (Keyboard_State[SDL_SCANCODE_D]) { if (shiptraverse < shiprot)shiptraverse += 0.1; }
			else { if (shiptraverse > 0.1)shiptraverse -= 0.1; }
			if (Keyboard_State[SDL_SCANCODE_A]) { if (shiptraverse > -1.0*shiprot)shiptraverse -= 0.1; }
			else { if (shiptraverse < -0.1)shiptraverse += 0.1; }

			if (Keyboard_State[SDL_SCANCODE_RIGHT]) turretAx += turretrot;
			if (Keyboard_State[SDL_SCANCODE_LEFT]) turretAx -= turretrot;
			if (Keyboard_State[SDL_SCANCODE_UP]) Fire();

			shipAx += shiptraverse;
			if (shipAx < -360) shipAx = 0;
			else if (shipAx > 360) shipAx = 0;

			if (turretAx < -360) turretAx = 0;
			else if (turretAx > 360) turretAx = 0;

			shipCenterX += speed * sin(-1 * shipAx*PI / 180)*speed_mod;
			shipCenterY += speed * cos(-1 * shipAx*PI / 180)*speed_mod;

			if (shipCenterX > MAP_WIDTH)shipCenterX = MAP_WIDTH; else if (shipCenterX < 0)shipCenterX = 0;
			if (shipCenterY > MAP_HEIGHT)shipCenterY = MAP_HEIGHT; else if (shipCenterY < 0)shipCenterY = 0;

			Camera_Rect.x = shipCenterX - CAMERA_WIDTH / 2; Camera_Rect.y = shipCenterY - CAMERA_HEIGHT / 2;

			if (Camera_Rect.x < 0)Camera_Rect.x = 0; if (Camera_Rect.y < 0)Camera_Rect.y = 0;
			if (Camera_Rect.x > MAP_WIDTH - Camera_Rect.w)Camera_Rect.x = MAP_WIDTH - Camera_Rect.w; if (Camera_Rect.y > MAP_HEIGHT - Camera_Rect.h)Camera_Rect.y = MAP_HEIGHT - Camera_Rect.h;

			DestRectShip.x = shipCenterX - DestRectShip.w / 2 - Camera_Rect.x; DestRectShip.y = shipCenterY - DestRectShip.h / 2 - Camera_Rect.y;
			
			hp-=dot;
			
			if (hp <= 0)IsAlive = false;

			cout << hp << " " << actiontime-checktime << " ";
			//cout << shipCenterX << " " << shipCenterY << " ";
		}
		else
		{
			if (speed > 0.0)speed -= 0.5; else if (speed < -0.1)speed += 0.5;
			
			if (shiptraverse > 0.1)shiptraverse -= 0.1; else if (shiptraverse < -0.1)shiptraverse += 0.1;

			shipAx += shiptraverse;
			if (shipAx < -360) shipAx = 0;
			else if (shipAx > 360) shipAx = 0;

			if (turretAx < -360) turretAx = 0;
			else if (turretAx > 360) turretAx = 0;

			shipCenterX += speed * sin(-1 * shipAx*PI / 180)*speed_mod;
			shipCenterY += speed * cos(-1 * shipAx*PI / 180)*speed_mod;

			if (shipCenterX > MAP_WIDTH)shipCenterX = MAP_WIDTH; else if (shipCenterX < 0)shipCenterX = 0;
			if (shipCenterY > MAP_HEIGHT)shipCenterY = MAP_HEIGHT; else if (shipCenterY < 0)shipCenterY = 0;

			Camera_Rect.x = shipCenterX - CAMERA_WIDTH / 2; Camera_Rect.y = shipCenterY - CAMERA_HEIGHT / 2;

			if (Camera_Rect.x < 0)Camera_Rect.x = 0; if (Camera_Rect.y < 0)Camera_Rect.y = 0;
			if (Camera_Rect.x > MAP_WIDTH - Camera_Rect.w)Camera_Rect.x = MAP_WIDTH - Camera_Rect.w; if (Camera_Rect.y > MAP_HEIGHT - Camera_Rect.h)Camera_Rect.y = MAP_HEIGHT - Camera_Rect.h;

			DestRectShip.x = shipCenterX - DestRectShip.w / 2 - Camera_Rect.x; DestRectShip.y = shipCenterY - DestRectShip.h / 2 - Camera_Rect.y;
		}
	};

	void Update_AI(Ship &Player)
	{
		actiontime = time(NULL);
		if (IsAlive&&Player.IsAlive)
		{
			//react in terms of distance
			if (Is_Inrange(Player.shipCenterX, Player.shipCenterY, shipCenterX, shipCenterY, 450))
			{
				targetAx = Get_Target_Angle(Player.shipCenterX, Player.shipCenterY, shipCenterX, shipCenterY);
				if (Is_Inrange(Player.shipCenterX, Player.shipCenterY, shipCenterX, shipCenterY, 250)) { if (speed > 0.0)speed -= 0.2; }
				else { if (speed < topspeed)speed += 0.1; }

				if (targetAx > shipAx && targetAx < shipAx + 180.0) { if (shiptraverse < shiprot)shiptraverse += 0.1; }
				else { if (shiptraverse > 0.1)shiptraverse -= 0.1; }
				if ((targetAx < shipAx && targetAx > shipAx - 180.0)) { if (shiptraverse > -1.0*shiprot)shiptraverse -= 0.1; }
				else { if (shiptraverse < -0.1)shiptraverse += 0.1; }


				if (targetAx > turretAx && targetAx < turretAx + 180.0) turretAx += turretrot;
				if (targetAx < shipAx && targetAx > shipAx - 180.0) turretAx -= turretrot;
				
				if (turretAx > targetAx - 10.0 && turretAx < targetAx + 10.0) Fire();
			}
			else
			{
				if (speed > 0.0)speed -= 0.1;
			}

			shipAx += shiptraverse;
			if (shipAx < -360) shipAx = 0;
			else if (shipAx > 360) shipAx = 0;

			if (turretAx < -360) turretAx = 0;
			else if (turretAx > 360) turretAx = 0;

			shipCenterX += speed * sin(-1 * shipAx*PI / 180)*speed_mod;
			shipCenterY += speed * cos(-1 * shipAx*PI / 180)*speed_mod;

			if (shipCenterX > MAP_WIDTH)shipCenterX = MAP_WIDTH; else if (shipCenterX < 0)shipCenterX = 0;
			if (shipCenterY > MAP_HEIGHT)shipCenterY = MAP_HEIGHT; else if (shipCenterY < 0)shipCenterY = 0;

			DestRectShip.x = shipCenterX - DestRectShip.w / 2 - Camera_Rect.x; DestRectShip.y = shipCenterY - DestRectShip.h / 2 - Camera_Rect.y;
			
			hp-=dot;

			if (hp <= 0)IsAlive = false;
		}
		else
		{
			if (speed > 0.0)speed -= 0.5; else if (speed < -0.1)speed += 0.5;

			if (shiptraverse > 0.1)shiptraverse -= 0.1; else if (shiptraverse < -0.1)shiptraverse += 0.1;

			shipAx += shiptraverse;
			if (shipAx < -360) shipAx = 0;
			else if (shipAx > 360) shipAx = 0;

			if (turretAx < -360) turretAx = 0;
			else if (turretAx > 360) turretAx = 0;

			shipCenterX += speed * sin(-1 * shipAx*PI / 180)*speed_mod;
			shipCenterY += speed * cos(-1 * shipAx*PI / 180)*speed_mod;

			if (shipCenterX > MAP_WIDTH)shipCenterX = MAP_WIDTH; else if (shipCenterX < 0)shipCenterX = 0;
			if (shipCenterY > MAP_HEIGHT)shipCenterY = MAP_HEIGHT; else if (shipCenterY < 0)shipCenterY = 0;

			DestRectShip.x = shipCenterX - DestRectShip.w / 2 - Camera_Rect.x; DestRectShip.y = shipCenterY - DestRectShip.h / 2 - Camera_Rect.y;
		}
	}
	
	friend void Collide(Ship &Unit1, Ship &Unit2);

	void Render_Turret(int i, double dir, double angle)
	{
		ShipTurrets[i].CenterX = DestRectShip.x + DestRectShip.w / 2 + ShipTurrets[i].dist * sin(-1 * dir*PI / 180); ShipTurrets[i].CenterY = DestRectShip.y + DestRectShip.h / 2 + ShipTurrets[i].dist * cos(-1 * dir*PI / 180);
		ShipTurrets[i].DestRectTurret.x = ShipTurrets[i].CenterX - ShipTurrets[i].DestRectTurret.w / 2; ShipTurrets[i].DestRectTurret.y = ShipTurrets[i].CenterY - ShipTurrets[i].DestRectTurret.h / 2;
		SDL_RenderCopyEx(Main_Renderer, ShipTurrets[i].TurretTexture, NULL, &ShipTurrets[i].DestRectTurret, angle, NULL, SDL_FLIP_NONE);
	};

	void Render_Torpedo_Turret(int i, double dir, double moddir, double angle)
	{
		ShipTurrets[i].CenterX = DestRectShip.x + DestRectShip.w / 2 + ShipTurrets[i].dist * sin(-1 * (dir + moddir)*PI / 180); ShipTurrets[i].CenterY = DestRectShip.y + DestRectShip.h / 2 + ShipTurrets[i].dist * cos(-1 * (dir + moddir)*PI / 180);
		ShipTurrets[i].DestRectTurret.x = ShipTurrets[i].CenterX - ShipTurrets[i].DestRectTurret.w / 2; ShipTurrets[i].DestRectTurret.y = ShipTurrets[i].CenterY - ShipTurrets[i].DestRectTurret.h / 2;
		SDL_RenderCopyEx(Main_Renderer, ShipTurrets[i].TurretTexture, NULL, &ShipTurrets[i].DestRectTurret, angle, NULL, SDL_FLIP_NONE);
	};

	void Render_DESTROYER()
	{
		Render_Turret(0, shipAx + shiptraverse * angle_mod, turretAx);
		Render_Turret(1, shipAx + shiptraverse * angle_mod, turretAx);
		Render_Turret(2, shipAx + shiptraverse * angle_mod, turretAx);
	}

	void Render_CRUISER()
	{
		Render_Turret(0, shipAx + shiptraverse * angle_mod, turretAx);
		Render_Turret(1, shipAx + shiptraverse * angle_mod, turretAx);
		Render_Turret(2, shipAx + shiptraverse * angle_mod, turretAx);
		Render_Torpedo_Turret(3, shipAx + shiptraverse * angle_mod, 90, turretAx);
		Render_Torpedo_Turret(4, shipAx + shiptraverse * angle_mod, -90, turretAx);
	}

	void Render_BATTLESHIP()
	{
		Render_Turret(0, shipAx + shiptraverse * angle_mod, turretAx);
		Render_Turret(1, shipAx + shiptraverse * angle_mod, turretAx);
		Render_Turret(2, shipAx + shiptraverse * angle_mod, turretAx);
	}

	void Fire()
	{
		if (actiontime - checktime >= shotcooldown)
		{
			switch (shipclass)
			{
			case SHIP_DESTROYER:
				for (int f = 0; f < 2; f++) { Shot newshot = Create_Shot(turretAx + (rand() % 11 - 5)*acc_mod, 100, 20, 600, ShipTurrets[f].CenterX + Camera_Rect.x, ShipTurrets[f].CenterY + Camera_Rect.y); Shots_To_Render.push_back(newshot); }
				break;
			case SHIP_CRUISER:
				for (int f = 0; f < 3; f++) for (int r = 0; r < 2; r++) { Shot newshot = Create_Shot(turretAx + (rand() % 11 - 5)*acc_mod, 100, 20, 800, ShipTurrets[f].CenterX + Camera_Rect.x, ShipTurrets[f].CenterY + Camera_Rect.y); Shots_To_Render.push_back(newshot); }
				break;
			case SHIP_BATTLESHIP:
				for (int f = 0; f < 3; f++) for (int r = 0; r < 3; r++) { Shot newshot = Create_Shot(turretAx + (rand() % 11 - 5)*acc_mod, 100, 20, 1000, ShipTurrets[f].CenterX + Camera_Rect.x, ShipTurrets[f].CenterY + Camera_Rect.y); Shots_To_Render.push_back(newshot); }
				break;
			}
			checktime = actiontime;
		}
	};
};

vector<Ship> Ships_To_Render;

bool Is_Collided(Ship &Unit1, Ship &Unit2)
{
	int Bot_X, Bot_Y, SHIP_X, SHIP_Y, R1 = Unit1.GetCollRange(), R2 = Unit2.GetCollRange();
	for (int i = 3; i >= -3; i--)
	{
		for (int j = 3; j >= -3; j--) 
		{ 
			Bot_X = Unit1.GetCenterX() + R1*sin(-1 * Unit1.GetAX()*PI / 180)*i; Bot_Y = Unit1.GetCenterY() + R1*cos(-1 * Unit1.GetAX()*PI / 180)*i;
			SHIP_X = Unit2.GetCenterX() + R2*sin(-1 * Unit2.GetAX()*PI / 180)*j; SHIP_Y = Unit2.GetCenterY() + R2*cos(-1 * Unit2.GetAX()*PI / 180)*j; 
			if (Is_Inrange(Bot_X, Bot_Y, SHIP_X, SHIP_Y, R1+R2)) return true;
		}
	}
	return false;
};

void Collide(Ship &Unit1, Ship &Unit2)
{
	double pushdir = Get_Target_Angle(Unit1.shipCenterX, Unit1.shipCenterY, Unit2.shipCenterX, Unit2.shipCenterY);
	Unit1.shipCenterX += 5 * sin(-1 * pushdir*PI / 180); Unit1.shipCenterY += 5 * cos(-1 * pushdir*PI / 180);
	Unit2.shipCenterX += 5 * sin(pushdir*PI / 180); Unit2.shipCenterY += 5 * cos(pushdir*PI / 180);
	Unit1.TakeDamage(Unit2.hp*Unit2.speed/ 100); Unit2.TakeDamage(Unit1.hp*Unit1.speed / 100);
	Unit1.dot++; Unit2.dot++;
}

bool Is_Hit(int B_X, int B_Y, int S_X, int S_Y, int R1, int R2, double A1, double A2)
{
	int BULLET_X, BULLET_Y, SHIP_X, SHIP_Y;
	for (int i = 3; i >= -3; i--)
	{
		for (int j = 3; j >= -3; j--)
		{
			BULLET_X = B_X + R1 * sin(-1 * A1*PI / 180)*i; BULLET_Y = B_Y + R1 * cos(-1 * A1*PI / 180)*i;
			SHIP_X = S_X + R2 * sin(-1 * A2*PI / 180)*j; SHIP_Y = S_Y + R2 * cos(-1 * A2*PI / 180)*j;
			if (Is_Inrange(BULLET_X, BULLET_Y, SHIP_X, SHIP_Y, R1 + R2)) return true;
		}
	}
	return false;
};

Ship Create_Ship()
{
	Ship new_ship(rand() % 3, rand() % 4000 + 500, rand() % 4000 + 500, rand() % 360);
	return new_ship;
};

int main(int argc, char* args[] )
{
	srand(time(NULL));
	//					START
	Init_SDL();

	bool Playing = true, Paused = true, Game_Running = true;
	double DirectorAim;
	SDL_Rect DirectorRect;
	SDL_Point DirectorRotPoint = { 5,5 };
	string thetext;
	SDL_Event Events;

	//setup
	Load_All_Images();
	
	SDL_QueryTexture(IMAGES[DIRECTOR], NULL, NULL, &DirectorRect.w, &DirectorRect.h);

	currenttime = time(NULL);

	Camera_Rect = {0,0,CAMERA_WIDTH,CAMERA_HEIGHT};

	for (int i = 0; i < rand()%6+5; i++) { Ship someship=Create_Ship(); Ships_To_Render.push_back(someship); }
	
	Ship Player_Ship(rand()%3);
	
	//run game
	while (Game_Running)
	{
		SDL_RenderClear(Main_Renderer);
		SDL_RenderCopy(Main_Renderer, IMAGES[BIG_MAP], &Camera_Rect, NULL);
		for (int d = 0; d < Ships_To_Render.size(); d++)
		{
			if (Ships_To_Render[d].GetHP() > 0)
			{
				DirectorAim = Get_Target_Angle(Ships_To_Render[d].GetCenterX(), Ships_To_Render[d].GetCenterY(), Player_Ship.GetCenterX(), Player_Ship.GetCenterY());
				DirectorRect.x = Player_Ship.GetCenterX() - Camera_Rect.x - 5; DirectorRect.y = Player_Ship.GetCenterY() - Camera_Rect.y - 5;
				SDL_RenderCopyEx(Main_Renderer, IMAGES[DIRECTOR], NULL, &DirectorRect, DirectorAim, &DirectorRotPoint, SDL_FLIP_NONE);
			}
		}
		Player_Ship.Render_Ship();
		for (int j = 0; j < Ships_To_Render.size(); j++) Ships_To_Render[j].Render_Ship();
		for (int n = 0; n < Shots_To_Render.size(); n++) Shots_To_Render[n].Render_Shot();
		SDL_RenderCopy(Main_Renderer, IMAGES[BIG_MAP_OVERLAY], &Camera_Rect, NULL);
		SDL_RenderPresent(Main_Renderer);

		SDL_StartTextInput();
		while (Paused)
		{
			while (SDL_PollEvent(&Events) != 0)
			{
				if (Events.type == SDL_KEYDOWN) 
				{
					if (Events.key.keysym.sym == SDLK_RETURN)
					{
						if (thetext == "dejavu") { switch (DEJA_VU) { case false: DEJA_VU = true; break; case true: DEJA_VU = false; break; } thetext = ""; }
						else if (thetext == "gottagofast") switch (GOTTA_GO_FAST) { { case false: GOTTA_GO_FAST = true; break; case true: GOTTA_GO_FAST = false; break; } thetext = ""; }
						else if (thetext == "stormtrooper") switch (STORMTROOPER) { { case false: STORMTROOPER = true; break; case true: STORMTROOPER = false; break; } thetext = ""; }
						else if (thetext == "") { Paused = false; Playing = true; }
						else { thetext = ""; Playing = true; }
					}
					else if (Events.key.keysym.sym == SDLK_ESCAPE) { Game_Running = false; Paused = false;  Playing = false; }
					else { Game_Running = true; Paused = true;  Playing = true; }
				}
				else if (Events.type == SDL_TEXTINPUT) thetext += Events.text.text;
				else if (Events.type == SDL_QUIT) { Game_Running = false; Paused = false;  Playing = false; }
			}
		}
		SDL_StopTextInput();
		
		if (DEJA_VU) angle_mod = 30; else angle_mod = 0;
		if (GOTTA_GO_FAST) speed_mod = 2; else speed_mod = 1;
		if (STORMTROOPER) acc_mod = 10; else acc_mod = 1;

		while (Playing)
		{
			currenttime = time(NULL);
			SDL_RenderClear(Main_Renderer);
			SDL_RenderCopy(Main_Renderer, IMAGES[BIG_MAP], &Camera_Rect, NULL);
			while (SDL_PollEvent(&Events) != 0)	if (Events.type == SDL_QUIT) { Game_Running = false; Playing = false; } else if (Events.type == SDL_KEYDOWN) if (Events.key.keysym.sym == SDLK_ESCAPE) { Paused = true; Playing = false; }

			Player_Ship.Update_Player();
			for (int d = 0; d < Ships_To_Render.size(); d++)
			{
				if(Ships_To_Render[d].GetHP()>0)
				{
					DirectorAim = Get_Target_Angle(Ships_To_Render[d].GetCenterX(), Ships_To_Render[d].GetCenterY(), Player_Ship.GetCenterX(), Player_Ship.GetCenterY());
					DirectorRect.x = Player_Ship.GetCenterX() - Camera_Rect.x - 5; DirectorRect.y = Player_Ship.GetCenterY() - Camera_Rect.y - 5;
					SDL_RenderCopyEx(Main_Renderer, IMAGES[DIRECTOR], NULL, &DirectorRect, DirectorAim, &DirectorRotPoint, SDL_FLIP_NONE);
				}
			}
			Player_Ship.Render_Ship();
			
			for (int j = 0; j < Ships_To_Render.size(); j++)
			{
				Ships_To_Render[j].Update_AI(Player_Ship);
				if (Is_Collided(Player_Ship, Ships_To_Render[j])) Collide(Player_Ship, Ships_To_Render[j]);
				for (int i = j + 1; i < Ships_To_Render.size(); i++) if (Is_Collided(Ships_To_Render[j], Ships_To_Render[i]))  Collide(Ships_To_Render[j], Ships_To_Render[i]);
				Ships_To_Render[j].Render_Ship();
			}

			for (int n = 0; n < Shots_To_Render.size(); n++)
			{
				Shots_To_Render[n].Update_Shot();
				if (Shots_To_Render[n].GetDist() < Shots_To_Render[n].GetDest() - 50)
				{
					if (Is_Hit(Shots_To_Render[n].GetCenterX(), Shots_To_Render[n].GetCenterY(), Player_Ship.GetCenterX(), Player_Ship.GetCenterY(), Shots_To_Render[n].GetCollRange(), Player_Ship.GetCollRange(), Shots_To_Render[n].GetAX(), Player_Ship.GetAX()))
					{
						Player_Ship.TakeDamage(Shots_To_Render[n].GetDamage());
						Shots_To_Render[n].Hit();
					};
					for (int m = 0; m < Ships_To_Render.size(); m++)
					{
						if (Is_Hit(Shots_To_Render[n].GetCenterX(), Shots_To_Render[n].GetCenterY(), Ships_To_Render[m].GetCenterX(), Ships_To_Render[m].GetCenterY(), Shots_To_Render[n].GetCollRange(), Ships_To_Render[m].GetCollRange(), Shots_To_Render[n].GetAX(), Ships_To_Render[m].GetAX()))
						{
							Ships_To_Render[m].TakeDamage(Shots_To_Render[n].GetDamage());
							Shots_To_Render[n].Hit();
						};
					}
					if (Shots_To_Render[n].GetStatus()) Shots_To_Render[n].Render_Shot();
					else { Shots_To_Render.erase(Shots_To_Render.begin() + n); n--; };
				}
			}

			cout << "\n";

			SDL_RenderCopy(Main_Renderer, IMAGES[BIG_MAP_OVERLAY], &Camera_Rect, NULL);
			SDL_RenderPresent(Main_Renderer);
			SDL_Delay(50);
		}
	}
	//					END
	Quit_SDL();
	return 0;
}