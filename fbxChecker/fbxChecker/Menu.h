#pragma once
#include "Reader.h"
#include "ExportFbx.h"
#include <iostream>
#include <string>

class Menu
{
public:
	Menu();

	~Menu();

	void init(bool fixFbx);

private:
	
	Reader* r;

	ExportFbx* e;

	std::string read;

	const char* filename;
};
