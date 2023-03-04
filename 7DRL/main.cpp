#include <TGL.h>

int main(int argc, char* argv[])
{
	TGL tgl;

	tgl.window(256, 192, 0xffffff, 4);
	tgl.title("7DRL 2023");

	while (tgl.running()) {
		tgl.clear();
		tgl.print_tiled("Hello World!", 1, 1);
		tgl.system();
	}

	return 0;
}
