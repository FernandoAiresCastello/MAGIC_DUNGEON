#include <TGL.h>

TGL tgl;

struct t_screen {
	int cols;
	int rows;
};
t_screen screen;

enum class t_direction {
	none, north, east, south, west
};

enum class t_terrain {
	oob, ground, wall, stairs
};

enum class t_entity {
	none, player
};

struct t_location {
	bool visited = false;
	t_terrain terrain = t_terrain::ground;
	t_entity entity = t_entity::none;
	int terrain_variant = 0;
};

struct t_exit {
	int x = -1;
	int y = -1;
};

struct t_floor {
	static const int width = 102;
	static const int height = 102;
	t_location oob = t_location();
	rgb forecolor = 0xffffff;
	rgb backcolor = 0x000000;
	t_exit floor_exit;

	t_floor()
	{
		oob.terrain = t_terrain::oob;
	}
	t_location& get(int x, int y)
	{
		if (x >= 0 && y >= 0 && x < width && y < height) {
			return locations[y][x];
		} else {
			return oob;
		}
	}
	void set(t_entity e, int x, int y)
	{
		if (x >= 0 && y >= 0 && x < width && y < height) {
			locations[y][x].entity = e;
		}
	}
	void set(t_terrain t, int x, int y)
	{
		if (x >= 0 && y >= 0 && x < width && y < height) {
			locations[y][x].terrain = t;
			locations[y][x].terrain_variant = tgl.rnd(0, 2);
		}
	}
	void detonate_wall(int x, int y)
	{
		if (get(x, y).terrain == t_terrain::wall) {
			set(t_terrain::ground, x, y);
		}
	}
	void visit(int x, int y)
	{
		if (x >= 0 && y >= 0 && x < width && y < height) {
			if (!locations[y][x].visited) {
				locations[y][x].visited = true;
			}
		}
	}
	void unvisit(int x, int y)
	{
		if (x >= 0 && y >= 0 && x < width && y < height) {
			if (locations[y][x].visited) {
				locations[y][x].visited = false;
			}
		}
	}
	void color(int fg, int bg)
	{
		forecolor = fg;
		backcolor = bg;
	}
private:
	t_location locations[height][width];
};
t_floor cur_floor;

struct t_view {
	int scroll_x = 0;
	int scroll_y = 0;

	void setscrl(int x, int y)
	{
		scroll_x = x;
		scroll_y = y;
	}
	void scroll(int dx, int dy)
	{
		scroll_x += dx;
		scroll_y += dy;
	}
};
t_view view;

struct t_bomb {
	int x = -1;
	int y = -1;
	bool active = false;
	int timer = 0;
	const int timer_max = 200;

	void activate(int bx, int by)
	{
		if (!active) {
			x = bx;
			y = by;
			timer = 0;
			active = true;
		}
	}
	void tick()
	{
		if (active) {
			timer++;
			if (timer >= timer_max) {
				detonate();
			}
		}
	}
	void detonate()
	{
		timer = 0;
		active = false;

		cur_floor.detonate_wall(x - 1, y - 1);
		cur_floor.detonate_wall(x + 0, y - 1);
		cur_floor.detonate_wall(x + 1, y - 1);
		cur_floor.detonate_wall(x - 1, y);
		cur_floor.detonate_wall(x, y);
		cur_floor.detonate_wall(x + 1, y);
		cur_floor.detonate_wall(x - 1, y + 1);
		cur_floor.detonate_wall(x + 0, y + 1);
		cur_floor.detonate_wall(x + 1, y + 1);
	}
};

struct t_player {
	t_direction dir = t_direction::none;
	t_bomb bomb;

	int get_x() { return x; }
	int get_y() { return y; }
	int get_prevx() { return prevx; }
	int get_prevy() { return prevy; }
	int get_life() { return life; }

	bool oob()
	{
		return x < 0 || y < 0 || x >= cur_floor.width || y >= cur_floor.height;
	}
	void setpos(int newx, int newy)
	{
		prevx = x;
		prevy = y;
		x = newx;
		y = newy;
	}
	void move(int dx, int dy)
	{
		t_direction old_dir = dir;

		if (dx > 0) dir = t_direction::east;
		else if (dx < 0) dir = t_direction::west;
		if (dy > 0) dir = t_direction::south;
		else if (dy < 0) dir = t_direction::north;

		int newx = x + dx;
		int newy = y + dy;
		if (can_move_to(newx, newy)) {
			setpos(newx, newy);
			view.scroll(dx, dy);
		}
	}
	bool can_move_to(int newx, int newy)
	{
		t_location& loc = cur_floor.get(newx, newy);
		t_terrain& t = loc.terrain;
		t_entity& e = loc.entity;
		
		return t == t_terrain::ground || t == t_terrain::stairs;
	}
	t_location& getloc()
	{
		return cur_floor.get(x, y);
	}
	bool is_on_stairs()
	{
		return getloc().terrain == t_terrain::stairs;
	}
	void drop_bomb()
	{
		if (!bomb.active) {
			bomb.activate(x, y);
		}
	}
private:
	int x = 0;
	int y = 0;
	int prevx = x;
	int prevy = y;
	int life = 100;
};
t_player player;

void generate_exit(int x, int y)
{
	cur_floor.floor_exit.x = x;
	cur_floor.floor_exit.y = y;
	cur_floor.set(t_terrain::stairs, x, y);
}
void generate_room(int x, int y, int w, int h)
{
	// walls
	for (int px = x; px <= x + w; px++) {
		cur_floor.set(t_terrain::wall, px, y);
		cur_floor.set(t_terrain::wall, px, y + h);
	}
	for (int py = y; py <= y + h; py++) {
		cur_floor.set(t_terrain::wall, x, py);
		cur_floor.set(t_terrain::wall, x + w, py);
	}

	// room entrance & room exit
	int exit_x = 0;
	int exit_y = 0;
	int exit_placement = tgl.rnd(0, 3);
	if (exit_placement == 0) { // on top corner
		exit_y = y;
		exit_x = tgl.rnd(x + 1, x + w - 1);
	} else if (exit_placement == 1) { // on bottom corner
		exit_y = y + h;
		exit_x = tgl.rnd(x + 1, x + w - 1);
	} else if (exit_placement == 2) { // on left corner
		exit_x = x;
		exit_y = tgl.rnd(y + 1, y + h - 1);
	} else if (exit_placement == 3) { // on right corner
		exit_x = x + w;
		exit_y = tgl.rnd(y + 1, y + h - 1);
	}
	cur_floor.set(t_terrain::ground, exit_x, exit_y);
}
void generate_wall(int x, int y, int length, int orient)
{
	int px = x;
	int py = y;
	for (int i = 0; i <= length; i++) {
		cur_floor.set(t_terrain::wall, px, py);
		if (orient == 0) {
			px++;
		} else {
			py++;
		}
	}
}

struct {
	vector<rgb> background = {
		0x101010, // black,
		0xc0c0c0, // gray,
		0x202050, // indigo
		0x3030c0, // blue,
		0x802020, // red
	};
	vector<rgb> foreground = {
		0x3050ff, // blue
		0x30ffff, // cyan
		0x30ff50, // green
		0xff4080, // pink
		0xff4040, // red
		0xffa030, // orange
		0xffff50, // yellow
		0xe0e0e0, // white
		0x404040, // gray
	};
} color_scheme;

void randomize_color_scheme()
{
	int fgc_index = tgl.rnd(0, color_scheme.foreground.size() - 1);
	int bgc_index = tgl.rnd(0, color_scheme.background.size() - 1);
	cur_floor.color(color_scheme.foreground[fgc_index], color_scheme.background[bgc_index]);
}
void init_current_floor()
{
	const int min_room_w = 3;
	const int min_room_h = 3;
	const int max_room_w = 20;
	const int max_room_h = 20;
	const int max_wall_len = 40;
	const int room_count = 50;
	const int random_wall_count = 40;
	const int exit_count = 5;

	// color scheme
	randomize_color_scheme();

	// base
	for (int y = 0; y < cur_floor.height; y++) {
		for (int x = 0; x < cur_floor.width; x++) {
			cur_floor.unvisit(x, y);
			cur_floor.set(t_terrain::ground, x, y);
		}
	}

	// corners
	for (int x = 0; x < cur_floor.width; x++) {
		cur_floor.set(t_terrain::wall, x, 0);
		cur_floor.set(t_terrain::wall, x, cur_floor.height - 1);
	}
	for (int y = 0; y < cur_floor.height; y++) {
		cur_floor.set(t_terrain::wall, 0, y);
		cur_floor.set(t_terrain::wall, cur_floor.width - 1, y);
	}
	// rooms
	for (int i = 0; i < room_count; i++) {
		int x = tgl.rnd(0, cur_floor.width);
		int y = tgl.rnd(0, cur_floor.height);
		int w = tgl.rnd(min_room_w, max_room_w);
		int h = tgl.rnd(min_room_h, max_room_h);
		generate_room(x, y, w, h);
	}
	// random walls
	for (int i = 0; i < random_wall_count; i++) {
		int x = tgl.rnd(0, cur_floor.width);
		int y = tgl.rnd(0, cur_floor.height);
		int length = tgl.rnd(2, max_wall_len);
		int orient = tgl.rnd(0, 1);
		generate_wall(x, y, length, orient);
	}
	// exits
	for (int i = 0; i < exit_count; i++) {
		int stx = tgl.rnd(1, cur_floor.width - 2);
		int sty = tgl.rnd(1, cur_floor.height - 2);
		generate_exit(stx, sty);
	}
}
void draw_current_floor()
{
	int scrx = 0;
	int scry = 0;

	tgl.tile_transparent(true);
	tgl.backcolor(cur_floor.backcolor);
	tgl.color_binary(cur_floor.forecolor);

	for (int mapy = view.scroll_y; mapy < view.scroll_y + screen.rows; mapy++) {
		for (int mapx = view.scroll_x; mapx < view.scroll_x + screen.cols; mapx++) {
			if (mapx >= 0 && mapy >= 0 && mapx < cur_floor.width && mapy < cur_floor.height) {
				t_location& loc = cur_floor.get(mapx, mapy);
				if (loc.visited) {
					// entities
					if (loc.entity == t_entity::player) {
						tgl.draw_tiled("player", scrx, scry);
					} else if (player.bomb.active && player.bomb.x == mapx && player.bomb.y == mapy) {
						tgl.draw_tiled("bomb", scrx, scry);
					}
					// terrain
					else {
						if (loc.terrain == t_terrain::ground) {
							if (loc.terrain_variant == 0) {
								tgl.draw_tiled("ground", scrx, scry);
							} else if (loc.terrain_variant == 1) {
								tgl.draw_tiled("ground_2", scrx, scry);
							} else if (loc.terrain_variant == 2) {
								tgl.draw_tiled("ground_3", scrx, scry);
							}
						} else if (loc.terrain == t_terrain::wall) {
							if (loc.terrain_variant == 0) {
								tgl.draw_tiled("solid", scrx, scry);
							} else if (loc.terrain_variant == 1 || loc.terrain_variant == 2) {
								tgl.draw_tiled("wall", scrx, scry);
							}
						} else if (loc.terrain == t_terrain::stairs) {
							tgl.draw_tiled("stairs", scrx, scry);
						}
					}
				}
			} else { // out-of-bounds
				tgl.draw_tiled("empty", scrx, scry);
			}
			scrx++;
		}
		scry++;
		scrx = 0;
	}
}
void draw_info()
{
	tgl.font_transparent(false);
	tgl.font_color(cur_floor.forecolor, cur_floor.backcolor);
	tgl.print_tiled(tgl.fmt("E%iN%i", player.get_x(), player.get_y()), 1, 0);
	tgl.print_tiled(tgl.fmt("~%03i", player.get_life()), 1, screen.rows - 1);
}
void sync_player()
{
	cur_floor.set(t_entity::none, player.get_prevx(), player.get_prevy());
	cur_floor.set(t_entity::player, player.get_x(), player.get_y());
}
void visit_surroundings()
{
	int x = player.get_x();
	int y = player.get_y();

	cur_floor.visit(x - 1, y - 1);
	cur_floor.visit(x + 0, y - 1);
	cur_floor.visit(x + 1, y - 1);
	cur_floor.visit(x - 1, y);
	cur_floor.visit(x, y);
	cur_floor.visit(x + 1, y);
	cur_floor.visit(x - 1, y + 1);
	cur_floor.visit(x + 0, y + 1);
	cur_floor.visit(x + 1, y + 1);
}

void init_tiles();
void goto_next_floor();
void game_init()
{
	tgl.window_gbc(0x000000, 5);
	screen.cols = tgl.cols();
	screen.rows = tgl.rows();

	init_tiles();
	goto_next_floor();
}
void goto_next_floor()
{
	init_current_floor();
	player.setpos(tgl.rnd(1, cur_floor.width - 1), tgl.rnd(1, cur_floor.height - 1));
	view.setscrl(player.get_x() - screen.cols / 2 + 1, player.get_y() - screen.rows / 2 + 1);
}
void check_collisions()
{
	if (player.is_on_stairs()) {
		goto_next_floor();
	}
}
void tick_bomb_timers()
{
	if (player.bomb.active) {
		player.bomb.tick();
	}
}
void game_loop()
{
	tgl.clear();
	sync_player();
	visit_surroundings();
	draw_current_floor();
	draw_info();
	tgl.system();

	check_collisions();
	tick_bomb_timers();

	int key = tgl.kb_lastkey();
	if (tgl.kb_esc()) tgl.exit();

	if (key == SDLK_RIGHT) player.move(1, 0);
	else if (key == SDLK_LEFT) player.move(-1, 0);
	else if (key == SDLK_DOWN) player.move(0, 1);
	else if (key == SDLK_UP) player.move(0, -1);
	else if (key == SDLK_c) randomize_color_scheme();
	else if (key == SDLK_SPACE) player.drop_bomb();
}
int main(int argc, char* argv[])
{
	game_init();
	while (tgl.running()) {
		game_loop();
	}
	return tgl.exit();
}
void init_tiles()
{
	tgl.tile_new("player_1",
		"00111000"
		"00111010"
		"11010010"
		"11111010"
		"11010111"
		"00111010"
		"01101000"
		"00001100"
	);
	tgl.tile_new("player_2",
		"00111010"
		"00111010"
		"11010010"
		"11111111"
		"11010010"
		"00111000"
		"00101100"
		"01100000"
	);
	tgl.tile_add("player", "player_1");
	tgl.tile_add("player", "player_2");

	tgl.tile_new("ground",
		"00000000"
		"00000000"
		"00000000"
		"00010000"
		"00000000"
		"00000000"
		"00000000"
		"00000000"
	);
	tgl.tile_add("ground", "ground");

	tgl.tile_new("ground_2",
		"00000000"
		"00000000"
		"00000100"
		"00000000"
		"00000000"
		"00010000"
		"00000000"
		"00000000"
	);
	tgl.tile_add("ground_2", "ground_2");

	tgl.tile_new("ground_3",
		"10000000"
		"00000000"
		"00000000"
		"00000000"
		"00000001"
		"00000000"
		"00000000"
		"00000000"
	);
	tgl.tile_add("ground_3", "ground_3");

	tgl.tile_new("empty",
		"00000000"
		"00000000"
		"00000000"
		"00000000"
		"00000000"
		"00000000"
		"00000000"
		"00000000"
	);
	tgl.tile_add("empty", "empty");

	tgl.tile_new("solid",
		"11111111"
		"11111111"
		"11111111"
		"11111111"
		"11111111"
		"11111111"
		"11111111"
		"00000000"
	);
	tgl.tile_add("solid", "solid");

	tgl.tile_new("marker",
		"10000001"
		"01000010"
		"00100100"
		"00011000"
		"00011000"
		"00100100"
		"01000010"
		"10000001"
	);
	tgl.tile_add("marker", "marker");

	tgl.tile_new("wall",
		"11111011"
		"11111011"
		"11111011"
		"00000000"
		"11011111"
		"11011111"
		"11011111"
		"00000000"
	);
	tgl.tile_add("wall", "wall");

	tgl.tile_new("stairs",
		"00000000"
		"11100000"
		"11100000"
		"11111000"
		"11111000"
		"11111110"
		"11111110"
		"00000000"
	);
	tgl.tile_add("stairs", "stairs", 3);
	tgl.tile_add("stairs", "empty");

	tgl.tile_new("bomb_1",
		"00000100"
		"00001000"
		"00111100"
		"01110110"
		"01111110"
		"00111100"
		"00000000"
		"00000000"
	);
	tgl.tile_new("bomb_2",
		"00000000"
		"00010000"
		"00001000"
		"00111100"
		"01110110"
		"01111110"
		"00111100"
		"00000000"
	);
	tgl.tile_add("bomb", "bomb_1");
	tgl.tile_add("bomb", "bomb_2");

	tgl.tile_new("arrow_down",
		"00111000"
		"00111000"
		"00111000"
		"11111110"
		"01111100"
		"00111000"
		"00010000"
		"00000000"
	);
	tgl.tile_add("arrow_down", "arrow_down");
}
