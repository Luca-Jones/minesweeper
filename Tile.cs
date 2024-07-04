public class Tile {
        public int x {get;set;}
        public int y {get;set;}
        public String c {get;set;}
        public int adjacent {get;set;}
        public bool isMine {get;set;}
        public bool revealed {get;set;}
        public bool highlighted {get;set;}
        public bool flagged {get;set;}

        public Tile(int x, int y, String c, bool m) {
            this.x = x;
            this.y = y;
            this.c = c;
            this.adjacent = 0;
            this.isMine = m;
            this.revealed = false;
            this.highlighted = false;
            this.flagged = false;
        }

        public Tile(Tile t) {
            this.x = t.x;
            this.y = t.y;
            this.c = t.c;
            this.adjacent = t.adjacent;
            this.isMine = t.isMine;
            this.revealed = t.revealed;
            this.highlighted = t.highlighted;
            this.flagged = t.flagged;
        }

        public void reveal() {
            
            if (this.revealed)
                return;
            
            this.revealed = true;
            this.flagged = false;

            if (this.isMine) {
                this.c = "X ";
            } else {
                this.c = this.adjacent.ToString() + " ";
            }
        }

        public void highlight() {
            this.highlighted = true;
        }

        public void flag() {
            if (!this.revealed) {
                this.flagged = true;
                this.c = "║▀";
            }
        }

    }