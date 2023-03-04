#pragma once
#include "global.h"

class t_infobox
{
public:

	const int y = 18;
	const int width = 32;

	void draw(TGL& tgl);
	void title(TGL& tgl, string str);
	void println(TGL& tgl, int line, string str);
	void println(TGL& tgl, int line, rgb color, string str);
};
