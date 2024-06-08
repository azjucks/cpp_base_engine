#pragma once

#include <vec4.h>

namespace Core
{
	namespace Maths
	{
		typedef vec4 Color;
	}
}

// Some Basic Colors
// NOTE: Custom raylib color palette for amazing visuals on WHITE background
#define LIGHTGRAY  { 200, 200, 200, 255 }   // Light Gray
#define GRAY       { 130, 130, 130, 255 }   // Gray
#define DARKGRAY   { 80, 80, 80, 255 }      // Dark Gray
#define YELLOW     { 253, 249, 0, 255 }     // Yellow
#define GOLD       { 255, 203, 0, 255 }     // Gold
#define ORANGE     { 255, 161, 0, 255 }     // Orange
#define PINK       { 255, 109, 194, 255 }   // Pink
#define RED        { 230, 41, 55, 255 }     // Red
#define MAROON     { 190, 33, 55, 255 }     // Maroon
#define GREEN      { 0, 228, 48, 255 }      // Green
#define LIME       { 0, 158, 47, 255 }      // Lime
#define DARKGREEN  { 0, 117, 44, 255 }      // Dark Green
#define SKYBLUE    { 102, 191, 255, 255 }   // Sky Blue
#define BLUE       { 0, 121, 241, 255 }     // Blue
#define DARKBLUE   { 0, 82, 172, 255 }      // Dark Blue
#define PURPLE     { 200, 122, 255, 255 }   // Purple
#define VIOLET     { 135, 60, 190, 255 }    // Violet
#define DARKPURPLE { 112, 31, 126, 255 }    // Dark Purple
#define BEIGE      { 211, 176, 131, 255 }   // Beige
#define BROWN      { 127, 106, 79, 255 }    // Brown
#define DARKBROWN  { 76, 63, 47, 255 }      // Dark Brown

#define WHITE      { 255, 255, 255, 255 }   // White
#define BLACK      { 0, 0, 0, 255 }         // Black
#define BLANK      { 0, 0, 0, 0 }           // Blank (Transparent)
#define MAGENTA    { 255, 0, 255, 255 }     // Magenta
#define RAYWHITE   { 245, 245, 245, 255 }   // My own White (raylib logo)