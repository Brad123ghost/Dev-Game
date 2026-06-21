#pragma once
#ifndef __CSOUND_H_
#define __CSOUND_H_

#include "basecomponent.h"
#include <string>

class CSound : public BaseComponent
{
	std::string soundName;
};

#endif // __CSOUND_H_