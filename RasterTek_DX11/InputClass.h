#pragma once

#ifndef INPUTCLASS_H
#define INPUTCLASS_H

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	void Initialise();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_Keys[256];
};

#endif
