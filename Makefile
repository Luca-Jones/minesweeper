default:
	gcc main.c input.c screen.c tile.c -o minesweeper

windows:
	x86_64-w64-mingw32-gcc main.c input.c screen.c tile.c -o minesweeper.exe
