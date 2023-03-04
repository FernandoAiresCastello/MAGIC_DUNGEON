#pragma once
#include "global.h"
#include "t_player.h"

class t_infobox
{
public:
	static const int y = 18;
	static const int width = 32;

	void print_test(TGL& tgl);
	void print_debug(TGL& tgl, t_player& player);

	void title(TGL& tgl, string str);
	void println(TGL& tgl, int line, string str);
	void println(TGL& tgl, int line, rgb color, string str);

private:

};
