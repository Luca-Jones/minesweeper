using System.Security.Cryptography.X509Certificates;

public class Field {

    public List<Tile> Tiles;
    public int Size {get;set;}
    public int Mines;
    public int CursorX {get;set;}
    public int CursorY {get;set;}
    public bool mineHit {get;set;}

    public Field(int size, int mines) {
        this.Size = size;
        this.Mines = mines;
        this.mineHit = false;
        this.Tiles = [];
        this.InitField();
    }

    public void InitField() {
        
        // scramble the mines
        int[] array = new int[this.Size * this.Size];
        for (int i = 0; i < this.Size * this.Size; i ++) 
            array[i] = i;
        var random = new Random();
        random.Shuffle(array);

        // create each tile
        for(int i = 0; i < this.Size; i ++) {
            for (int j = 0; j < this.Size; j ++) {
                if (array[this.Size * i + j] < this.Mines) {
                    this.Tiles.Add(new Tile(j, i, "░░", true));
                } else {
                    this.Tiles.Add(new Tile(j, i, "░░", false));
                }
            }
        }

        // number each tile
        foreach (Tile mine in this.Tiles) {
            if (mine.isMine) {
                for (int dx = -1; dx <= 1; dx ++) {
                    for (int dy = -1; dy <= 1; dy ++) {
                        if (IsInBounds(mine.x + dx, mine.y + dy)) {
                            this.TileAt(mine.x + dx, mine.y + dy).adjacent += 1;
                        }
                    }
                }
            }
        }


    }

    public Tile TileAt(int x, int y) {
        return this.Tiles.ElementAt(this.Size * y + x);
    }

    public Tile CurrentTile() {
        return this.TileAt(this.CursorX, this.CursorY);
    }

    public void Show() {
        
        Console.Clear();

        this.TileAt(this.CursorX, this.CursorY).highlight();

        // title
        Console.WriteLine("---MINESWEEPER---");

        // top line
        Console.Write("╔═");
        for (int i = 0; i < this.Size; i ++) {
            Console.Write("══");
        }
        Console.Write("═╗\n");

        // main body
        for (int i = 0; i < this.Size; i ++) {
            Console.Write("║ ");
            for (int j = 0; j < this.Size; j ++) {
                Tile tile = this.TileAt(j, i);
                if (tile.highlighted) {
                    Console.Write("██");
                } else {
                    Console.Write("{0}", tile.c);
                }
            }
            Console.Write(" ║\n");
        }

        // bottom line
        Console.Write("╚═");
        for (int i = 0; i < this.Size; i ++) {
            Console.Write("══");
        }
        Console.Write("═╝\n");

    }

    public void MoveCursor(int dx, int dy) {
        if (this.IsInBounds(this.CursorX + dx, this.CursorY + dy)) {
                
                this.TileAt(this.CursorX, this.CursorY).highlighted = false;
                this.CursorX += dx;
                this.CursorY += dy;
        }
    }

    public void Mine(int x, int y) {

        Tile thisTile = this.TileAt(x, y);

        if (thisTile.highlighted) {
            
            if (thisTile.isMine) {
                this.mineHit = true;
                return;
            }

            if (thisTile.revealed) {

                return;

            } else if (thisTile.adjacent == 0) {

                MineHelper(thisTile, x, y);

            } else {

                thisTile.reveal();
                return;

            }

        } else {

            if (thisTile.revealed || thisTile.flagged || thisTile.isMine) {

                return;

            } else if (thisTile.adjacent == 0) {

                MineHelper(thisTile, x, y);

            } else {

                // reveal
                thisTile.reveal();
                return;

            }

        }

        return;

    }

    public void MineHelper(Tile thisTile, int x, int y) {
        
        thisTile.reveal();
        
        for (int dx = -1; dx <= 1; dx ++) {
            for (int dy = -1; dy <= 1; dy ++) {
                if (IsInBounds(x + dx, y + dy) && (dx == 0 || dy == 0 || this.TileAt(x + dx, y + dy).adjacent > 0)) {

                    Mine(x + dx, y + dy);

                }
            }
        }
    }

    public bool IsInBounds(int x, int y) {
        return x >= 0 && x < this.Size && y>= 0 && y < this.Size;
    }

    public bool IsExposed() {
        foreach(Tile t in this.Tiles) {

            if (t.isMine)
                continue;

            if (!t.revealed)
                return false;
                
        }
        return true;
    }

}


/*


        if (!thisTile.highlighted && (thisTile.revealed || thisTile.flagged))
            return;

        thisTile.reveal();
        
        if (thisTile.isMine) {
            this.mineHit = true;
            return;
        }

        if (thisTile.adjacent > 0 && !thisTile.highlighted) 
            return;

        if (thisTile.highlighted && thisTile.adjacent > 0) {

            for (int dx = -1; dx <= 1; dx ++) {
                for (int dy = -1; dy <= 1; dy ++) {
                    if (IsInBounds(x + dx, y + dy)) {
                        this.Mine(x + dx, y + dy);
                    }
                }
            }

        } else if (thisTile.adjacent == 0) {        

            int[] dx = {-1,0,0,1};
            int[] dy = {0,1,-1,0};

            for (int i = 0; i < 4; i ++) {
                
                if (this.IsInBounds(x + dx[i], y + dy[i])) {

                    Tile nextTile = this.TileAt(x + dx[i], y + dy[i]);

                    if (!nextTile.isMine) {
                        this.Mine(x + dx[i], y + dy[i]);
                    }
                    
                }
            }
        }

*/