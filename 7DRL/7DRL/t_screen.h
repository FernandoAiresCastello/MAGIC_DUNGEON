#pragma once
#include "global.h"
#include "t_view.h"
#include "t_color_scheme.h"

struct t_screen {
	int cols;
	int rows;
	t_view view;
	t_color_scheme color_scheme;

	void clear_top_text()
	{
		for (int x = 0; x < cols; x++) {
			tgl.print_tiled(" ", x, 0);
		}
	}
	void clear_bottom_text()
	{
		for (int x = 0; x < cols; x++) {
			tgl.print_tiled(" ", x, rows - 2);
			tgl.print_tiled(" ", x, rows - 1);
		}
	}
	void print_bottom(string msg1, string msg2)
	{
		clear_bottom_text();
		tgl.print_tiled(msg1, 0, rows - 2);
		tgl.print_tiled(msg2, 0, rows - 1);
	}
	void print_pause(string msg, int timer = 100)
	{
		while (tgl.running() && timer > 0) {
			clear_bottom_text();
			tgl.print_tiled(msg, 0, rows - 2);
			tgl.system();
			timer--;
		}
	}
	bool confirm(string msg)
	{
		return confirm(msg, "");
	}
	bool confirm(string msg1, string msg2)
	{
		clear_bottom_text();
		tgl.print_tiled(msg1, 0, rows - 2);
		tgl.print_tiled(msg2, 0, rows - 1);

		bool finished = false;
		bool confirmed = false;

		int key = 0;

		while (tgl.running() && !finished) {
			tgl.system();
			key = tgl.kb_lastkey();
			if (key == SDLK_y) {
				confirmed = true;
				finished = true;
			} else if (key == SDLK_n || key == SDLK_ESCAPE) {
				confirmed = false;
				finished = true;
			}
		}
		return confirmed;
	}
	rgb inc_color_r(rgb color, int amount = 0x10)
	{
		int red = (color >> 16) & 0xff;
		int green = (color >> 8) & 0xff;
		int blue = color & 0xff;
		if (red == 0xf0) red = 0xff; else red += amount;
		if (red > 0xff) {
			red = 0;
		}
		return (red << 16) | (green << 8) | blue;
	}
	rgb inc_color_g(rgb color, int amount = 0x10)
	{
		int red = (color >> 16) & 0xff;
		int green = (color >> 8) & 0xff;
		int blue = color & 0xff;
		if (green == 0xf0) green = 0xff; else green += amount;
		if (green > 0xff) {
			green = 0;
		}
		return (red << 16) | (green << 8) | blue;
	}
	rgb inc_color_b(rgb color, int amount = 0x10)
	{
		int red = (color >> 16) & 0xff;
		int green = (color >> 8) & 0xff;
		int blue = color & 0xff;
		if (blue == 0xf0) blue = 0xff; else blue += amount;
		if (blue > 0xff) {
			blue = 0;
		}
		return (red << 16) | (green << 8) | blue;
	}
	rgb dec_color_r(rgb color, int amount = -0x10)
	{
		int red = (color >> 16) & 0xff;
		int green = (color >> 8) & 0xff;
		int blue = color & 0xff;
		if (red == 0xff) amount++;
		red += amount;
		if (red < 0x00) {
			red = 0xff;
		}
		return (red << 16) | (green << 8) | blue;
	}
	rgb dec_color_g(rgb color, int amount = -0x10)
	{
		int red = (color >> 16) & 0xff;
		int green = (color >> 8) & 0xff;
		int blue = color & 0xff;
		if (green == 0xff) amount++;
		green += amount;
		if (green < 0x00) {
			green = 0xff;
		}
		return (red << 16) | (green << 8) | blue;
	}
	rgb dec_color_b(rgb color, int amount = -0x10)
	{
		int red = (color >> 16) & 0xff;
		int green = (color >> 8) & 0xff;
		int blue = color & 0xff;
		if (blue == 0xff) amount++;
		blue += amount;
		if (blue < 0x00) {
			blue = 0xff;
		}
		return (red << 16) | (green << 8) | blue;
	}
};
