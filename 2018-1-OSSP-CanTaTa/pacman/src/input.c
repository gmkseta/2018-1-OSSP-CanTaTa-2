#include "input.h"

//does a check to ensure the given keycode is within the bounds of valid keys
//stops the program with an error if it is out of bounds
static void check_keycode(int keycode);

//gets the frame this direction was pressed.
//Uses both wasd and arrow keys.
static int frame_for_direction(Direction dir);

static bool keysHeld[MAX_KEYS] = {false};

static unsigned int keysPressedFrame[MAX_KEYS] = {0};
static unsigned int keysReleasedFrame[MAX_KEYS] = {0};

static unsigned int curKeyFrame = 1;

void keyevents_finished(void)
{
	curKeyFrame++;
}

void handle_keydown(int keycode)
{
	check_keycode(keycode);

	if (!keysHeld[keycode]) keysPressedFrame[keycode] = curKeyFrame;

	keysHeld[keycode] = true;
}

void handle_keyup(int keycode)
{
	check_keycode(keycode);

	if (keysHeld[keycode]) keysReleasedFrame[keycode] = curKeyFrame;

	keysHeld[keycode] = false;
}

bool dir_key_held(Direction direction,int player_num)
{
	if (player_num==0)
	{
		switch (direction)
		{
			case Up:    return keysHeld[SDLK_UP];
			case Down:  return keysHeld[SDLK_DOWN];
			case Left:  return keysHeld[SDLK_LEFT];
			case Right: return keysHeld[SDLK_RIGHT];
		}
	}
	else
	{
		switch (direction)
				{
					case Up:    return keysHeld[SDLK_w];
					case Down:  return keysHeld[SDLK_s];
					case Left:  return keysHeld[SDLK_a];
					case Right: return keysHeld[SDLK_d];
				}
	}

	printf("should never reach here\n");
	exit(1);
}

bool dir_pressed_now(Direction *dir,int player_num)
{
	int highestPushed = 0;

	Direction dirs[4] = {Up, Left, Down, Right};

	for (int i = 3; i >= 0; i--)
	{
		if (!dir_key_held(dirs[i],player_num))continue;

		int x = frame_for_direction(dirs[i]);

		if (x > highestPushed)
		{
			*dir = dirs[i];
			highestPushed = x;
		}
	}

	return highestPushed != 0;
}

bool key_held(int keycode)
{
	check_keycode(keycode);

	return keysHeld[keycode];
}

bool key_pressed(int keycode)
{
	check_keycode(keycode);

	return keysPressedFrame[keycode] == (curKeyFrame - 1);
}

bool key_released(int keycode)
{
	check_keycode(keycode);//#19 : 1. 이 부분에서 키 값 계속 넘어가버려서 흠.. 체크 한뒤 false 바로만들기...
	//안그러면 true 상태라 menu 에서 online 체크 후 바로 make room 들어가져버림
	bool rt =keysReleasedFrame[keycode] == (curKeyFrame - 1);
	keysReleasedFrame[keycode]=false;

	return rt;
}

#define max(a, b) (a) > (b) ? (a) : (b)
#define min(a, b) max((b), (a))

static int frame_for_direction(Direction dir)
{
	switch (dir)
	{
		case Up:    return max(keysPressedFrame[SDLK_UP]   , keysPressedFrame[SDLK_w]);
		case Down:  return max(keysPressedFrame[SDLK_DOWN] , keysPressedFrame[SDLK_s]);
		case Left:  return max(keysPressedFrame[SDLK_LEFT] , keysPressedFrame[SDLK_a]);
		case Right: return max(keysPressedFrame[SDLK_RIGHT], keysPressedFrame[SDLK_d]);
	}

	printf("should never reach here\n");
	exit(1);
}

static void check_keycode(int keycode)
{
	if (keycode >= MAX_KEYS)
	{
		printf("Keycode %d is out of range.\n", keycode);
		printf("Aborting\n");
		exit(1);
	}
}
