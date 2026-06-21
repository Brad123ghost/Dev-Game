#pragma once
#ifndef __CINVENTORY_H_
#define __CINVENTORY_H_

#include "basecomponent.h"
#include <string>
#include <map>

struct Item
{
	std::string itemName;
	std::string itemDescription;
	std::string itemIconPath;
	int quantity;
	int inventoryPos;
};

class CInventory : public BaseComponent
{
public:
	std::map<std::string, Item> items;
	int size;
	int rows;
	CInventory(int inventorySize = 9, int inventoryRows = 1)
	{
		size = inventorySize;
		rows = inventoryRows;
	}
};
#endif // __CINVENTORY_H_