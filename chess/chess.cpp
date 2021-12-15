// chess.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>

enum Colour {
    White = 0, Black = 1, Empty = 2
};

enum Pieces {
    Pawn, Knight, Bishop, Rook, Queen, King, Nothing
};

enum Coords {
    a, b, c, d, e, f, g, h
};

class Piece {
    int colour = Empty; 
    int piece = Nothing;
    int x, y;
    int moveNum = 0;
    int enPassant = 0; //keeps the move number this was made at so that it can be checked if this matches board move number -1 and piece movenum == 1, can be captured with en passant
    int attackedByWhite, attackedByBlack, pinnedByWhite, pinnedByBlack;
public:
     char virtual getPiece() {

         switch (piece) {
         case Pawn: if (colour == Black) { return 'p'; } else return 'P';
             break;
         case Rook: if (colour == Black) { return 'r'; }
                  else return 'R';
             break;
         case Bishop: if (colour == Black) { return 'b'; }
                    else return 'B';
             break;
         case Queen: if (colour == Black) { return 'q'; }
                    else return 'Q';
             break;
         case King: if (colour == Black) { return 'k'; }
                    else return 'K';
             break;
         case Knight: if (colour == Black) { return 'h'; }
                    else return 'H';
             break;
         case Nothing: return ' ';
             break;
         
         }
    }

     void virtual getCoords(int& xCoord, int& yCoord) {
         xCoord = x;
         yCoord = y;
     }

     void virtual setCoords(int xCoord, int yCoord) {
         x = xCoord;
         y = yCoord;
     }

     void setPiece(Pieces p, Colour c) {
         piece = p;
         colour = c;
     }

     int getColour() {
         return colour;
     }

     int getMoveNumber() {
         return moveNum;
     }

     void setMoveNumber(int m) {
         moveNum = m + 1;
     }

     void setEnPassant(int e) {
         enPassant = e;
     }

     int getEnPassant() {
         return enPassant;
     }

     void setAttack(int colour) {
         if (colour == White) {
             attackedByWhite += 1;
         }
         else attackedByBlack += 1;
     }

     void setPinned(int colour) {
         if (colour == White) {
             pinnedByWhite += 1;
         }
         else pinnedByBlack += 1;
     }

     void clearAttack() {
         attackedByBlack = 0;
         attackedByWhite = 0;
         pinnedByWhite = 0;
         pinnedByBlack = 0;
     }

     bool underAttack(int colour) {
         if (colour == White && attackedByBlack > 0) {
             return true;
         }
         if (colour == Black && attackedByWhite > 0) {
             return true;
         }
         return false;
     }

     bool pinned(int colour) {
         if (colour == White && pinnedByBlack > 0) {
             return true;
         }
         if (colour == Black && pinnedByWhite > 0) {
             return true;
         }
         return false;
     }

     void testAttack() {
         std::cout << "Black : " << attackedByBlack << " White : " << attackedByWhite << "\n";
     }
};

class Board {
public:
    Piece grid[8][8];
    int moveNum = 0;
    std::vector<Piece> piecePositions;
    std::string fenInput;
    Board() {
        //p2->setColour();

        int tempChoice;
        std::cout << "1. New Game: \n2. FEN: \n";
        std::cin >> tempChoice;
        std::cout << "\n";

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                
                
                grid[i][j].setCoords(i, j);
            }
        }


        if(tempChoice != 2){
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {

                    if (i == 1) {
                        grid[i][j].setPiece(Pawn, Black);

                    }
                    if (i == 6) {
                        grid[i][j].setPiece(Pawn, White);

                    }
                }
            }

            //rooks
            grid[0][0].setPiece(Rook, Black);
            grid[0][7].setPiece(Rook, Black);
            grid[7][0].setPiece(Rook, White);
            grid[7][7].setPiece(Rook, White);
            //bishops
            grid[0][2].setPiece(Bishop, Black);
            grid[0][5].setPiece(Bishop, Black);
            grid[7][2].setPiece(Bishop, White);
            grid[7][5].setPiece(Bishop, White);
            //Knights
            grid[0][1].setPiece(Knight, Black);
            grid[0][6].setPiece(Knight, Black);
            grid[7][1].setPiece(Knight, White);
            grid[7][6].setPiece(Knight, White);
            //Queens
            grid[0][3].setPiece(Queen, Black);
            grid[7][3].setPiece(Queen, White);
            //kings
            grid[0][4].setPiece(King, Black);
            grid[7][4].setPiece(King, White);
        }
        else {
            std::cout << "input FEN code: ";
            std::cin >> fenInput;
            std::cout << "\n";
            int counter1 = 0, counter2 = 0;
            bool endOfPieces = false;
            bool whiteToPlay = false;
            for (int i = 0; i < fenInput.size(); i++)
            {
                switch (fenInput[i])
                {
                default:
                    break;
                case 'r': grid[counter2][counter1].setPiece(Rook, Black); counter1++;
                    break;
                case 'n': grid[counter2][counter1].setPiece(Knight, Black); counter1++;
                    break;
                case 'b': if (endOfPieces == false) { grid[counter2][counter1].setPiece(Bishop, Black); counter1++; }
                    break;
                case 'q': grid[counter2][counter1].setPiece(Queen, Black); counter1++;
                    break;
                case 'k': grid[counter2][counter1].setPiece(King, Black); counter1++;
                    break;
                case 'p': grid[counter2][counter1].setPiece(Pawn, Black); counter1++;
                    break;
                case '/': counter1 = 0; counter2++;
                    break;
                case 'R': grid[counter2][counter1].setPiece(Rook, White); counter1++;
                    break;
                case 'N': grid[counter2][counter1].setPiece(Knight, White); counter1++;
                    break;
                case 'B': grid[counter2][counter1].setPiece(Bishop, White); counter1++;
                    break;
                case 'Q': grid[counter2][counter1].setPiece(Queen, White); counter1++;
                    break;
                case 'K': grid[counter2][counter1].setPiece(King, White); counter1++;
                    break;
                case 'P': grid[counter2][counter1].setPiece(Pawn, White); counter1++;
                    break;
                case '1': case '2': case '3': case '4': case '5': case'6': case '7': case '8': counter1 += fenInput[i] -48;
                    break;
                }
            }
        }
        

        
    }

    void resetList() {

       piecePositions.clear();

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (grid[i][j].getPiece() != ' ') {
                    piecePositions.push_back(grid[i][j]);
                    
                }
                grid[i][j].clearAttack();
            }
        }
    }

    void displayBoard() {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                std::cout << " ___";
            }
            std::cout << std::endl;
            for (int j = 0; j < 8; j++)
            {
                std::cout << "| " << grid[i][j].getPiece() << " ";
            }
            std::cout << "|  " << 8 - i << "\n";
        }
        for (int i = 0; i < 8; i++)
        {
            std::cout << " ___";
        }

        std::cout << "\n\n  a   b   c   d   e   f   g   h\n";
        std::cout << "\n";
        if (moveNum % 2 == 0) {
            std::cout << "White's turn!";
        }
        else {
            std::cout << "Black's turn!";
        }
        
    }

    void move() {


        std::string playerMove = "";
        int srcColour;
        int dstColour;
        int x1, x2, y1, y2;
            std::cout << "\n\n\n\n\n\n\n\n\n";
            std::cin >> playerMove;
            std::cout << "\n\n\n\n\n\n";

            if (playerMove.length() == 4) {
                y1 = (playerMove[0] - '0') -1;
                x1 = 8 - (playerMove[1] - '0');
                y2 = (playerMove[2] - '0') -1;
                x2 = 8 - (playerMove[3] - '0');

                if (y1 >= 48) {
                    y1 -= 48;
                }
                if (y2 >= 48) {
                    y2 -= 48;
                }

                srcColour = grid[x1][y1].getColour();
                dstColour = grid[x2][y2].getColour();
                if (srcColour == (moveNum % 2) && dstColour != (moveNum % 2)) {

                    switch (grid[x1][y1].getPiece()) {
                        case 'p' : case 'P' : if (movePawn(grid[x1][y1], grid[x2][y2])) {
                            moveNum++;
                        }
                                  break;
                        case 'r' : case 'R' : if (moveRook(grid[x1][y1], grid[x2][y2])) {
                            moveNum++;
                        }
                                 break;
                        case 'b': case 'B': if (moveBishop(grid[x1][y1], grid[x2][y2])) {
                            moveNum++;
                        }
                                break;
                        case 'h': case 'H': if (moveKnight(grid[x1][y1], grid[x2][y2])) {
                            moveNum++;
                        }
                                break;
                        case 'q': case 'Q': if (moveQueen(grid[x1][y1], grid[x2][y2])) {
                            moveNum++;
                        }
                                break;
                        case 'k': case 'K': if (moveKing(grid[x1][y1], grid[x2][y2])) {
                            moveNum++;
                        }
                                break;
                        
                    }
                    
                    
                }
                
            


        }
        
        //testCoords();
    }

    bool movePawn(Piece src, Piece dst) {
        int x1, y1, x2, y2, moves, colour1, colour2;

        src.getCoords(x1, y1);
        dst.getCoords(x2, y2);
        moves = src.getMoveNumber();
        colour1 = src.getColour();
        colour2 = dst.getColour();


        grid[1][2].testAttack();

        if (((x1 + 1 == x2) || (x1 + 2 == x2 && moves == 0)) && colour1 == Black) {
            if ((y1 + 1 == y2 || y1 - 1 == y2) && colour2 == White && x1 + 1 == x2) {
                grid[x2][y2].setPiece(Pawn, Black);
                grid[x2][y2].setMoveNumber(moves);
                grid[x2][y2].setCoords(x2, y2);
                grid[x1][y1].setPiece(Nothing, Empty);

                return true;
            }
            else if (y1 == y2 && colour2 == Empty) {
                grid[x2][y2].setPiece(Pawn, Black);
                grid[x2][y2].setMoveNumber(moves);
                grid[x2][y2].setCoords(x2, y2);
                grid[x1][y1].setPiece(Nothing, Empty);
                std::cout << "moves : " << moves << "\n";
                if (x1 + 2 == x2) {
                    grid[x2][y2].setEnPassant(moveNum);
                }
                return true;               
            }
            else if ((y1 + 1 == y2 || y1 - 1 == y2) && grid[x2-1][y2].getColour() == White) {  //enPassant
                if (grid[x2 - 1][y2].getEnPassant() == moveNum - 1 && grid[x2 - 1][y2].getMoveNumber() == 1) {
                    grid[x2][y2].setPiece(Pawn, Black);
                    grid[x2][y2].setMoveNumber(moves);
                    grid[x2][y2].setCoords(x2, y2);
                    grid[x1][y1].setPiece(Nothing, Empty);
                    grid[x2-1][y2].setPiece(Nothing, Empty);
                    return true;
                }
            }
        }

        if (((x1 - 1 == x2) || (x1 - 2 == x2 && moves == 0)) && colour1 == White) {
            if ((y1 + 1 == y2 || y1 - 1 == y2) && colour2 == Black && x1 - 1 == x2) {
                grid[x2][y2].setPiece(Pawn, White);
                grid[x2][y2].setMoveNumber(moves);
                grid[x2][y2].setCoords(x2, y2);
                grid[x1][y1].setPiece(Nothing, Empty);
                return true;
            }
            else if(y1 == y2 && colour2 == Empty) {
                grid[x2][y2].setPiece(Pawn, White);
                grid[x2][y2].setMoveNumber(moves);
                grid[x2][y2].setCoords(x2, y2);
                grid[x1][y1].setPiece(Nothing, Empty);
                std::cout << "moves : " << moves << "\n";
                if (x1 - 2 == x2) {
                    grid[x2][y2].setEnPassant(moveNum);
                }
                return true;
            }           
        }

        return false;
    }

    bool moveRook(Piece src, Piece dst) {
        int x1, y1, x2, y2, moves, colour1, colour2;
        src.getCoords(x1, y1);
        dst.getCoords(x2, y2);
        moves = src.getMoveNumber();
        colour1 = src.getColour();
        colour2 = dst.getColour();



        /* 
        cleaner movement code potential solution
        var1 = destination - source
           if(var < 0) var = var / var *-1
           else var = var / var

        do twice to get x and y

        if(x == 0 and y !== 0 || y == 0 and x != 0) //rook
        if(x == y || x *-1 == y) //bishop
        */

        if ((x1 != x2 && y1 == y2) || (x1 == x2 && y1 != y2)) {
            if (x1 != x2) {
                int lowest, highest;
                if (x1 > x2) {
                    highest = x1;
                    lowest = x2;
                }
                else {
                    highest = x2;
                    lowest = x1;
                }
                for (int i = lowest+1; i < highest; i++)
                {
                    if (grid[i][y1].getPiece() != ' ') return false;
                }
            }

            if (y1 != y2) {
                int lowest, highest;
                if (y1 > y2) {
                    highest = y1;
                    lowest = y2;
                }
                else {
                    highest = y2;
                    lowest = y1;
                }
                for (int i = lowest + 1; i < highest; i++)
                {
                    if (grid[x1][i].getPiece() != ' ') return false;
                }
            }

            if (colour1 == White) {
                grid[x2][y2].setPiece(Rook, White);
            }
            else {
                grid[x2][y2].setPiece(Rook, Black);
            }
            
            grid[x2][y2].setMoveNumber(moves);
            grid[x2][y2].setCoords(x2, y2);
            grid[x1][y1].setPiece(Nothing, Empty);
            std::cout << "moves : " << moves << "\n";
            return true;

        }
    }

    bool moveBishop(Piece src, Piece dst) {
        int x1, y1, x2, y2, moves, colour1, colour2;
        src.getCoords(x1, y1);
        dst.getCoords(x2, y2);
        moves = src.getMoveNumber();
        colour1 = src.getColour();
        colour2 = dst.getColour();

        int xDifference, yDifference;
        if (x1 > x2) {
            xDifference = x1 - x2;
        }
        else {
            xDifference = x2 - x1;
        }

        if (y1 > y2) {
            yDifference = y1 - y2;
        }
        else {
            yDifference = y2 - y1;
        }

        if (xDifference == yDifference) {

            if (x1 > x2) {
                if (y1 > y2) {
                    for (int i = 1; i < xDifference; i++)
                    {
                        if (grid[x1 - i][y1 - i].getPiece() != ' ') return false;
                    }
                }
                else {
                    for (int i = 1; i < xDifference; i++)
                    {
                        if (grid[x1 - i][y1 + i].getPiece() != ' ') return false;
                    }
                }
            }
            else {
                if (y1 > y2) {
                    for (int i = 1; i < xDifference; i++)
                    {
                        if (grid[x1 + i][y1 - i].getPiece() != ' ') return false;
                    }
                }
                else {
                    for (int i = 1; i < xDifference; i++)
                    {
                        if (grid[x1 + i][y1 + i].getPiece() != ' ') return false;
                    }
                }
            }



            

            if (colour1 == White) {
                grid[x2][y2].setPiece(Bishop, White);
            }
            else {
                grid[x2][y2].setPiece(Bishop, Black);
            }

            grid[x2][y2].setMoveNumber(moves);
            grid[x2][y2].setCoords(x2, y2);
            grid[x1][y1].setPiece(Nothing, Empty);
            std::cout << "moves : " << moves << "\n";
            return true;
        }
        return false;
    }

    bool moveKnight(Piece src, Piece dst) {
        int x1, y1, x2, y2, moves, colour1, colour2;
        src.getCoords(x1, y1);
        dst.getCoords(x2, y2);
        moves = src.getMoveNumber();
        colour1 = src.getColour();
        colour2 = dst.getColour();


        if ((x1 + 2 == x2 && y1 + 1 == y2) || (x1 + 2 == x2 && y1 - 1 == y2) || (x1 - 2 == x2 && y1 + 1 == y2) || (x1 - 2 == x2 && y1 - 1 == y2) || (y1 + 2 == y2 && x1 + 1 == x2) || (y1 + 2 == y2 && x1 - 1 == x2) || (y1 - 2 == y2 && x1 + 1 == x2) || (y1 - 2 == y2 && x1 - 1 == x2)) {
            if (colour1 == White) {
                grid[x2][y2].setPiece(Knight, White);
            }
            else {
                grid[x2][y2].setPiece(Knight, Black);
            }

            grid[x2][y2].setMoveNumber(moves);
            grid[x2][y2].setCoords(x2, y2);
            grid[x1][y1].setPiece(Nothing, Empty);
            std::cout << "moves : " << moves << "\n";
            return true;
        }
        return false;
    }

    bool moveQueen(Piece src, Piece dst) {
        int x1, y1, x2, y2, moves, colour1, colour2;
        src.getCoords(x1, y1);
        dst.getCoords(x2, y2);
        moves = src.getMoveNumber();
        colour1 = src.getColour();
        colour2 = dst.getColour();

        if ((x1 != x2 && y1 == y2) || (x1 == x2 && y1 != y2)) {
            if (x1 != x2) {
                int lowest, highest;
                if (x1 > x2) {
                    highest = x1;
                    lowest = x2;
                }
                else {
                    highest = x2;
                    lowest = x1;
                }
                for (int i = lowest + 1; i < highest; i++)
                {
                    if (grid[i][y1].getPiece() != ' ') return false;
                }
            }

            if (y1 != y2) {
                int lowest, highest;
                if (y1 > y2) {
                    highest = y1;
                    lowest = y2;
                }
                else {
                    highest = y2;
                    lowest = y1;
                }
                for (int i = lowest + 1; i < highest; i++)
                {
                    if (grid[x1][i].getPiece() != ' ') return false;
                }
            }

            if (colour1 == White) {
                grid[x2][y2].setPiece(Queen, White);
            }
            else {
                grid[x2][y2].setPiece(Queen, Black);
            }

            grid[x2][y2].setMoveNumber(moves);
            grid[x2][y2].setCoords(x2, y2);
            grid[x1][y1].setPiece(Nothing, Empty);
            std::cout << "moves : " << moves << "\n";
            return true;
        }

            int xDifference, yDifference;
            if (x1 > x2) {
                xDifference = x1 - x2;
            }
            else {
                xDifference = x2 - x1;
            }

            if (y1 > y2) {
                yDifference = y1 - y2;
            }
            else {
                yDifference = y2 - y1;
            }

            if (xDifference == yDifference) {

                if (x1 > x2) {
                    if (y1 > y2) {
                        for (int i = 1; i < xDifference; i++)
                        {
                            if (grid[x1 - i][y1 - i].getPiece() != ' ') return false;
                        }
                    }
                    else {
                        for (int i = 1; i < xDifference; i++)
                        {
                            if (grid[x1 - i][y1 + i].getPiece() != ' ') return false;
                        }
                    }
                }
                else {
                    if (y1 > y2) {
                        for (int i = 1; i < xDifference; i++)
                        {
                            if (grid[x1 + i][y1 - i].getPiece() != ' ') return false;
                        }
                    }
                    else {
                        for (int i = 1; i < xDifference; i++)
                        {
                            if (grid[x1 + i][y1 + i].getPiece() != ' ') return false;
                        }
                    }
                }



            

                if (colour1 == White) {
                    grid[x2][y2].setPiece(Queen, White);
                }
                else {
                    grid[x2][y2].setPiece(Queen, Black);
                }

                grid[x2][y2].setMoveNumber(moves);
                grid[x2][y2].setCoords(x2, y2);
                grid[x1][y1].setPiece(Nothing, Empty);
                std::cout << "moves : " << moves << "\n";
                return true;
            }
            return false;
        
    }

    bool moveKing(Piece src, Piece dst) {
        int x1, y1, x2, y2, moves, colour1, colour2;
        src.getCoords(x1, y1);
        dst.getCoords(x2, y2);
        moves = src.getMoveNumber();
        colour1 = src.getColour();
        colour2 = dst.getColour();

        if ((x1 - x2 == 1 || x1 - x2 == -1 || x1 - x2 == 0) && (y1 - y2 == 1 || y1 - y2 == -1 ||y1 - y2 ==  0) && !grid[x2][y2].underAttack(colour1)) {
            if (x1 - x2 == 0 && y1 - y2 == 0) {
                return false;
            }
            if (colour1 == White) {
                grid[x2][y2].setPiece(King, White);
            }
            else {
                grid[x2][y2].setPiece(King, Black);
            }

            grid[x2][y2].setMoveNumber(moves);
            grid[x2][y2].setCoords(x2, y2);
            grid[x1][y1].setPiece(Nothing, Empty);
            std::cout << "moves : " << moves << "\n";
            return true;
        }
        return false;
    }

    void calculatePawnAttack(Piece src) {
        int colour = src.getColour();
        int x, y;
        src.getCoords(x, y);
        if (colour == White) {
            if (y > 0) {
             grid[x - 1][y - 1].setAttack(colour);
            }
            
            if (y < 7) {
                grid[x - 1][y + 1].setAttack(colour);
            }
            
        }
        else if (colour == Black) {
            if (y > 0) {
                grid[x + 1][y - 1].setAttack(colour);
            }
            if (y < 7) {
                grid[x + 1][y + 1].setAttack(colour);
            }
            
        }
    }

    void calculateRookAttack(Piece src) {
        int colour = src.getColour();
        bool foundKing = false;
        int x, y, counter = 0;
        int xDirection, yDirection;
        src.getCoords(x, y);

        for (int i = 0; i < 4; i++) //loop the 4 directions
        {
            if (i == 0) {
                xDirection = 1;
                yDirection = 0;
            }
            if (i == 1) {
                xDirection = 0;
                yDirection = -1;
            }
            if (i == 2) {
                xDirection = -1;
                yDirection = 0;
            }
            if (i == 3) {
                xDirection = 0;
                yDirection = 1;
            }

            foundKing = false;
            for (int j = 0; j < 2; j++) //loop through twice to check whether king is in the line
            {
                counter = 0;
                
                for (int k = 0; k < 8; k++) //loop through each square
                {

                    if (x + k * xDirection < 8 && x + k * xDirection >= 0 && y + k * yDirection < 8 && y + k * yDirection >= 0) {

                        if (counter <= 1) {
                            grid[x + k * xDirection][y + k * yDirection].setAttack(colour);
                        }
                        if (counter <= 2 && foundKing == true) {
                            grid[x + k * xDirection][y + k * yDirection].setPinned(colour);
                        }

                        if ((grid[x + k * xDirection][y + k * yDirection].getPiece() == 'k' && colour == White) || (grid[x + k * xDirection][y + k * yDirection].getPiece() == 'K' && colour == Black)) {
                            if (counter <= 2) {
                                foundKing = true;
                            }
                        }

                        if (grid[x + k * xDirection][y + k * yDirection].getPiece() != ' ') {
                            counter++; 
                        }
                    }
                }
            }
        }
    }

    void calculateBishopAttack(Piece src) {
        int colour = src.getColour();
        int x, y, counter = 0;
        src.getCoords(x, y);
        int xDirection, yDirection;
        bool foundKing = false;
        for (int i = 0; i < 4; i++) //loop the 4 directions
        {
            if (i == 0) {
                xDirection = 1;
                yDirection = 1;
            }
            if (i == 1) {
                xDirection = 1;
                yDirection = -1;
            }
            if (i == 2) {
                xDirection = -1;
                yDirection = 1;
            }
            if (i == 3) {
                xDirection = -1;
                yDirection = -1;
            }

            foundKing = false;
            for (int j = 0; j < 2; j++) //loop through twice to check whether king is in the line
            {
                counter = 0;

                for (int k = 0; k < 8; k++) //loop through each square
                {

                    if (x + k * xDirection < 8 && x + k * xDirection >= 0 && y + k * yDirection < 8 && y + k * yDirection >= 0) { //check whether square is inbounds

                        if (counter <= 1) {
                            grid[x + k * xDirection][y + k * yDirection].setAttack(colour);
                        }
                        if (counter <= 2 && foundKing == true) {
                            grid[x + k * xDirection][y + k * yDirection].setPinned(colour);
                        }

                        if ((grid[x + k * xDirection][y + k * yDirection].getPiece() == 'k' && colour == White) || (grid[x + k * xDirection][y + k * yDirection].getPiece() == 'K' && colour == Black)) {
                            if (counter <= 2) {
                                foundKing = true;
                            }
                        }

                        if (grid[x + k * xDirection][y + k * yDirection].getPiece() != ' ') {
                            counter++;
                        }
                    }
                }
            }
        }
    }

    void calculateKnightAttack(Piece src) {
        int colour = src.getColour();
        int x, y, counter = 0;
        src.getCoords(x, y);



        if (x - 1 >= 0 && y - 2 >= 0) {
            grid[x - 1][y - 2].setAttack(colour);
        }
        if (x - 2 >= 0 && y - 1 >= 0) {
            grid[x - 2][y - 1].setAttack(colour);
        }

        if (x + 1 < 8 && y - 2 >= 0) {
            grid[x + 1][y - 2].setAttack(colour);
        }
        if (x + 2 < 8 && y - 1 >= 0) {
            grid[x + 2][y - 1].setAttack(colour);
        }

        if (x + 1 < 8 && y + 2 < 8) {
            grid[x + 1][y + 2].setAttack(colour);
        }
        if (x + 2 < 8 && y + 1 < 8) {
            grid[x + 2][y + 1].setAttack(colour);
        }

        if (x - 1 >= 0 && y + 2 < 8) {
            grid[x - 1][y + 2].setAttack(colour);
        }
        if (x - 2 >= 0 && y + 1 < 8) {
            grid[x - 2][y + 1].setAttack(colour);
        }


        

    }

    void calculateKingAttack(Piece src) {
        int colour = src.getColour();
        int x, y, counter = 0;
        src.getCoords(x, y);

        if (x - 1 >= 0) {
            grid[x - 1][y].setAttack(colour);
            if (y - 1 >= 0 ) {
                grid[x - 1][y - 1].setAttack(colour);
            }
        }

        if (y - 1 >= 0) {
            grid[x][y - 1].setAttack(colour);
            if (x + 1 < 8) {
                grid[x + 1][y - 1].setAttack(colour);
            }
        }

        if (x + 1 < 8) {
            grid[x + 1][y].setAttack(colour);
            if (y + 1 < 8) {
                grid[x + 1][y + 1].setAttack(colour);
            }
        }

        if (y + 1 < 8) {
            grid[x][y + 1].setAttack(colour);

            if (x - 1 >= 0) {
                grid[x - 1][y + 1].setAttack(colour);
            }
        }
        
    }

    void calculateAttackedSquares() {
        for (int i = 0; i < piecePositions.size(); i++)
        {
            switch (piecePositions[i].getPiece()) {
            case 'p': case 'P': calculatePawnAttack(piecePositions[i]);
                    break;
            case 'r': case 'R': calculateRookAttack(piecePositions[i]);
                    break;
            case 'b': case 'B': calculateBishopAttack(piecePositions[i]);
                    break;
            case 'h': case 'H': calculateKnightAttack(piecePositions[i]); 
                    break;
            case 'q': case 'Q': calculateRookAttack(piecePositions[i]); calculateBishopAttack(piecePositions[i]);
                    break;
            case 'k': case 'K': calculateKingAttack(piecePositions[i]);
                    break;
            }
        }
    }

    void testAttacked() {
        int x, y;
        std::cout << "\n";
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {

                std::cout << "  |  ";
                if (grid[i][j].underAttack(Black)) {
                    std::cout << "W";
                }

                if (grid[i][j].underAttack(White)) {
                    std::cout << "B";
                }

                if (!grid[i][j].underAttack(White) && !grid[i][j].underAttack(Black)) {
                    std::cout << " ";
                }
                
            }
            std::cout << "  |\n  ";
            for (int j = 0; j < 8; j++)
            {
                std::cout << "  --- ";
            }
            std::cout << "  |\n";
        }
    }

    void testPinned() {
        int x, y;
        std::cout << "\n";
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {

                std::cout << "  |  ";
                if (grid[i][j].pinned(Black)) {
                    std::cout << "W";
                }

                if (grid[i][j].pinned(White)) {
                    std::cout << "B";
                }

                if (!grid[i][j].pinned(White) && !grid[i][j].pinned(Black)) {
                    std::cout << " ";
                }

            }
            std::cout << "  |\n  ";
            for (int j = 0; j < 8; j++)
            {
                std::cout << "  --- ";
            }
            std::cout << "  |\n";
        }
    }
};

class gameManager {
    Board b;
    bool playing = true;
public:
    void driver() {
        b.displayBoard();
        while (playing == true) {
            b.resetList();
            b.calculateAttackedSquares();
            b.move();
            b.displayBoard();
            b.testPinned();
            //b.testAttacked();
        }
        
    }
};


int main()
{

     gameManager game;
     game.driver();

}

