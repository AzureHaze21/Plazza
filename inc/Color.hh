//
// EPITECH PROJECT, 2018
// 
// File description:
// 
//

#ifndef _COLOR_HH_
#define _COLOR_HH_

#include <string>

typedef enum e_color
{
	RED	= 9,
	BLUE	= 12,
	GREEN	= 2,
	YELLOW	= 11,
	ORANGE	= 202,
	CYAN	= 14,
	PURPLE	= 90,
	MAGENTA	= 197
} COLOR_ENUM;

std::string Color(COLOR_ENUM c, const std::string& s)
{
	return "\x1b[38;5;" + std::to_string(c) + 'm' + s + "\x1b[0m";
}

std::string Color(int c, const std::string& s)
{
	return "\x1b[38;5;" + std::to_string(c) + 'm' + s + "\x1b[0m";
}

#endif
