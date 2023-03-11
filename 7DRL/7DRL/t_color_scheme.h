#pragma once
#include "global.h"

struct t_color_scheme {
	vector<pair<rgb, rgb>> presets = {
		// black backgrounds
		{ 0xc030a0, 0x101010 }, // purple-black
		{ 0xff4080, 0x101010 }, // pink-black
		{ 0xe0e0e0, 0x101010 }, // white-black
		{ 0x404040, 0x101010 }, // gray-black
		{ 0xffff50, 0x101010 }, // yellow-black
		{ 0xffa030, 0x101010 }, // amber-black
		{ 0x3050ff, 0x101010 }, // blue-black
		{ 0x30ffff, 0x101010 }, // cyan-black
		{ 0x30ff50, 0x101010 }, // green-black
		{ 0xff4040, 0x101010 }, // red-black
	};
	vector<pair<rgb, rgb>> old_presets = {
		// purple backgrounds
		{ 0x1010a0, 0x902080 }, // blue-purple
		{ 0xe05080, 0x902080 }, // pink-purple
		{ 0xe0e040, 0x902080 }, // yellow-purple
		//{ 0x10e010, 0x902080 }, // green-purple
		{ 0x10c0ff, 0x902080 }, // cyan-purple
		{ 0xe0e0e0, 0x902080 }, // white-purple
		{ 0x101010, 0x902080 }, // black-purple
		// amber backgrounds
		{ 0x101010, 0xffa030 }, // black-amber
		{ 0xe02020, 0xe09020 }, // red-amber
		{ 0xe0e0e0, 0xe09020 }, // white-amber
		// pink backgrounds
		//{ 0x10a010, 0xe04080 }, // green-pink
		{ 0x10c0ff, 0xe04080 }, // cyan-pink
		{ 0xe0e040, 0xe04080 }, // yellow-pink
		{ 0x2020d0, 0xe04080 }, // blue-pink
		{ 0xe0e0e0, 0xe04080 }, // white-pink
		{ 0xc01010, 0xe04080 }, // red-pink
		{ 0x101010, 0xe04080 }, // black-pink
		// gray backgrounds
		{ 0xc0c0c0, 0x606060 }, // white-gray
		{ 0xe05080, 0x606060 }, // pink-gray
		{ 0x1010a0, 0x606060 }, // blue-gray
		{ 0x10a010, 0x606060 }, // green-gray
		{ 0x101010, 0x606060 }, // black-gray
		{ 0xa02020, 0x606060 }, // red-gray
		// red backgrounds
		//{ 0xe01010, 0x901010 }, // red-red
		{ 0x10c0ff, 0xa02020 }, // cyan-red
		{ 0xffa030, 0xa02020 }, // amber-red
		{ 0xe0e040, 0xa02020 }, // yellow-red
		{ 0xe0e0e0, 0xa02020 }, // white-red
		{ 0x101010, 0xa02020 }, // black-red
		// green backgrounds
		{ 0x904020, 0x20a020 }, // brown-green
		{ 0x30ffff, 0x20a020 }, // cyan-green
		{ 0x2020a0, 0x20a020 }, // blue-green
		{ 0xa02020, 0x20a020 }, // red-green
		{ 0xb02090, 0x20a020 }, // purple-green
		{ 0xe0e0e0, 0x20a020 }, // white-green
		{ 0x101010, 0x20a020 }, // black-green
		// cyan backgrounds
		{ 0x20ffff, 0x20d0d0 }, // cyan-cyan
		{ 0xffffff, 0x20d0d0 }, // white-cyan
		{ 0x20a020, 0x20e0e0 }, // green-cyan
		{ 0x2020a0, 0x20e0e0 }, // blue-cyan
		{ 0xe02020, 0x20e0e0 }, // red-cyan
		{ 0x101010, 0x20e0e0 }, // black-cyan
		// yellow backgrounds
		{ 0xc030a0, 0xe0e040 }, // purple-yellow
		{ 0x2020a0, 0xe0e040 }, // blue-yellow
		{ 0x20a020, 0xe0e040 }, // green-yellow
		{ 0xe02020, 0xe0e040 }, // red-yellow
		{ 0x101010, 0xe0e040 }, // black-yellow
		// blue backgrounds
		{ 0xff4080, 0x3030c0 }, // pink-blue
		{ 0x101010, 0x3030c0 }, // black-blue
		//{ 0x3050ff, 0x3030c0 }, // blue-blue
		{ 0x30ffff, 0x3030c0 }, // cyan-blue
		{ 0xff6060, 0x3030c0 }, // red-blue		
		{ 0x30ff50, 0x3030c0 }, // green-blue
		{ 0xffff50, 0x3030c0 }, // yellow-blue
		{ 0xe0e0e0, 0x3030c0 }, // white-blue
		// white backgrounds
		//{ 0xffffff, 0xd0d0d0 }, // white-white
		{ 0x10c0c0, 0xe0e0e0 }, // cyan-white
		{ 0x1030e0, 0xe0e0e0 }, // blue-white
		{ 0xe02020, 0xe0e0e0 }, // red-white
		{ 0x108020, 0xe0e0e0 }, // green-white
		{ 0x101010, 0xe0e0e0 }, // black-white
		// black backgrounds
		{ 0xc030a0, 0x101010 }, // purple-black
		{ 0xff4080, 0x101010 }, // pink-black
		{ 0xe0e0e0, 0x101010 }, // white-black
		{ 0x404040, 0x101010 }, // gray-black
		{ 0xffff50, 0x101010 }, // yellow-black
		{ 0xffa030, 0x101010 }, // amber-black
		{ 0x3050ff, 0x101010 }, // blue-black
		{ 0x30ffff, 0x101010 }, // cyan-black
		{ 0x30ff50, 0x101010 }, // green-black
		{ 0xff4040, 0x101010 }, // red-black
	};
};
