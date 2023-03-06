#pragma once
#include "global.h"

struct t_tileloader
{
	void load_all();

private:
	void load(string id, string filename);
	void load_single(string id);
};
