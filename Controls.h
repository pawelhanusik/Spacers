#pragma once

#include <thread>
#include <conio.h>
#include <map>


class Controllable{
public:
	virtual void control(char keyPressed) = 0;
};

class Controls {
	bool m_running;
	std::thread keyPressThread;
	char m_buffer;
	std::map<int, Controllable*> toControl;
public:

	Controls();
	~Controls();

	int addElementToControl(Controllable *controllable);
	void removeElementToControl(int idToRemove);

	char getKey();
	void controllEverything();
private:
	static void keyPressListener(bool *running, volatile char *buffer);
};