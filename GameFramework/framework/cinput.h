#pragma once
#ifndef __CINPUT_H_
#define __CINPUT_H_

#include "basecomponent.h"

enum InputMode
{
	WASD = 0,
	ARROWKEYS = 1
};

class CInput : public BaseComponent
{
public:
	CInput() {};

public:
	bool m_bUp{ false };
	bool m_bDown{ false };
	bool m_bLeft{ false };
	bool m_bRight{ false };
	bool m_bSpace{ false };
	bool m_bShift{ false };
	bool m_bLeftClick{ false };
	bool m_bRightClick{ false };
	int m_fWalkSpeed = 100;
	int m_fSprintSpeed = 200;
	InputMode m_eInputMode{ WASD };
};

#endif // __CINPUT_H_