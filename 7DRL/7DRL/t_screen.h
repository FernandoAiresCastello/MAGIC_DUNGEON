#pragma once
#include "global.h"
#include "t_view.h"
#include "t_color_scheme.h"

struct t_screen {
	int cols;
	int rows;
	t_view view;
	t_color_scheme color_scheme;
};
