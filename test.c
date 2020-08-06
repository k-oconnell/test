#include <ncurses.h>
#include <stdlib.h>

//declaring function headers
int clearMat();
void drawLShape(int x, int y, int matrix[][32]);
int clearRows(int matSize, int matrix[][32]);
void shiftRowsDown(int rowDrop, int matSize,int matrix[][32]);

int main(void){

  int matSize = 32;
  int xi = 0;
  int yi = 20;
  int xHold = 0;
  int yHold = 0;
  int i, j, input, rowShift;
  int currentMat[32][32]; //This holds stored blocks
  int newMat[32][32]; //This holds the current block that the user is controlling

  // ******* Initializing the Terminal Window *********
  initscr();
  cbreak();
  noecho();
  curs_set(FALSE);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);

  clear();
  // ***** Clear all matrices before beginning the game
  for (i = 0; i < matSize; i++){
    for (j = 0; j < matSize; j++){
      currentMat[i][j] = 0;
      newMat[i][j] = 0;
      refresh();
    }
  }

  // ***** Place blocks at the last three rows, I did this to test the clear rows functionality
   for (j = 4; j < matSize; j++){
     currentMat[30][j] = 1;
     currentMat[29][j] = 1;
     currentMat[31][j] = 1;
   }

   // ***** Game Begins ******
  while(1){
    rowShift = clearRows(matSize, currentMat); //Check to see if any rows are full
    drawLShape(yi, xi, newMat); //Update the currently moving piece

    // This nested for-loop is what draws values in the matrix (instead of mvaddch, we'll use canvas()->SetPixel)
    for (i = 0; i < matSize; i++){
      for (j = 0; j < matSize; j++){
        if ((currentMat[i][j] == 1) && (newMat[i][j] != 1)){
          mvaddch(i, j, 'O');
          refresh();
        }
        else if ((currentMat[i][j] == 0) && (newMat[i][j] == 1)){
          mvaddch(i, j, 'O');
          refresh();
        }
        else if ((currentMat[i][j] == 1) && (newMat[i][j] == 1)){
          xi = xHold;
          yi = yHold;
        }
        else {
          mvaddch(i, j, '*');
          refresh();
        }
        newMat[i][j] = 0;
        /*else if (currentMat[i][j] == newMat[i][j]){
          currentMat[i][j] = currentMat[i][j];
        }
        if (currentMat[i][j] == 0){
          mvaddch(j, i, '*');
          refresh();
        }
        else if (currentMat[i][j] == 1){
          mvaddch(j, i, 'O');
          refresh();
        }*/
      }
    }


    input = getch();
    xHold = xi; //This saves the current location, I use this for when a block hits another block
    yHold = yi;

    //Block movement controls
    switch(input){
      case KEY_UP:
        yi = yi - 1;
        break;
      case KEY_DOWN:
        yi = yi + 1;
        break;
      case KEY_LEFT:
        xi = xi - 1;
        break;
      case KEY_RIGHT:
        xi = xi + 1;
        break;
      case 32:
        drawLShape(yi, xi, currentMat);
        xi = 0;
        yi = 8;
        break;
    }
    if (xi >= matSize){
      xi = matSize - 1;
    }
    else if (xi < 0){
      xi = 0;
    }
    else if (yi >= matSize){
      yi = matSize - 1;
    }
  }


  endwin();
  exit(0);
}

int clearRows(int matSize, int matrix[][32]){
  int i, j;
  int rowDrop = -1;
  int counter = 0;
  int rowsToClear[matSize]; //The rows that need to be cleared are stored in this array

  for (i = 0; i < matSize; i++){
    rowsToClear[i] = NULL;
  }

  for (i = 0; i < matSize; i++){
    for (j = 0; j < matSize; j++){
      if (matrix[i][j] == 1){
        counter++;
        if (counter == matSize){ // If the counter reaches 32, hence, a row of 1's is detected,
          rowsToClear[i] = 1;    // the row index will be saved into the rowsToClear array
          rowDrop = i;
        }
      }
    }
    counter = 0;
  }

  for (i = matSize-1; i >= 0; i--){ //Goes through rowsToClear array and clears the rows in the actual matrix
      for (j = 0; j < matSize; j++){
        if (rowsToClear[i] != NULL){
          matrix[i][j] = 0;
        }
      }
  }
  return rowDrop; //I thought rowDrop variable would be used for the shiftRowsDown function
}

void drawLShape(int x, int y, int matrix[][32]){
  matrix[x][y] = 1;
  matrix[x-1][y] = 1;
  matrix[x-2][y] = 1;
  matrix[x-3][y] = 1;
  matrix[x-4][y] = 1;
  matrix[x-5][y] = 1;
  matrix[x][y+1] = 1;
  matrix[x-1][y+1] = 1;
  matrix[x-2][y+1] = 1;
  matrix[x-3][y+1] = 1;
  matrix[x-4][y+1] = 1;
  matrix[x-5][y+1] = 1;
  matrix[x][y+2] = 1;
  matrix[x-1][y+2] = 1;
  matrix[x][y+3] = 1;
  matrix[x-1][y+3] = 1;
}

//This fucntion doesn't work properly. It should drop the blocks after a row has been cleared
//However it doesn't do anything to the matrix
void shiftRowsDown(int rowShift, int matSize, int matrix[][32]){
  int i, j;
  for (i = rowShift; i >=0 ; i--){
    for (j = 0; j < matSize; j++){
      if ((matrix[i][j] == 0) && (matrix[i-1][j] == 1)){
        matrix[i][j] = matrix[i-1][j];
      }
    }
  }
}
