#pragma once
#ifndef __CINPUT_H_
#define __CINPUT_H_

#include "basecomponent.h"

class CInput : public BaseComponent
{
public:
	CInput() {};

public:
	bool m_bUp{ false };
	bool m_bDown{ false };
	bool m_bLeft{ false };
	bool m_bRight{ false };
	bool m_bLeftClick{ false };
	bool m_bRightClick{ false };

};

#endif // __CINPUT_H_