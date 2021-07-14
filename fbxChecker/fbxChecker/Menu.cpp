#include "Menu.h"

#include <filesystem>
#include <stack>
#include "Output.h"
//C++17
namespace fs = std::filesystem;

Menu::Menu()
{
	r = new Reader();
	e = new ExportFbx();
}

Menu::~Menu()
{
}

void Menu::init(bool fixFbx)
{

	//Check the .exe folder and put in a string's stack the fbx object's names
	std::stack<std::string> s;
	for (const auto& entry : fs::directory_iterator(fs::current_path().root_name())) {
		if (entry.path().extension().string() == ".fbx")
		{
			s.push(entry.path().filename().string());
		}
	}

	//parameters only for the loading graphic interface
	float percent = 50.0f / s.size(),
		loaded = 0;
	std::string percentStr = "",
		emptyStr = "";

	if (fixFbx)
	{
		while (!s.empty())
		{
			//things for the render
			loaded += percent;
			//loading interface render
			std::string load = "";
			for (int i = 0; i < 50.0f; i++)
			{
				if (i < loaded)
					load += "/";
				else
					load += " ";
			}
			std::cout << "		-FBX CHECKER-\n";
			std::cout << "\nCHEKING: \n[" + load + "] \n\n";

			//Reader routine
			if (r->correctFile(s.top().c_str())) {

					r->processScene(true,e, s.top().c_str());	
					r->clear();
			}
			s.pop();
			system("cls");
		}
		std::cout << "		-FBX CHECKER-\n";
		std::cout << "\nEXPORTING: PLEASE WAIT\n\n";
		e->exportFbxFixed();
		system("cls");
	}

	else
	{
		Output::init();
		//go through the stack
		while (!s.empty())
		{
			//things for the render
			loaded += percent;
			//loading interface render
			std::string load = "";
			for (int i = 0; i < 50.0f; i++)
			{
				if (i < loaded)
					load += "/";
				else
					load += " ";
			}
			std::cout << "		-FBX ANALYZER-\n";
			std::cout << "\nCHEKING: \n[" + load + "] \n\n";

			//Reader routine
			if (r->correctFile(s.top().c_str())) {

				Output::newFbx(s.top());
				r->processScene();
				r->clear();
				Output::endFbx();
			}
			s.pop();
			system("cls");
		}
		Output::end();
	}
	
}