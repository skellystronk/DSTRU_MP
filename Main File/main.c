#include <stdio.h>
#include <stdbool.h>
void Replace(int Rpos, int Cpos, bool *go, bool *found, int R[3][3], int B[3][3], int S[3][3], int T[3][3]);
void Expand(int Rpos, int Cpos, bool *found, bool *go, int R[3][3], int B[3][3], int S[3][3], int T[3][3]);
int countSet(int set[3][3]);

//deletes position of the pieces(?)
void removePos(int Rpos, int Cpos, bool *go, int R[3][3], int B[3][3], int S[3][3], int T[3][3])
{

    if (Rpos >= 0 && Rpos < 3 && Cpos >= 0 && Cpos < 3)
    {
        //if Red's Turn
        if (*go == true)
            R[Rpos][Cpos] = 0;  //remove what is in that position

        //Blue's turn
        else
            B[Rpos][Cpos] = 0;  //remove what is in that position

        S[Rpos][Cpos] = 0;
        T[Rpos][Cpos] = 0;
    }
}

void Expand(int Rpos, int Cpos, bool *found, bool *go, int R[3][3], int B[3][3], int S[3][3], int T[3][3])
{
    //explode
    removePos(Rpos, Cpos, go, R, B, S, T);

    //MOVEMENT
    if (*go)
        Replace(Rpos - 1, Cpos, go, found, R, B, S, T); //UP
    
    else
        Replace(Rpos + 1, Cpos, go, found, R, B, S, T); //DOWN
    
    Replace(Rpos, Cpos - 1, go, found, R, B, S, T); //LEFT
    Replace(Rpos, Cpos + 1, go, found, R, B, S, T); //RIGHT
}

//this function lets R and B to capture each other
void Replace(int Rpos, int Cpos, bool *go, bool *found, int R[3][3], int B[3][3], int S[3][3], int T[3][3])
{
    if (Cpos >= 0 && Cpos < 3 && Rpos >= 0 && Rpos < 3)
    {
        //stated in specs
        *found = false;

        //red's turn
        if (*go == 1)
        {
            //if red will move to a spot taken by blue
            if (B[Rpos][Cpos] == 1) //if space is taken by blue
            {
                B[Rpos][Cpos] = 0;  //remove/capture blue piece
                *found = true;         //i still dont know how found helps in expand
            }

            //if red will only move
            else if (R[Rpos][Cpos] == 0)    //spot isn't taken
                R[Rpos][Cpos] = 1;          //take that spot
            

            else if (R[Rpos][Cpos] == 1)    //spot is taken by red
                *found = true;              //add to counter of expand(?)
        }

        //blue's turn
        else
        {
            //if blue will move to a spot taken by red
            if (R[Rpos][Cpos] == 1) //if space is taken by red
            {
                R[Rpos][Cpos] = 0;  //remove/capture red piece
                *found = true;
            }

            //if blue will only move
            else if (B[Rpos][Cpos] == 0)    //spot isnt taken
                B[Rpos][Cpos] = 1;          //take spot
            

            else if (B[Rpos][Cpos] == 1)
                *found = true;
        }

        //for S T and expand(?)
        if (*found == 1)
        {
            //baks di ko gets
            if (S[Rpos][Cpos] == 0)
            {
                S[Rpos][Cpos] = 1; // S = S U {pos} ^ found
                *found = 0;        // found = false
            }

            //baks di ko gets
            else if (S[Rpos][Cpos] == 1 && T[Rpos][Cpos] == 0)
            {
                T[Rpos][Cpos] = 1; // T = T U {pso}
                Expand(Rpos, Cpos, found, go, R, B, S, T);
            }
        }
    }
}


void Update(int Rpos, int Cpos, bool *good, bool *go, bool *found, int R[3][3], int B[3][3], int S[3][3], int T[3][3])
{
    //Stated in specs (refresh)
    *good = false;

    if (S[Rpos][Cpos] == 0) // if S is not visited
    {
        S[Rpos][Cpos] = 1; // S is now visited
        *good = !(*good); // set good to true
    }

    else if (!(*good) && S[Rpos][Cpos] == 1 && T[Rpos][Cpos] == 0) // if *good = true and S is visited and T is not triggered
    {
        T[Rpos][Cpos] = 1; // T is now triggered
        Expand(Rpos, Cpos, found, go, R, B, S, T);
    }
}

void NextPlayerMove(int Rpos, int Cpos, int R[3][3], int B[3][3], int S[3][3], int T[3][3],
                    bool *good, bool *go, int *val, bool *start, bool over, bool *found)
{
    // Case 1 Red's turn
    if (!over && *start && *go)
    {
        R[Rpos][Cpos] = 1;     // Place Red piece
        S[Rpos][Cpos] = 1;     // Mark visited
        *good = true;
    }

    // Case 2: Blue's turn
    else if (!over && *start && !*go)
    {
        B[Rpos][Cpos] = 1;     // Place Blue piece
        S[Rpos][Cpos] = 1;     // Mark S visited
        *good = true;
    }

    // Case 3 : starting phase
    if (!over && !(*start) && ((*go && R[Rpos][Cpos]) || (!*go && B[Rpos][Cpos])))
    {
        Update(Rpos, Cpos, good, go, found, R, B, S, T); // calling update function
        *good = true;
    }

    // Case 4: players moved at least once
    if (countSet(R) >= 1 && countSet(B) >= 1)
    {
        *start = false;
    }

    // Case 5: After a good move
    if (!over && *good)
    {
        *good = false;        // Reset flag
        *go = !(*go);            // Switch turn to the other player
        *val = *val + 1;        // Increase turn counter
    }
}


//this function counts the score of each player
int countSet(int set[3][3])
{
    int i, j;
    int count = 0;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (set[i][j] == 1)
                count++;
        }
    }

    return count;
}

//this function tells if the game is over
void GamerOver(int val, int R[3][3], int B[3][3])
{
    int countR = countSet(R);   //counts score of Red
    int countB = countSet(B);   //counts score of Blue

    bool over = (val >= 20 || (countR > 0 && countB == 0) || (countR == 0 && countB > 0));

    if(over)
    {
        if (countR > countB)        //if Red has higher score, Red wins
            printf("R wins\n");

        else if (countR < countB)   //if Blue has higher score, Blue wins
            printf("B wins\n");

        else if (countR == countB)  //if Red amd Blue have the same score, draw
            printf("draw\n");
    }
}

//checks if game is still valid
bool isSystemOver(int val, int countR, int countB, bool start)
{
    int countF = 9 - (countR + countB); //F = empty space
    bool ahead = (!start && ((countR > 0 && countB == 0) || (countR == 0 && countB > 0)));

    if (countF == 3 || val >= 20 || ahead)
        return true;

    return false;
}

//converts the int to char so that it is displayable in the grid
char pieceDisplay (int Rpos, int Cpos, int R[3][3], int B[3][3])
{
    if (R[Rpos][Cpos] == 1)
        return 'R';

    if (B[Rpos][Cpos] == 1)
        return 'B';

    return ' ';
}

//displays grid and players
void gridDisplay(int R[3][3], int B[3][3])
{
    printf("\n");

    //grid omsim
    printf("      0    1    2\n");
    printf("   +----+----+----+\n");
    printf("0  | %c  | %c  | %c  |\n", pieceDisplay(0, 0, R, B), pieceDisplay(0, 1, R, B), pieceDisplay(0, 2, R, B));
    printf("   +----+----+----+\n");
    printf("1  | %c  | %c  | %c  |\n", pieceDisplay(1, 0, R, B), pieceDisplay(1, 1, R, B), pieceDisplay(1, 2, R, B));
    printf("   +----+----+----+\n");
    printf("2  | %c  | %c  | %c  |\n", pieceDisplay(2, 0, R, B), pieceDisplay(2, 1, R, B), pieceDisplay(2, 2, R, B));
    printf("   +----+----+----+\n");
}

int main()
{
    //game flags
    bool good = false, go = true, start = true, over = false, found = false;

    //coords of each variable
    int R[3][3] = {0}, B[3][3] = {0}, S[3][3] = {0}, F[3][3] = {0}, T[3][3] = {0};
    int Rpos, Cpos;

    //number of turns(?)
    int val = 0;

    //position
    int pos;

    //board 0-8
    /*
      0 1 2  F F F
      3 4 5  F F F
      6 7 8  F F F
    */
    char board[3][3];

    printf(" Hi! This was developed by Yours truly: Miguel Hermogenes, Ayumi Ishikawa, and Samuel Parado \n");

    while (!over)
    {
        //display grid
        gridDisplay(R, B);

        //turn count and current player turn
        printf("\n");
        printf("Turn: %d | Player: \n", val);
            if (go == true)
                printf("RED\n");
            else
                printf("BLUE\n");

        printf("Enter Row (0-2) and Column (0-2) (separated by space): ");
        if (scanf("%d %d", &Rpos, &Cpos) != 2)
        {
            printf(" INVALID INPUT! ");
            while (getchar() != '\n'); //clear buffer
        }
        else if (Rpos < 0 || Rpos > 2 || Cpos < 0 || Cpos > 2) // if input is out of bounds
        {
            printf("INVALID INPUT!\n");
        }

        else
        {
        NextPlayerMove(Rpos, Cpos, R, B, S, T, &good, &go, &val, &start, over, &found);

        //Replace(Rpos, Cpos, &go, &found, R[3][3], B[3][3], S[3][3], T[3][3]);

        //Expand(Rpos, Cpos, &found, &go, R[3][3], B[3][3], S[3][3], T[3][3]);

        //Update(Rpos, Cpos, &good, &go, &found, R[3][3], B[3][3], S[3][3], T[3][3]);
        over = isSystemOver(val, countSet(R), countSet(B), start);
        
        //if game is over
            if(over)
            {
                gridDisplay(R, B); // display final grid
                GamerOver(val, R, B); // printf winner or draw
            }
    }
    return 0;
}