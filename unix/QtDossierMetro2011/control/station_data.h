#include "control.h"
#include "metro_protocol.h"

#define METRO_MAX_LIGNE		20
#define METRO_MAX_STATION	40

station_t stations[METRO_MAX_STATION] = {
		{"",   0,   0},
		{"A1", 200, 20},	// 1
		{"A2", 260, 37},	// 2
		{"A3", 240, 78},	// 3
		{"A4", 220, 150},	// 4
		{"A5", 320, 195},	// 5
		{"A6", 400, 275},	// 6
		{"A7", 530, 275},	// 7
		{"B1", 410, 20},	// 8
		{"B2", 220, 240},	// 9
		{"B3", 140, 245},	// 10
		{"C1", 122, 97},	// 11
		{"C3", 74,  160},	// 12
		{"D1", 60,  52},	// 13
		{"D2", 45,  118},	// 14
		{"D3", 420, 155},	// 15
		{"D4", 482, 65},	// 16
		{"E1", 120, 30},	// 17
		{"E3", 162, 160},	// 18
		{"E4", 182, 200},	// 19
		{"E6", 300, 275},	// 20
		{"E8", 480, 275},	// 21
		{"E9", 500, 200},	// 22
		{"F1", 20,  20},	// 23
		{"F4", 200, 115},	// 24
		{"F6", 335, 135},	// 25
		{"F8", 430, 60},	// 26
		{"X1", 20,  210},	// 27
		{"X2", 40,  255},	// 28
		{"X3", 520, 155},	// 29
		{"X4", 397, 122},	// 30
		{"X5", 342, 77},	// 31
		{"X6", 325, 26},	// 32
		{"",   0,   0},
	};

ligne_legacy_t lignes[METRO_MAX_LIGNE] = {
		{{
			{1, 200, 20},	// A1
			{2, 260, 40},	// A2
			{3, 260, 80},	// A3
			{4, 240, 140},	// A4
			{5, 320, 200},	// A5
			{6, 400, 260},	// A6
			{21, 480, 260},	// E8
			{7, 540, 260},	// A7
			
			{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
		}, LCOLOR_BLUE},
		{{
			{8, 400, 20},	// B1
			{32, 325, 30},	// X6
			{2, 262, 40},	// A2
			{3, 262, 80},	// A3
			{4, 242, 140},	// A4
			{9, 230, 220},	// B2
			{10, 140, 250},	// B3
			{28, 40, 260},	// X2
			
			{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
		}, LCOLOR_RED},
		{{
			{13, 60, 55},	// D1
			{14, 60, 120},	// D2
			{12, 70, 160},	// C3
			{10, 140, 253},	// B3
			{9, 226, 224},	// B2
			{5, 320, 200},	// A5
			{15, 420, 140},	// D3
			{16, 480, 65},	// D4
			
			{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
		}, LCOLOR_YELLOW}, 
		{{
			{27, 20, 200},	// X1
			{12, 72, 160},	// C3
			{14, 62, 120},	// D2
			{11, 120, 100},	// C1
			{3, 240, 80},	// A3
			{31, 360, 80},	// X5
			{30, 390, 120},	// X4
			{15, 420, 140},	// D3
			{29, 530, 140},	// X3
			
			{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
		}, LCOLOR_GREEN}, 
		{{
			{17, 120, 30},	// E1
			{11, 120, 100},	// C1
			{18, 160, 160},	// E3
			{19, 180, 200},	// E4
			{9, 230, 225},	// B2
			{20, 300, 262},	// E6
			{6, 400, 262},	// A6
			{21, 480, 262},	// E8
			{22, 500, 200},	// E9
			
			{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
		}, LCOLOR_WHITE}, 
		{{
			{23, 20, 20},	// F1
			{13, 60, 55},	// D1
			{11, 120, 100},	// C1
			{24, 200, 120},	// F4
			{4, 240, 140},	// A4
			{25, 340, 140},	// F6
			{30, 390, 120},	// X4
			{26, 430, 60},	// F8
			
			{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
		}, LCOLOR_BLACK},
		{{
			{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
			{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
		}, LCOLOR_EOF},
	};

metro_nodes_t nodes[METRO_MAX_STATION] = {
		{0, { 0,  0,  0,  0,  0,  0}},
		{0, { 2,  0,  0,  0,  0,  0}},	// 1 - A1
		{0, { 1,  3, 32,  0,  0,  0}},	// 2 - A2
		{0, { 2,  4, 11, 31,  0,  0}},	// 3 - A3
		{0, { 3, 24, 25,  5,  9,  0}},	// 4 - A4
		{0, { 4, 15,  6,  9,  0,  0}},	// 5 - A5
		{0, { 5, 21, 20,  0,  0,  0}},	// 6 - A6
		{0, {21,  0,  0,  0,  0,  0}},	// 7 - A7
		{0, {32,  0,  0,  0,  0,  0}},	// 8 - B1
		{0, {19,  4,  5, 20, 10,  0}},	// 9 - B2
		{0, { 9, 28, 12,  0,  0,  0}},	// 10 - B3
		{0, {13, 17,  3, 18, 24, 14}}, 	// 11 - C1
		{0, {14, 27, 10,  0,  0,  0}},	// 12 - C3
		{0, {23, 11, 14,  0,  0,  0}},	// 13 - D1 *
		{0, {13, 11, 12,  0,  0,  0}},	// 14 - D2
		{0, {30, 16,  5, 29,  0,  0}},	// 15 - D3
		{0, {15,  0,  0,  0,  0,  0}},	// 16 - D4
		{0, {11,  0,  0,  0,  0,  0}},	// 17 - E1
		{0, {11, 19,  0,  0,  0,  0}},	// 18 - E3
		{0, {18,  9,  0,  0,  0,  0}},	// 19 - E4
		{0, { 9,  6,  0,  0,  0,  0}},	// 20 - E6
		{0, { 6,  7, 22,  0,  0,  0}},	// 21 - E8
		{0, {21,  0,  0,  0,  0,  0}},	// 22 - E9
		{0, {13,  0,  0,  0,  0,  0}},	// 23 - F1
		{0, { 4, 11,  0,  0,  0,  0}},	// 24 - F4
		{0, { 4, 30,  0,  0,  0,  0}},	// 25 - F6
		{0, {30,  0,  0,  0,  0,  0}},	// 26 - F8
		{0, {12,  0,  0,  0,  0,  0}},	// 27 - X1
		{0, {10,  0,  0,  0,  0,  0}},	// 28 - X2
		{0, {15,  0,  0,  0,  0,  0}},	// 29 - X3
		{0, {31, 25, 15, 26,  0,  0}},	// 30 - X4
		{0, { 3, 30,  0,  0,  0,  0}},	// 31 - X5
		{0, { 2,  8,  0,  0,  0,  0}},	// 32 - X6
		{0, { 0,  0,  0,  0,  0,  0}} 
	   };
