// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Menu.h"
#include <string>

int main(int argc, char* argv[])
{

    bool entra = true;;
    Menu* m = new Menu();
    const char* c =  "--fixerror" ;
    int i = 0;

    while (i<11)
    {
        if (argv[1][i] != c[i])
        {
            entra = false;
        }

        i++;
    }
    
    if (entra)
    {
        m->init(true);

    }
    else
    {
        m->init(false);

    } 

}
   
    
