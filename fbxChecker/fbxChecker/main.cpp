// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Menu.h"
#include <string>

int main(std::string argv)
{
    Menu* m = new Menu();

   if (argv == "--fixerror")
   {
       m->init(true);
   }
   else
   {
       m->init(false);
   }   
    //int i = 0;
}
   
    
