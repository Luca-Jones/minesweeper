class Game {

    static void Main(String[] args) {
        
        bool gameState = true;
        bool firstGame = true;
        ConsoleKey key;

        while (gameState) {

            Console.Clear();
            Console.Write("How big of a board?: ");
            String inputString = Console.ReadLine();
            int size;
            try {
                size = Int32.Parse(inputString);
            } catch(Exception e) {
                continue;
            }

            Console.Write("\nHow many mines?: ");
            
            inputString = Console.ReadLine();
            int mines;
            try {
                mines = Int32.Parse(inputString);
            } catch (Exception e) {
                continue;
            }
            // set up new game
            Field mineField = new(size, mines);

            while (true) {
                
                // show field
                mineField.Show();

                // read user input
                key = Console.ReadKey().Key;
                switch(key) {
                    case ConsoleKey.W:
                    case ConsoleKey.UpArrow:
                        mineField.MoveCursor(0,-1);
                        break;
                    case ConsoleKey.A:
                    case ConsoleKey.LeftArrow:
                        mineField.MoveCursor(-1,0);
                        break;
                    case ConsoleKey.S:
                    case ConsoleKey.DownArrow:
                        mineField.MoveCursor(0,1);
                        break;
                    case ConsoleKey.D:
                    case ConsoleKey.RightArrow:
                        mineField.MoveCursor(1,0);
                        break;
                    case ConsoleKey.Enter:
                        mineField.Mine(mineField.CursorX, mineField.CursorY);
                        break;
                    case ConsoleKey.F:
                        mineField.CurrentTile().flag();
                        break;
                    default:
                        break;
                }

                // checks win condition
                if (mineField.IsExposed()) {
                    Console.Clear();
                    Console.WriteLine("---YOU WON!---\nPlay again? (y/n)");
                    int input = Console.Read();
                    Console.Write(input);
                    gameState = input == 'y' || input == 'Y';
                    break;
                }

                // checks lose condition
                if (mineField.mineHit) {
                    Console.Clear();
                    Console.WriteLine("---GAME OVER---\nPlay again? (y/n)");
                    int input = Console.Read();
                    Console.Write(input);
                    gameState = input == 'y' || input == 'Y';
                    break;
                }

                // quit with ESC
                if (key == ConsoleKey.Escape) {
                    gameState = false;
                    break;
                }


            }
    
            firstGame = false;

        }

        Console.Clear();

    }

}