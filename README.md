# minesweeper
Terminal minesweeper game written in C for linux (with Windows support).

# Controls
| Key | Action |
|-----|--------|
| `w`/`a`/`s`/`d` or `h`/`j`/`k`/`l` | Move cursor (up/left/down/right) |
| `m` or `Enter` | Reveal tile |
| `f` | Flag/unflag tile |
| `q` or `Esc` | Quit |

# Build
Linux (requires `gcc` and `make`):
```
make
```

Windows cross-compile (requires `x86_64-w64-mingw32-gcc`):
```
make windows
```

Then run the game:
```
./minesweeper
```
