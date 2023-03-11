#pragma once
#include "global.h"

struct t_color_scheme {

	vector<pair<rgb, rgb>> user_presets;

	void save()
	{
		string file_output = "";
		for (auto& preset : user_presets) {
			file_output += tgl.fmt("0x%06x,0x%06x", preset.first, preset.second) + "\n";
		}
		tgl.file_csave("color_schemes.txt", file_output);
	}
	void load()
	{
		user_presets.clear();
		auto lines = tgl.file_lines("color_schemes.txt");
		for (auto& line : lines) {
			if (line.empty() || line[0] == '#') {
				continue;
			}
			auto colors = tgl.split(line, ',');
			if (colors.size() == 2) {
				rgb forecolor = tgl.to_int(colors[0]);
				rgb backcolor = tgl.to_int(colors[1]);
				user_presets.push_back({ forecolor, backcolor });
			} else {
				tgl.abort("Error loading color preset from color_schemes.txt");
			}
		}
	}
};
