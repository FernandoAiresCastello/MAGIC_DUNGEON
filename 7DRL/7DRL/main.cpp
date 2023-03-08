#include <TGL.h>

TGL tgl;

enum class t_terrain {
	oob, ground, wall
};
enum class t_entity {
	none, player
};
struct t_location {
	bool visited = false;
	t_terrain terrain = t_terrain::ground;
	t_entity entity = t_entity::none;
};
struct t_world {
	static const int width = 256;
	static const int height = 256;
	t_location oob = t_location();
	t_world() {
		oob.terrain = t_terrain::oob;
	}
	t_location& get(int x, int y) {
		if (x >= 0 && y >= 0 && x < world.width && y < world.height) {
			return locations[y][x];
		} else {
			return oob;
		}
	}
	void set(t_entity e, int x, int y) {
		if (x >= 0 && y >= 0 && x < world.width && y < world.height) {
			locations[y][x].entity = e;
		}
	}
	void set(t_terrain t, int x, int y) {
		if (x >= 0 && y >= 0 && x < world.width && y < world.height) {
			locations[y][x].terrain = t;
		}
	}
	void visit(int x, int y) {
		if (x >= 0 && y >= 0 && x < world.width && y < world.height) {
			if (!locations[y][x].visited) {
				locations[y][x].visited = true;
			}
		}
	}
private:
	t_location locations[height][width];
} world;
struct {
	int scroll_x = 0;
	int scroll_y = 0;
	void setscrl(int x, int y) {
		scroll_x = x;
		scroll_y = y;
	}
} view;
struct {
	int cols;
	int rows;
} screen;
struct {
	int getx() { return x; }
	int gety() { return y; }
	int getprevx() { return prevx; }
	int getprevy() { return prevy; }

	bool oob() {
		return x < 0 || y < 0 || x >= world.width || y >= world.height;
	}
	void setpos(int newx, int newy) {
		prevx = x;
		prevy = y;
		x = newx;
		y = newy;
	}
	void move(int dx, int dy) {
		int newx = x + dx;
		int newy = y + dy;
		if (can_move_to(newx, newy)) {
			setpos(newx, newy);
		}
	}
	bool can_move_to(int newx, int newy) {
		return world.get(newx, newy).terrain == t_terrain::ground;
	}
private:
	int x = 0;
	int y = 0;
	int prevx = x;
	int prevy = y;
} player;

void init_tiles()
{
	tgl.tile_new("player",
		"00111010"
		"00111010"
		"11010010"
		"11111111"
		"11010010"
		"00111000"
		"00101000"
		"01101100"
	);
	tgl.tile_add("player", "player");

	tgl.tile_new("ground",
		"00000000"
		"00000000"
		"00000000"
		"00011000"
		"00011000"
		"00000000"
		"00000000"
		"00000000"
	);
	tgl.tile_add("ground", "ground");

	tgl.tile_new("oob",
		"00000000"
		"01000010"
		"00100100"
		"00011000"
		"00011000"
		"00100100"
		"01000010"
		"00000000"
	);
	tgl.tile_add("oob", "oob");

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
}
void init_world()
{
	world.set(t_terrain::wall, 0, 0);
	world.set(t_terrain::wall, 1, 1);
}
void draw_world()
{
	int scrx = 0;
	int scry = 0;

	tgl.clear();
	tgl.transparent(true);

	for (int mapy = view.scroll_y; mapy < view.scroll_y + screen.rows; mapy++) {
		for (int mapx = view.scroll_x; mapx < view.scroll_x + screen.cols; mapx++) {
			if (mapx >= 0 && mapy >= 0 && mapx < world.width && mapy < world.height) {
				t_location& loc = world.get(mapx, mapy);
				if (loc.visited) {
					// terrain
					if (loc.terrain == t_terrain::ground) {
						tgl.color_binary(0x202020);
						tgl.draw_tiled("ground", scrx, scry);
					} else if (loc.terrain == t_terrain::wall) {
						tgl.color_binary(0x808080);
						tgl.draw_tiled("wall", scrx, scry);
					}
					// entities
					if (loc.entity == t_entity::player) {
						tgl.color_binary(0xd0d0d0);
						tgl.draw_tiled("player", scrx, scry);
					}
				}
			} else {
				tgl.color_binary(0x202020);
				tgl.draw_tiled("oob", scrx, scry);
			}
			scrx++;
		}
		scry++;
		scrx = 0;
	}
}
void sync_player()
{
	world.set(t_entity::none, player.getprevx(), player.getprevy());
	world.set(t_entity::player, player.getx(), player.gety());
}
void visit_surroundings()
{
	int x = player.getx();
	int y = player.gety();

	world.visit(x - 1, y - 1);
	world.visit(x + 0, y - 1);
	world.visit(x + 1, y - 1);
	world.visit(x - 1, y);
	world.visit(x, y);
	world.visit(x + 1, y);
	world.visit(x - 1, y + 1);
	world.visit(x + 0, y + 1);
	world.visit(x + 1, y + 1);
}
int main(int argc, char* argv[])
{
	tgl.window_gbc(0x000000, 5);
	screen.cols = tgl.cols();
	screen.rows = tgl.rows();

	init_tiles();
	init_world();

	player.setpos(5, 5);
	view.setscrl(0, 0);

	while (tgl.running()) {
		
		tgl.clear();
			sync_player();
			visit_surroundings();
			draw_world();
		tgl.system();

		if (tgl.kb_esc()) tgl.exit();
		int key = tgl.kb_lastkey();
		if (key == SDLK_RIGHT) player.move(1, 0);
		else if (key == SDLK_LEFT) player.move(-1, 0);
		else if (key == SDLK_DOWN) player.move(0, 1);
		else if (key == SDLK_UP) player.move(0, -1);
	}

	return tgl.exit();
}
