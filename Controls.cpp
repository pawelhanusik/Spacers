#include "Controls.h"

Controls::Controls()
{
	m_buffer = 0;
	m_running = true;
	keyPressThread = std::thread(keyPressListener, &m_running, &m_buffer);
}
Controls::~Controls()
{
	m_buffer = 0;
	m_running = false;
	keyPressThread.join();
}

int Controls::addElementToControl(Controllable * controllable)
{
	//toControl.push_back(controllable);
	//return toControl.size() - 1;
	static int newControlID = -1;
	newControlID++;
	toControl[newControlID] = controllable;
	return newControlID;
}

void Controls::removeElementToControl(int idToRemove)
{
	//toControl.erase(toControl.begin() + idToRemove);
	toControl.erase(idToRemove);
}

char Controls::getKey()
{
	char ret = m_buffer;
	m_buffer = 0;
	return ret;
}

void Controls::controllEverything()
{
	char keyPressed = this->getKey();
	for (std::map<int, Controllable*>::const_iterator it = toControl.begin(); it != toControl.end(); ++it)
	{
		(*it).second->control(keyPressed);
	}
}

void Controls::keyPressListener(bool *running, volatile char *buffer)
{
	while (*running) {
		while (*buffer != 0);
		if(_kbhit())
			*buffer = _getch();
	}
}
