#pragma once

const int Blocks [] [5] [5] =
	{
	// Empty
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	// Simple pad
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	1, 1, 1, 1, 1,
	// Simple pad with console
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 2, 4, 2, 2,
	2, 2, 2, 2, 2,
	1, 1, 1, 1, 1,
	// Vent pad
	3, 3, 3, 3, 3,
	3, 3, 3, 3, 3,
	3, 3, 3, 3, 3,
	3, 3, 3, 3, 3,
	1, 1, 1, 1, 1,
	// Vent to simple pad
	3, 3, 5, 2, 2,
	3, 3, 5, 2, 2,
	3, 3, 5, 2, 2,
	3, 3, 5, 2, 2,
	1, 1, 1, 1, 1,
	// Simple to vent pad
	2, 2, 6, 3, 3,
	2, 2, 6, 3, 3,
	2, 2, 6, 3, 3,
	2, 2, 6, 3, 3,
	1, 1, 1, 1, 1,
	// Filled block
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	// Vent to simple locked door pad
	3, 3, 5, 2, 2,
	3, 3, 5, 2, 2,
	3, 3, 5, 2, 2,
	3, 3, 5, 2, 2,
	1, 1, 1, 1, 1,
	// Simple to vent locked door pad
	2, 2, 6, 3, 3,
	2, 2, 6, 3, 3,
	2, 2, 6, 3, 3,
	2, 2, 6, 3, 3,
	1, 1, 1, 1, 1,
	// Vent block
	3, 3, 3, 3, 3,
	3, 3, 3, 3, 3,
	3, 3, 3, 3, 3,
	3, 3, 3, 3, 3,
	3, 3, 3, 3, 3,
	// Window pad
	10, 9,  10, 10, 9,
	9,  10, 10, 9,  10,
	10, 9,  10, 10, 9,
	10, 10, 10, 9,  10,
	1,  1,  1,  1,  1,
	// Left dock
	11, 2, 10, 10, 10,
	11, 2, 10, 10, 10,
	11, 4, 10, 10, 10,
	11, 2, 10, 10, 10,
	11, 1,  1,  1,  1,
	// Right dock
	10, 10, 10, 4, 11,
	10, 10, 10, 4, 11,
	10, 10, 10, 4, 11,
	10, 10, 10, 4, 11,
	1,  1,  1,  1, 11,
	// Wall block
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
    2, 2, 2, 2, 2,
	// Pad with soldier looking left
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	1, 1, 1, 1, 1,
	// Pad with soldier looking right
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	1, 1, 1, 1, 1,
	// Door pad
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 4, 2, 4, 2,
	2, 2, 2, 2, 2,
	1, 1, 1, 1, 1,
	// Window wall
	10, 9,  10, 10, 9,
	9,  10, 10, 9,  10,
	10, 9,  10, 10, 9,
	10, 10, 9,  10, 10,
	9,  10, 10, 10, 10,
	// 	Ladder left up
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	7, 2, 2, 2, 2,
	// 	Ladder left down
	2, 7, 2, 2, 2,
	2, 2, 7, 2, 2,
	2, 2, 2, 7, 2,
	2, 2, 2, 2, 7,
	1, 1, 1, 1, 1,
	// Ladder right up
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 8,
	// Ladder right down
	2, 2, 2, 8, 2,
	2, 2, 8, 2, 2,
	2, 8, 2, 2, 2,
	8, 2, 2, 2, 2,
	1, 1, 1, 1, 1,
	// Left lift entrance
	2, 2, 2, 10, 6,
	2, 2, 2, 10, 6,
	2, 2, 2, 10, 6,
	2, 2, 2, 10, 6,
	1, 1, 1,  1, 1,
	// Right lift entrance
	7, 10, 2, 2, 2,
	7, 10, 2, 2, 2,
	7, 10, 2, 2, 2,
	7, 10, 2, 2, 2,
	1, 1, 1, 1, 1,
	// Lift start
	3, 3, 3, 3, 3,
	3, 3, 3, 3, 3,
	3, 3, 3, 3, 3,
	3, 3, 3, 3, 3,
	3, 3, 3, 3, 3,
	};

const char blocksNames [] [32] =
	{
	"Empty",
	"Basic pad",
	"Basic pad with console",
	"Vent pad",
	"Vent to basic pad",
	"Basic to vent pad",
	"Filled block",
	"Vent to simple locked door pad",
	"Simple to vent locked door pad",
	"Vent block",
	"Window pad",
	"Left dock",
	"Right dock",
	"Wall block",
	"Pad with soldier looking left",
	"Pad with soldier looking right",
	"Door pad",
	"Window wall",
	"Ladder left up", 
	"Ladder left down",
	"Ladder right up",
	"Ladder right down",
	"Left lift entrance",
	"Right lift entrance",
	"Lift start"
	};