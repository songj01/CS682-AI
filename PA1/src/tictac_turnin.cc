#include <tictac_support.h>
#include <stdio.h>
/**
	make_move: takes a board state and makes a legal
	(hopefully optimal) move

	args:
		int [][3] board: 3x3 array of ints representing the
		board state. The values of board are altered based
		on the move
			0: empty
			1: x
		 -1: o

	returns (int):
		the number of steps it took to choose the best move
		(current implementation returns 1 by default, 0 if no move made)
**/
bool game_over(int board[3][3]);
int check_winner(int board[3][3]);
bool tie(int board[3][3]);
void Human_move(int board[3][3], int state);
int minimax(int board[3][3], int depth, int player);
void AI_move(int board[3][3], int depth, int player);







	// figure out what move it is
int make_move(int board[][3]){
	int state = 0;
	for(int i = 0; i< 3;i++)
 		for(int j = 0; j< 3; j++)
			state += board[i][j];

  state = -state;
  if(state == 0) state = 1;

/** If the game is not over:
  * if state == 1, AI will play;
  * if state == -1, human will play.
  */

 if(!game_over(board)){
  	if(state == 1)
  		AI_move(board, 0, state);
  	else if(state == -1)
  		Human_move(board, state);
			game_over(board);
	}

	return 0;
	}




 /// If AI wins or Human Wins, or it is tie, the game will end.
bool game_over(int board[3][3]){
	if(check_winner(board)> 0){
	  printf( " AI Wins, Game Over\n" );
		return true;}
	if(check_winner(board)< 0){
		printf( " Human Wins, Game Over\n" );
		return true;}
	if(tie(board)){
		printf( " It is a tie\n" );
		return true;}
	return false;
	}




 /** According the rule of game, the game will be over in advance if the state is as below,
   * the winner will be AI or Human,
   * if AI wins, AI will gain +10 points; if Human wins, AI will gain -10 points.
   */
int check_winner(int board[3][3]){
	int i, j;
  for(i =0; i <3; i++){
    if(board[i][0]==board[i][1]&&board[i][1]==board[i][2]){
			if(board[i][0] == 1) return +10;
      else if(board[i][0] == -1) return -10;
  		}
		}

	for(j =0; j <3; j++){
    if(board[0][j]==board[1][j]&&board[1][j]==board[2][j]){
			if(board[0][j] == 1) return +10;
      else if(board[0][j] == -1) return -10;
    	}

  	}

  if(board[0][0]==board[1][1]&&board[0][0]==board[2][2]){
		if(board[0][0]==1) return +10;
    else if(board[0][0]==-1) return -10;
		else return 0;
  	}

  if(board[0][2]==board[1][1]&&board[0][2]==board[2][0]){
    if(board[0][2]==1) return +10;
    else if(board[0][2]==-1) return -10;
  	}
	else return 0;

	}



/** Scan the board,if the states are not equal 0,
  * meanwhile, no one win the game,that will be tie.
  */
bool tie(int board[3][3]){
	int count =0;
	if(check_winner(board)==0){
	for(int i =0; i<3; i++)
  	for(int j =0; j< 3; j++)
    	if(board[i][j] == 0 ){
				count++;
			}
       	if(count == 0) return true;
			}
return false;
	}





 /** Assume this function pretend to be human,
   * find any unoccupied square and make the move.
   */
void Human_move(int board[3][3], int state){
	/*for(int i = 0; i< 3; i++){
 	for(int j =0; j< 3; j++)
		if(board[i][j] ==0){
		  printf( "player [%d] made move: [%d,%d]\n", state, i, j );
		  board[i][j] = state;
			break;
   }
	 break;
 }*/
 int i=-1, j=-1;
  printf( "input the move of human\n" );
 scanf("%d", &i);
 scanf("%d", &j);
 printf( "player [%d] made move: [%d,%d]\n", state, i, j );
 board[i][j] = state;

  }


int minimax(int board[3][3], int depth,int player){
	int score;

    ///base case
  	if(check_winner(board)== +10){
			score = 10-depth ;
    	return score;
    	}
 		if(check_winner(board)== -10){
    	score = -10+depth;
    	return score;
   		}
 	 	if(tie(board)){
    	score = 0;
    	return score;
    	}
	else if(player == 1){
		int bestVal = -100000;
		for(int i =0; i< 3;i++)
			for(int j =0; j< 3;j++)
				if(board[i][j] == 0){
				  board[i][j] = player;
					int val = minimax(board, depth+1,-player);
					bestVal = (val > bestVal)? val:bestVal;
					board[i][j] = 0;
				 }
		return bestVal;
	}
  else{
  	int bestVal = +100000;
		for(int i =0; i< 3;i++)
			for(int j =0; j< 3;j++)
				if(board[i][j] == 0){
				  board[i][j] = player;
					int val = minimax(board, depth+1,-player);
					bestVal = (bestVal > val)? val:bestVal;
					board[i][j] = 0;
				 }
		return bestVal;
	  }

  }

void AI_move(int board[3][3], int depth,int player){
	int m =-1,n =-1, bestVal = -100000;
		for(int i =0; i< 3;i++)
			for(int j =0; j< 3;j++)
				if(board[i][j] == 0){
				  board[i][j] = player;
					int val = minimax(board,depth+1, -player);
					if(val > bestVal){bestVal = val;m = i; n = j;}
					   board[i][j] = 0;
					   }
			printf( "player [%d] made move: [%d,%d]\n", player, m, n );
		  board[m][n] = player;

 }
