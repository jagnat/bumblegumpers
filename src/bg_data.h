#pragma once
#ifndef _BG_DATA_H_
#define _BG_DATA_H_

#endif

#include "primitives.h"

union Color32
{
	struct
	{
		uint8 r, g, b, a;
	};
	struct
	{
		uint8 elements[4];
	};
	uint element;
};

inline Color32 WithAlpha(Color32 c, float alpha)
{
	Color32 r = {c.r, c.g, c.b, (uint8)(alpha * 255)};
	return r;
}

static const Color32 COLOR_WHITE = {255, 255, 255, 255};
static const Color32 COLOR_RED = {255, 0, 0, 255};
static const Color32 COLOR_GREEN = {0, 255, 0, 255};
static const Color32 COLOR_BLUE = {0, 0, 255, 255};
static const Color32 COLOR_BLACK = {0, 0, 0, 255};

enum CameraMode
{
	CAM_GAME,
	CAM_SCREEN
};

struct Texture
{
	uint glId;
	int width, height;
	int bytesPerPixel;
	bool linearBlend;
	uint8 *pixels;
};

union Vertex;
struct SpriteBatch;

struct SpriteBatch
{
	union
	{
		struct
		{
			uint vboId;
			uint iboId;
			uint vaoId;
		};
		uint ids[3];
	};
	uint totalVertices;
	uint totalIndices;
	uint populatedVertices;
	Vertex *vertices;
	float z;
	Color32 col;
};

struct Renderer
{
	bool initialized;
	uint width, height;

	Mat4 projMatrix;
	Mat4 viewMatrix;
	Mat4 screenMatrix;

	float tilePixels;
	float zoom;
#define TEXTURE_EMPTY 0
#define TEXTURE_TILESHEET 1
#define TEXTURE_FONT 2
	Texture textures[16];

	uint programId;
	int transformLocation;
	int textureLocation;
	SpriteBatch spritebatch;
};

struct Button
{
	bool pressed;
	bool released;
	bool down;
};

union ModState
{
	struct
	{
		bool control;
		bool shift;
		bool alt;
	};
	bool mods[3];
};

struct Input
{
	union
	{
		struct
		{
			// Gameplay buttons
			Button left, right, up, down;
			Button attack, use, shield;

			// Editor buttons
			Button toggleEdit;

			// Mouse
			Button leftMouse, rightMouse, middleMouse;
		};
		#define NUM_BUTTONS 16
		Button buttons[NUM_BUTTONS];
	};
	ModState modState;

	// NOTE: for imgui
	// TODO: remove or change
	bool keysDown[512];

	// TODO: expand this if we overflow
	#define INPUT_TEXT_BUFFER_LENGTH 256
	char textBuffer[INPUT_TEXT_BUFFER_LENGTH];

	int mouseX, mouseY;
};

enum PlayerState
{
	PLAYER_ONGROUND,
	PLAYER_AIRWALKING,
	PLAYER_JUMPING,
	PLAYER_CLIMBING,
	PLAYER_FALLING
};

struct Player
{
	int x, y;
	PlayerState state;
};

enum TileCollision : uint8
{
	TC_EMPTY = 0,
	TC_SOLID = 1,
	TC_LADDER = 2,
	TC_CORNER = 3,
	TC_PLATFORM = 4
};

struct World
{
	char id[64];
	uint width, height;
	TileCollision *collision;
	uint16 *bTiles;
	uint16 *mTiles;
	uint16 *fTiles;
};

enum EditMode : int
{
	MODE_BACKGROUND = 0,
	MODE_MAIN = 1,
	MODE_FOREGROUND = 2,
	MODE_COLLISION = 3
};

// In ImGui, ###label allows both text modes to have same ID
char blockText[] = "Picker: Block Mode###selector";
char collisionText[] = "Picker: Collision Mode###selector";
struct Editor
{
	World *world;
	Vec2 camPos;
	Vec2 cursorPos;

	EditMode editMode;
	char *editText;
	uint16 editId;

	uint screenW, screenH;
};


struct Game
{
	bool inEditor;

	World world;
	Player player;

	Input input;
};

struct Platform
{
	SDL_Window *window;
	SDL_GLContext glContext;
	double targetUpdateDelta;
	double targetRenderDelta;
	double timerResolution;
	uint width, height;
	bool running;

	Renderer renderer;
	Game game;
	Editor editor;
};
