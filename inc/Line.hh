/*
** EPITECH PROJECT, 2018
** 
** File description:
** 
*/

#ifndef _LINE_HH_
#define _LINE_HH_

#include <string>

std::string Line(int curr)
{
	static unsigned last = 1;
	int n = curr - last;
	
	last = curr;
	return ("\r" + (n < 0 ? "\33[" + std::to_string(-n) + 'A' : std::string(n, '\n')));
}

#endif
