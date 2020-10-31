#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

Player::Player() : Module()
{

}

Player::~Player()
{
	hero = app->tex->Load("Assets/textures/hero_idle.gif");
}

bool Player::Start()
{
	bool ret = true;

	return ret;
}

bool Player::Update()
{
	if (Godmode == false) 
	{
		//Gravity
		/*if (gravity == true)
			position.y -= speedy;*/

		//Mov left
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
			position.x += speedx;

		//Mov right
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
			position.x -= speedx;

		//Jump
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && gravity == false)
		{
			jump = true;
			maxJump = position.y;
		}
		if (jump == true && position.y < maxJump + 40)
		{
			position.y += (2 * speedy);
		}
	}

	//Godmode
	if (Godmode == true) 
	{
		gravity = false;

		//Mov left
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
			position.x += 1;

		//Mov right
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
			position.x -= 1;

		//Mov up
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE)
			position.y += 1;

		//Mov down
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE)
			position.y -= 1;
	}

	return true;
}

/*bool Player::PostUpdate()
{
	return true;
}*/

bool Player::CleanUp()
{
	return true;
}
	