#include <iostream>
#include <stdio.h>
#include <time.h>
#include <thread>
#include <vector>
#include <memory>

#include "Controls.h"
#include "Math.h"

#include "Graphics\Screen.h"
#include "Graphics\Gui.h"
#include "Graphics\Menu.h"

#include "Entities\Player.h"
#include "Entities\Enemy.h"
#include "Entities\Boss.h"

#include "Physics\Physics.h"
#include "Physics\Collision.h"
#include "Utils.h"

#pragma comment(lib, "winmm.lib")

const int targetFPS = 30;

bool running = true;

//int splash();
int splash(const char* imgPath);
int mainLoop();
int bossLoop();
void gameOver();
void gameWon();
int menu();
void controls();
void credits();


int main()
{
	bool running = true;
	do {
		PlaySound("Resources/Basic Implosion.wav", NULL, SND_ASYNC);
		splash("Resources/tlo.bmp");
	
		switch (menu()) {
		case 0:
			if (mainLoop() == 0)
			{
				Utils::gotoxy(0, 0);
				PlaySound("Resources/Summon the Rawk.wav", NULL, SND_ASYNC);
				if (bossLoop() == 0)
				{
					PlaySound(NULL, NULL, SND_ASYNC);
					gameWon();
					running = false;
				}
				else
				{
					gameOver();
				}
			}
			else
			{
				gameOver();
			}
			break;
		case 1:
			controls();
			break;
		case 2:
			credits();
			break;
		case 3:
			running = false;
			break;
		}
	} while (running);
	
	PlaySound(NULL, NULL, SND_ASYNC);
	return 0;
}

void controls()
{
	Screen screen;
	std::string guiLook = "       Controls\n"
		"       ````````\n"
		"    r -> reload the cannon\n"
		"space -> shoot\n"
		"\n"
		"    w -> go up\n"
		"    s -> go down\n"
		"    a -> go left\n"
		"    d -> go right\n";
	guiLook += " ";
	Gui g(guiLook, &screen, vec2( (screen.m_width - 26) / 2, 10));

	screen.draw();
	system("PAUSE");
}
void credits()
{
	Screen screen;
	std::string guiLook = "           Programming\n          Pawel Hanusik\n\n\n";
	guiLook += "     Graphics and 'Graphics'\n          Pawel Hanusik\n\n\n";
	guiLook += "             Music : \n\"Basic Implosion\" - Kevin MacLeod\n\"Summon the Rawk\" - Kevin MacLeod\n  \"Sad Romance (Violin Version)\"";
	Gui g(guiLook, &screen, vec2((screen.m_width - 33) / 2, 10));

	screen.draw();
	system("PAUSE");
}
int menu()
{
	static int selectedItem = -1;
	Screen screen;
	Controls controls;

	Gui gui(
		" ____     _____      ____      ____    _____    _____     ____\n/  __|   |  __ \\    / __ \\    / ___|  |  ___|  |  __ \\   /  __|\n| |      | |__> |  / |__| \\  / /      | |__    | |__> |  | |\n \\_ \\    |  ___/   |  __  |  | |      |  __|   |   __/    \\_ \\\n   | \\   | |       | (  ) |  | |      | |      |  _ \\       | \\\n __| |   | |       | |  | |  \\ \\___   | |___   | | \\ \\    __| |\n|____/   |_|       |_|  |_|   \\____|  |_____|  |_|  \\_\\  |___ /"
		, &screen, vec2((screen.m_width - 62) / 2, 7));
	Menu mainMenu([](int a)->void {
		selectedItem = a;
	}, &screen, &controls);
	mainMenu.setPos(vec2(0, 2));
	mainMenu.addElement("START");
	mainMenu.addElement("CONTROLS");
	mainMenu.addElement("CREDITS");
	mainMenu.addElement("EX.IT");

	mainMenu.makeVisible();
	time_t beforeTime, delayTime;
	while (selectedItem == -1) {
		time(&beforeTime);

		screen.clear();
		controls.controllEverything();
		screen.draw();

		delayTime = 1000 / targetFPS + beforeTime - time(NULL);
		if (delayTime > 0) {
			clock_t time_end;
			time_end = clock() + delayTime * CLOCKS_PER_SEC / 1000;
			while (clock() < time_end);
		}
	}
	int ret = selectedItem;
	selectedItem = -1;
	return ret;
}
void gameWon()
{
	PlaySound(NULL, NULL, SND_ASYNC);
	splash("Resources/win.bmp");
	PlaySound("Resources/Slow Clap.wav", NULL, SND_SYNC);
}
void gameOver()
{
	static bool wasShowed = false;

	PlaySound(NULL, NULL, SND_ASYNC);
	
	if (!wasShowed) {
		splash("Resources/lose.bmp");
		PlaySound("Resources/Sad Romance (Violin Version).wav", NULL, SND_ASYNC);

		splash("Resources/lose2.bmp");
		time_t time_end = clock() + 2000 * CLOCKS_PER_SEC / 1000; while (clock() < time_end);
		splash("Resources/lose3.bmp");
		time_end = clock() + 2000 * CLOCKS_PER_SEC / 1000; while (clock() < time_end);
		splash("Resources/lose4.bmp");
		time_end = clock() + 2000 * CLOCKS_PER_SEC / 1000; while (clock() < time_end);
		splash("Resources/lose5.bmp");
		time_end = clock() + 2000 * CLOCKS_PER_SEC / 1000; while (clock() < time_end);
		splash("Resources/lose6.bmp");
		time_end = clock() + 2000 * CLOCKS_PER_SEC / 1000; while (clock() < time_end);
		splash("Resources/lose7.bmp");
		time_end = clock() + 5000 * CLOCKS_PER_SEC / 1000; while (clock() < time_end);

		wasShowed = true;
	}
	else {
		PlaySound("Resources/Sad Romance (Violin Version).wav", NULL, SND_ASYNC);
		splash("Resources/lose.bmp");
		time_t time_end = clock() + 2000 * CLOCKS_PER_SEC / 1000; while (clock() < time_end);
	}
}
int bossLoop()
{
	Screen screen;
	Controls controls;
	Physics physics;

	srand(time(NULL));
	///////////////////////////////////////////

	Player *player = new Player("==\\\n|-->\n==/", &screen, &controls, &physics);
	player->teleportTo(vec2(3, 14));

	Gui gui("Health: 100% Ammo: |||||",
		&screen, vec2(0, screen.getHeight() - 1));

	Boss *boss = new Boss(&screen, &physics, vec2(100, 12), &player->m_pos);

	///////////////////////////////////////////


	running = true;
	time_t beforeTime, delayTime;
	while (running && boss->m_hp > 0 && player->m_hp > 0)
	{
		time(&beforeTime);


		screen.clear();
		controls.controllEverything();
		physics.update();

		std::string guiLook = " Health: ";
		for (int i = 0; i < player->m_hp / 10; ++i) guiLook += (char)219;
		for (int i = player->m_hp / 10; i < 10; ++i) guiLook += ' ';
		guiLook += "     Ammo: ";
		for (int i = 0; i < player->m_weaponAmmo; ++i) guiLook += (char)219;
		for (int i = player->m_weaponAmmo; i < screen.m_width - 55; ++i) guiLook += ' ';
		guiLook += "        Boss: ";
		for (int i = 0; i < boss->m_hp / 10; ++i) guiLook += (char)219;
		gui.m_look = guiLook;

		screen.update();
		screen.draw();


		delayTime = 1000 / targetFPS + beforeTime - time(NULL);
		if (delayTime > 0) {
			clock_t time_end;
			time_end = clock() + delayTime * CLOCKS_PER_SEC / 1000;
			while (clock() < time_end);
		}
	}
	running = false;

	int ret = 0;
	if (player->m_hp <= 0){
		ret = 1;
		player->explode();
	}
	else {
		boss->explode();
	}

	delete boss;
	delete player;

	return ret;
}
int mainLoop()
{
	//after winning this lvl for 5 times, this lvl will be skipped
	static int timesWonThisLvl = 0;
	if (timesWonThisLvl >= 3) return 0;

	Screen screen;
	Controls controls;
	Physics physics;

	srand(time(NULL));
	///////////////////////////////////////////
	
	Player *player = new Player("==\\\n|-->\n==/", &screen, &controls, &physics);
	player->teleportTo(vec2(3, 14));
	
	Gui gui("Health: 100% Ammo: |||||",
		&screen, vec2(0, screen.getHeight()-1));
	
	for(int i = 0; i < 200; i++)
		new Enemy(&screen, &physics, vec2(715 - i*i/2, rand() % (BasicScreen::m_height-5) + 1), &player->m_pos);

	///////////////////////////////////////////

	int score = 0;
	running = true;
	time_t beforeTime, delayTime;
	while (running && score < 800 && player->m_hp > 0)
	{
		time(&beforeTime);
		

		screen.clear();
		controls.controllEverything();
		physics.update();
		
		std::string guiLook = " Health: ";
		for (int i = 0; i < player->m_hp / 10; ++i) guiLook += (char)219;
		for (int i = player->m_hp / 10; i < 10; ++i) guiLook += ' ';
		guiLook += "     Ammo: ";
		for (int i = 0; i < player->m_weaponAmmo; ++i) guiLook += (char)219;
		score++;
		gui.m_look = guiLook;

		screen.update();
		screen.draw();

		delayTime = 1000/targetFPS + beforeTime - time(NULL);
		if (delayTime > 0) {
			clock_t time_end;
			time_end = clock() + delayTime * CLOCKS_PER_SEC / 1000;
			while (clock() < time_end);
		}
	}
	running = false;


	int ret = 0;
	if (player->m_hp <= 0) ret = 1;
	delete player;

	if (ret == 0) timesWonThisLvl++;

	return ret;
}
int splash(const char* imgPath)
{
	/*GraphicScreen gScreen;
	Picture splash(vec2(0, 0), gScreen.getWidth(), gScreen.getHeight(),
		Utils::loadBMP_custom(imgPath));	
	gScreen.placeOnScreen(&splash);
	gScreen.draw();*/
	
	GraphicScreen gs;
	Picture *pic = gs.BMP2Pic(imgPath);
	GraphicScreen::img_resize(pic, gs.m_width, gs.m_height);
	gs.splash(pic);

	return 0;
}
/*int splash()
{
	GraphicScreen gScreen;
	Picture splash(vec2(0, 0), gScreen.getWidth(), gScreen.getHeight());
	float f = 0.01;
	for (int i = 0; i < gScreen.getWidth(); ++i) {
		for (int j = 0; j < gScreen.getHeight(); ++j) {
			int tmpI, tmpJ;
			if (i < gScreen.getWidth()/2)
				tmpI = i;
			else
				tmpI = (gScreen.getWidth() - i);
			if (j < gScreen.getHeight()/2 + 100)
				tmpJ = j;
			else
				tmpJ = (gScreen.getHeight() - j);

			splash.setPixel(vec2(i, j), f*tmpI, f*tmpJ, f*tmpI*tmpJ/9);
		}
	}
	//splash.pixels = Utils::loadBMP_custom("Resources/tlo.bmp");
	gScreen.placeOnScreen(&splash);
	gScreen.draw();

	return 0;
}*/