/*
A simple chess client featuring a minimax evaluation engine.
*/

#include <iostream>
#include <string>
#include <vector>

//board utilities
void initBoard();
void initTestBoard();
void drawBoard();
void flipBoard();
std::vector<std::vector<int>> board(8, std::vector<int>(8));

bool inCheck();
int value(int i, int j);

void logMoves();
std::vector<int> moveLog;
std::vector<int> bestMoves;
int moveCounter;

//engine utilities
void move(int depth);
int maxEvaluation(int depth); //minimax evaluation
int MAX_DEPTH = 3; //preset "thinking" depth of 3 turns

int main()
{
	std::cout << "Please select your game type:" << std::endl
		<< "1) Human vs AI" << std::endl
		<< "2) AI vs AI" << std::endl;

	std::string inputString;
	std::cin >> inputString;

	if (inputString == "1")
	{
		std::cout << "Please select your difficulty:" << std::endl
			<< "1) Chess Titans (Microsoft)" << std::endl
			<< "2) Deep Blue (IBM)" << std::endl
			<< "3) AlphaZero (DeepMind)" << std::endl
			<< "4+) HAL 9000 (expect large computation times)" << std::endl;

		std::cin >> inputString;
		MAX_DEPTH = stoi(inputString);

		std::cout << "Enter your move in the form \"d2d4\":" << std::endl;
		initBoard();
		//initTestBoard();
		drawBoard();

		while (inputString != "quit")
		{
			std::cin >> inputString;
			
			int iFrom = inputString[0] - '0' - 49;
			int jFrom = inputString[1] - '0' - 1;
			int iTo = inputString[2] - '0' - 49;
			int jTo = inputString[3] - '0' - 1;

			//checks that the entry is in the required form
			if (iFrom >= 0 && iFrom <= 7 && jFrom >= 0 && jFrom <= 7 && iTo >= 0 && iTo <= 7 && jTo >= 0 && jTo <= 7)
			{
				//in the case of promotion, allow the player to choose which piece they promote to
				if (board[iFrom][jFrom] == 1 && jTo == 7)
				{
					std::cout << "Promote to the following:" << std::endl
						<< "1) Queen" << std::endl
						<< "2) Knight" << std::endl
						<< "3) Bishop" << std::endl
						<< "4) Rook" << std::endl;

					std::cin >> inputString;

					switch (stoi(inputString))
					{
						case 1: board[iTo][jTo] = 9;
							break;
						case 2: board[iTo][jTo] = 5;
							break;
						case 3: board[iTo][jTo] = 7;
							break;
						case 4: board[iTo][jTo] = 3;
							break;
					}

					board[iFrom][jFrom] = 0;
					moveCounter++;
				}
				else
				{
					//make the human move
					board[iTo][jTo] = board[iFrom][jFrom];
					board[iFrom][jFrom] = 0;
					moveCounter++;
				}

				//make the AI move
				flipBoard();
				move(MAX_DEPTH);
				flipBoard();

				drawBoard();
			}
			else if (inputString == "reset")
			{
				//clear and reinitialise the board
				for(int i = 0; i <= 7; i++)
					for (int j = 0; j <= 7; j++)
					{
						board[i][j] = 0;
					}

				moveCounter = 0;

				initBoard();
				//initTestBoard();
				drawBoard();
			}
			else if(inputString != "quit")
			{
				std::cout << "Invalid move." << std::endl;
			}
		}
	}
	else if (inputString == "2")
	{
		std::cout << "Please select your difficulty:" << std::endl
			<< "1) Chess Titans (Microsoft)" << std::endl
			<< "2) Deep Blue (IBM)" << std::endl
			<< "3) AlphaZero (DeepMind)" << std::endl
			<< "4+) HAL 9000 (expect large computation times)" << std::endl;

		std::cin >> inputString;
		MAX_DEPTH = stoi(inputString);

		std::cout << "Type any message to progress the game." << std::endl;
		initBoard();
		//initTestBoard();
		drawBoard();

		while (inputString != "quit")
		{
			std::cin >> inputString;

			if (inputString == "reset")
			{
				for (int i = 0; i <= 7; i++)
					for (int j = 0; j <= 7; j++)
					{
						board[i][j] = 0;
					}

				moveCounter = 0;

				initBoard();
				//initTestBoard();
				drawBoard();
			}
			else if (inputString != "quit")
			{
				if (moveCounter % 2 == 0)
				{
					move(MAX_DEPTH);
				}
				else
				{
					flipBoard();
					move(MAX_DEPTH);
					flipBoard();
				}

				drawBoard();
			}
		}
	}

	return 0;
}

void initBoard()
{
	for (int i = 0; i <= 7; i++)
	{
		//initialise pawns
		board[i][1] = 1;
		board[i][6] = 2;
	}

	//initialise rooks
	board[0][0] = 3;
	board[7][0] = 3;
	board[0][7] = 4;
	board[7][7] = 4;

	//initialise knights
	board[1][0] = 5;
	board[6][0] = 5;
	board[1][7] = 6;
	board[6][7] = 6;

	//initialise bishops
	board[2][0] = 7;
	board[5][0] = 7;
	board[2][7] = 8;
	board[5][7] = 8;

	//initialise queens
	board[3][0] = 9;
	board[3][7] = 10;

	//initialise kings
	board[4][0] = 11;
	board[4][7] = 12;
}

void initTestBoard()
{
	//move evaluation test
	board[5][7] = 8;
	board[4][6] = 6;
	board[2][6] = 2;
	board[3][5] = 7;
	board[4][0] = 3;

	/*
	//promotion test
	board[2][6] = 1;
	board[3][5] = 1;
	board[4][6] = 6;
	board[7][6] = 4;
	*/
}

void drawBoard()
{
	for (int j = 7; j >= 0; j--)
	{
		for (int i = 0; i <= 7; i++)
		{
			switch (board[i][j])
			{
				case 0: std::cout << "+";
					break;
				case 1: std::cout << "p";
					break;
				case 2: std::cout << "P";
					break;
				case 3: std::cout << "r";
					break;
				case 4: std::cout << "R";
					break;
				case 5: std::cout << "n";
					break;
				case 6: std::cout << "N";
					break;
				case 7: std::cout << "b";
					break;
				case 8: std::cout << "B";
					break;
				case 9: std::cout << "q";
					break;
				case 10: std::cout << "Q";
					break;
				case 11: std::cout << "k";
					break;
				case 12: std::cout << "K";
					break;
			}

			std::cout << " ";
		}

		std::cout << std::endl;
	}
}

void flipBoard() //allows us to take advantage of black/white symmetry
{
	for (int i = 0; i <= 7; i++)
		for (int j = 0; j <= 7; j++)
		{
			switch (board[i][j])
			{
			case 1: board[i][j] = 2;
				break;
			case 2: board[i][j] = 1;
				break;
			case 3: board[i][j] = 4;
				break;
			case 4: board[i][j] = 3;
				break;
			case 5: board[i][j] = 6;
				break;
			case 6: board[i][j] = 5;
				break;
			case 7: board[i][j] = 8;
				break;
			case 8: board[i][j] = 7;
				break;
			case 9: board[i][j] = 10;
				break;
			case 10: board[i][j] = 9;
				break;
			case 11: board[i][j] = 12;
				break;
			case 12: board[i][j] = 11;
				break;
			}
		}
}

int value(int i, int j)
{
	switch (board[i][j])
	{
		case 0: return 0;
			break;
		case 1: return 1;
			break;
		case 2: return 1;
			break;
		case 3: return 5;
			break;
		case 4: return 5;
			break;
		case 5: return 3;
			break;
		case 6: return 3;
			break;
		case 7: return 3;
			break;
		case 8: return 3;
			break;
		case 9: return 9;
			break;
		case 10: return 9;
			break;
		case 11: return 21;
			break;
		case 12: return 21;
			break;
	}
}

void move(int depth)
{
	//build the minimax evaluation tree
	int evaluate = maxEvaluation(depth);

	if (bestMoves.size() != 0)
	{
		//choose one of the best available moves at random
		int k = 4 * (rand() % (bestMoves.size() / 4));

		//filter for promotion cases
		if (board[bestMoves[k]][bestMoves[k + 1]] == 1 && (bestMoves[k + 3] == 0 || bestMoves[k + 3] == 7))
		{
			//automatically promote to a queen
			board[bestMoves[k + 2]][bestMoves[k + 3]] = 9;
			board[bestMoves[k]][bestMoves[k + 1]] = 0;
			moveCounter++;
		}
		else
		{
			//apply the assigned mvoe and increment the move counter
			board[bestMoves[k + 2]][bestMoves[k + 3]] = board[bestMoves[k]][bestMoves[k + 1]];
			board[bestMoves[k]][bestMoves[k + 1]] = 0;
			moveCounter++;
		}

		bestMoves.clear();
	}
	else
	{
		//if no good moves are found, the game is over
		if (moveCounter % 2 == 0)
		{
			std::cout << "White has no good moves!" << std::endl;
		}
		else
		{
			std::cout << "Black has no good moves!" << std::endl;
		}
	}
}

int maxEvaluation(int depth)
{
	int maxEval = -60;

	if (depth == MAX_DEPTH)
	{
		//log and record all legal moves
		logMoves();
		std::vector<int> rollingLog = moveLog;

		//for each legal move, test all possible outcomes and associate with each an evaluation
		for (int i = 0; i < rollingLog.size(); i += 4)
		{
			//record the piece values at the positions that we intend to change
			int fromPiece = board[rollingLog[i]][rollingLog[i + 1]];
			int toPiece = board[rollingLog[i + 2]][rollingLog[i + 3]];

			int tempEval = value(rollingLog[i + 2], rollingLog[i + 3]);

			//alter the board
			board[rollingLog[i]][rollingLog[i + 1]] = 0;
			board[rollingLog[i + 2]][rollingLog[i + 3]] = fromPiece;

			//in the case of promotion we increase the board evaluation by 9 - 1 = 8;
			if (fromPiece == 1 && (rollingLog[i + 3] == 7 || rollingLog[i + 3] == 0))
			{
				tempEval += 8;
				board[rollingLog[i + 2]][rollingLog[i + 3]] = 9;
			}

			moveCounter++;
			flipBoard();

			//define the move evaluation recursively
			int eval = tempEval - maxEvaluation(depth - 1);

			//reset the board
			flipBoard();
			moveCounter--;
			board[rollingLog[i]][rollingLog[i + 1]] = fromPiece;
			board[rollingLog[i + 2]][rollingLog[i + 3]] = toPiece;

			//record those moves that give the optimum evaluation
			if (eval > maxEval)
			{
				maxEval = eval;

				bestMoves.clear();
				for (int j = i; j <= i + 3; j++)
				{
					bestMoves.push_back(rollingLog[j]);
				}
			}
			else if (eval == maxEval)
			{
				for (int j = i; j <= i + 3; j++)
				{
					bestMoves.push_back(rollingLog[j]);
				}
			}
		}

		//return the evaluation at the base of our move space
		return maxEval;
	}
	else if (depth > 0) //recursively assign evaluations to each branch of the move space
	{
		logMoves();
		std::vector<int> rollingLog = moveLog;

		for (int i = 0; i < rollingLog.size(); i += 4)
		{
			int fromPiece = board[rollingLog[i]][rollingLog[i + 1]];
			int toPiece = board[rollingLog[i + 2]][rollingLog[i + 3]];

			int tempEval = value(rollingLog[i + 2], rollingLog[i + 3]);

			board[rollingLog[i]][rollingLog[i + 1]] = 0;
			board[rollingLog[i + 2]][rollingLog[i + 3]] = fromPiece;

			if (fromPiece == 1 && (rollingLog[i + 3] == 7 || rollingLog[i + 3] == 0))
			{
				tempEval += 8;
				board[rollingLog[i + 2]][rollingLog[i + 3]] = 9;
			}

			moveCounter++;
			flipBoard();

			int eval = tempEval - maxEvaluation(depth - 1);

			flipBoard();
			moveCounter--;
			board[rollingLog[i]][rollingLog[i + 1]] = fromPiece;
			board[rollingLog[i + 2]][rollingLog[i + 3]] = toPiece;

			if (eval > maxEval)
			{
				maxEval = eval;
			}
		}

		return maxEval;
	}
}

bool inCheck()
{
	for (int i = 0; i <= 7; i++)
		for (int j = 0; j <= 7; j++)
		{
			if (board[i][j] == 11)
			{
				//check pawn attacks
				if (moveCounter % 2 == 0)
				{
					if (j + 1 <= 7)
					{
						if (i - 1 >= 0)
						{
							if (board[i - 1][j + 1] == 2)
							{
								return true;
							}
						}
						if (i + 1 <= 7)
						{
							if (board[i + 1][j + 1] == 2)
							{
								return true;
							}
						}
					}
				}
				if (moveCounter % 2 == 1)
				{
					if (j - 1 >= 0)
					{
						if (i - 1 >= 0)
						{
							if (board[i - 1][j - 1] == 2)
							{
								return true;
							}
						}
						if (i + 1 <= 7)
						{
							if (board[i + 1][j - 1] == 2)
							{
								return true;
							}
						}
					}
				}

				//check knights attacks
				if (i + 2 <= 7)
				{
					if (j + 1 <= 7)
					{
						if (board[i + 2][j + 1] == 6)
						{
							return true;
						}
					}
					if (j - 1 >= 0)
					{
						if (board[i + 2][j - 1] == 6)
						{
							return true;
						}
					}
				}
				if (i - 2 >= 0)
				{
					if (j + 1 <= 7)
					{
						if (board[i - 2][j + 1] == 6)
						{
							return true;
						}
					}
					if (j - 1 >= 0)
					{
						if (board[i - 2][j - 1] == 6)
						{
							return true;
						}
					}
				}
				if (i + 1 <= 7)
				{
					if (j + 2 <= 7)
					{
						if (board[i + 1][j + 2] == 6)
						{
							return true;
						}
					}
					if (j - 2 >= 0)
					{
						if (board[i + 1][j - 2] == 6)
						{
							return true;
						}
					}
				}
				if (i - 1 >= 0)
				{
					if (j + 2 <= 7)
					{
						if (board[i - 1][j + 2] == 6)
						{
							return true;
						}
					}
					if (j - 2 >= 0)
					{
						if (board[i - 1][j - 2] == 6)
						{
							return true;
						}
					}
				}

				//check rook/lateral queen attacks
				for (int k = i + 1; k <= 7; k++)
				{
					if (board[k][j] % 2 == 1)
					{
						break;
					}
					else if (board[k][j] == 4 || board[k][j] == 10)
					{
						return true;
					}
				}
				for (int k = i - 1; k >= 0; k--)
				{
					if (board[k][j] % 2 == 1)
					{
						break;
					}
					else if (board[k][j] == 4 || board[k][j] == 10)
					{
						return true;
					}
				}
				for (int k = j + 1; k <= 7; k++)
				{
					if (board[i][k] % 2 == 1)
					{
						break;
					}
					else if (board[i][k] == 4 || board[i][k] == 10)
					{
						return true;
					}
				}
				for (int k = j - 1; k >= 0; k--)
				{
					if (board[i][k] % 2 == 1)
					{
						break;
					}
					else if (board[i][k] == 4 || board[i][k] == 10)
					{
						return true;
					}
				}

				//check bishop/diagonal queen attacks
				for (int k = 1; k <= 7; k++)
				{
					if (i + k <= 7)
					{
						if (j + k <= 7)
						{
							if (board[i + k][j + k] % 2 == 1)
							{
								break;
							}
							else if (board[i + k][j + k] == 8 || board[i + k][j + k] == 10)
							{
								return true;
							}
						}
					}
				}
				for (int k = 1; k <= 7; k++)
				{
					if (i + k <= 7)
					{
						if (j - k >= 0)
						{
							if (board[i + k][j - k] % 2 == 1)
							{
								break;
							}
							else if (board[i + k][j - k] == 8 || board[i + k][j - k] == 10)
							{
								return true;
							}
						}
					}
				}
				for (int k = 1; k <= 7; k++)
				{
					if (i - k >= 0)
					{
						if (j + k <= 7)
						{
							if (board[i - k][j + k] % 2 == 1)
							{
								break;
							}
							else if (board[i - k][j + k] == 8 || board[i - k][j + k] == 10)
							{
								return true;
							}
						}
					}
				}
				for (int k = 1; k <= 7; k++)
				{
					if (i - k >= 0)
					{
						if (j - k >= 0)
						{
							if (board[i - k][j - k] % 2 == 1)
							{
								break;
							}
							else if (board[i - k][j - k] == 8 || board[i - k][j - k] == 10)
							{
								return true;
							}
						}
					}
				}

				//check king moves
				{
					if (i + 1 <= 7)
					{
						if (board[i + 1][j] == 12)
						{
							return true;
						}

						if (j + 1 <= 7)
						{
							if (board[i + 1][j + 1] == 12)
							{
								return true;
							}
						}
						if (j - 1 >= 0)
						{
							if (board[i + 1][j - 1] == 12)
							{
								return true;
							}
						}
					}
					if (i - 1 >= 0)
					{
						if (board[i - 1][j] == 12)
						{
							return true;
						}

						if (j + 1 <= 7)
						{
							if (board[i - 1][j + 1] == 12)
							{
								return true;
							}
						}
						if (j - 1 >= 0)
						{
							if (board[i - 1][j - 1] == 12)
							{
								return true;
							}
						}
					}
					if (j + 1 <= 7)
					{
						if (board[i][j + 1] == 12)
						{
							return true;
						}
					}
					if (j - 1 >= 0)
					{
						if (board[i][j - 1] == 12)
						{
							return true;
						}
					}
				}
			}
		}

	return false;
}

void logMoves()
{
	std::vector<int> log;

	for (int i = 0; i <= 7; i++)
		for (int j = 0; j <= 7; j++)
		{
			//log all possible moves for white pawns
			if (board[i][j] == 1 && moveCounter % 2 == 0)
			{
				if (j + 1 <= 7)
				{
					if (board[i][j + 1] == 0)
					{
						if (j == 1 && board[i][j + 2] == 0)
						{
							log.push_back(i);
							log.push_back(j);
							log.push_back(i);
							log.push_back(j + 2);
						}

						log.push_back(i);
						log.push_back(j);
						log.push_back(i);
						log.push_back(j + 1);
					}
					if (i - 1 >= 0)
					{
						if (board[i - 1][j + 1] != 0 && board[i - 1][j + 1] % 2 == 0)
						{
							log.push_back(i);
							log.push_back(j);
							log.push_back(i - 1);
							log.push_back(j + 1);
						}
					}
					if (i + 1 <= 7)
					{
						if (board[i + 1][j + 1] != 0 && board[i + 1][j + 1] % 2 == 0)
						{
							log.push_back(i);
							log.push_back(j);
							log.push_back(i + 1);
							log.push_back(j + 1);
						}
					}
				}
			}

			//log all possible moves for black pawns
			if (board[i][j] == 1 && moveCounter % 2 == 1)
			{
				if (j - 1 >= 0)
				{
					if (board[i][j - 1] == 0)
					{
						if (j == 6 && board[i][j - 2] == 0)
						{
							log.push_back(i);
							log.push_back(j);
							log.push_back(i);
							log.push_back(j -2);
						}

						log.push_back(i);
						log.push_back(j);
						log.push_back(i);
						log.push_back(j - 1);
					}
					if (i - 1 >= 0)
					{
						if (board[i - 1][j - 1] != 0 && board[i - 1][j - 1] % 2 == 0)
						{
							log.push_back(i);
							log.push_back(j);
							log.push_back(i - 1);
							log.push_back(j - 1);
						}
					}
					if (i + 1 <= 7)
					{
						if (board[i + 1][j - 1] != 0 && board[i + 1][j - 1] % 2 == 0)
						{
							log.push_back(i);
							log.push_back(j);
							log.push_back(i + 1);
							log.push_back(j - 1);
						}
					}
				}
			}

			//log all possible rook/queen moves
			if (board[i][j] == 3 || board[i][j] == 9)
			{
				for (int k = i + 1; k <= 7; k++)
				{
					if (board[k][j] % 2 == 1)
					{
						break;
					}

					if (board[k][j] == 0)
					{
						log.push_back(i);
						log.push_back(j);
						log.push_back(k);
						log.push_back(j);
					}
					else if (board[k][j] % 2 == 0)
					{
						log.push_back(i);
						log.push_back(j);
						log.push_back(k);
						log.push_back(j);
						break;
					}
				}
				for (int k = i - 1; k >= 0; k--)
				{
					if (board[k][j] % 2 == 1)
					{
						break;
					}

					if (board[k][j] == 0)
					{
						log.push_back(i);
						log.push_back(j);
						log.push_back(k);
						log.push_back(j);
					}
					else if (board[k][j] % 2 == 0)
					{
						log.push_back(i);
						log.push_back(j);
						log.push_back(k);
						log.push_back(j);
						break;
					}
				}
				for (int k = j + 1; k <= 7; k++)
				{
					if (board[i][k] % 2 == 1)
					{
						break;
					}

					if (board[i][k] == 0)
					{
						log.push_back(i);
						log.push_back(j);
						log.push_back(i);
						log.push_back(k);
					}
					else if (board[i][k] % 2 == 0)
					{
						log.push_back(i);
						log.push_back(j);
						log.push_back(i);
						log.push_back(k);
						break;
					}
				}
				for (int k = j - 1; k >= 0; k--)
				{
					if (board[i][k] % 2 == 1)
					{
						break;
					}

					if (board[i][k] == 0)
					{
						log.push_back(i);
						log.push_back(j);
						log.push_back(i);
						log.push_back(k);
					}
					else if (board[i][k] % 2 == 0)
					{
						log.push_back(i);
						log.push_back(j);
						log.push_back(i);
						log.push_back(k);
						break;
					}
				}
			}

			//log all possible knight moves
			if (board[i][j] == 5)
			{
				if (i + 2 <= 7)
				{
					if (j + 1 <= 7)
					{
						if (board[i + 2][j + 1] % 2 == 0)
						{
							log.push_back(i);
							log.push_back(j);
							log.push_back(i + 2);
							log.push_back(j + 1);
						}
					}
					if (j - 1 >= 0)
					{
						if (board[i + 2][j - 1] % 2 == 0)
						{
							log.push_back(i);
							log.push_back(j);
							log.push_back(i + 2);
							log.push_back(j - 1);
						}
					}
				}
				if (i - 2 >= 0)
				{
					if (j + 1 <= 7)
					{
						if (board[i - 2][j + 1] % 2 == 0)
						{
							log.push_back(i);
							log.push_back(j);
							log.push_back(i - 2);
							log.push_back(j + 1);
						}
					}
					if (j - 1 >= 0)
					{
						if (board[i - 2][j - 1] % 2 == 0)
						{
							log.push_back(i);
							log.push_back(j);
							log.push_back(i - 2);
							log.push_back(j - 1);
						}
					}
				}
				if (j + 2 <= 7)
				{
					if (i + 1 <= 7)
					{
						if (board[i + 1][j + 2] % 2 == 0)
						{
							log.push_back(i);
							log.push_back(j);
							log.push_back(i + 1);
							log.push_back(j + 2);
						}
					}
					if (i - 1 >= 0)
					{
						if (board[i - 1][j + 2] % 2 == 0)
						{
							log.push_back(i);
							log.push_back(j);
							log.push_back(i - 1);
							log.push_back(j + 2);
						}
					}
				}
				if (j - 2 >= 0)
				{
					if (i + 1 <= 7)
					{
						if (board[i + 1][j - 2] % 2 == 0)
						{
							log.push_back(i);
							log.push_back(j);
							log.push_back(i + 1);
							log.push_back(j - 2);
						}
					}
					if (i - 1 >= 0)
					{
						if (board[i - 1][j - 2] % 2 == 0)
						{
							log.push_back(i);
							log.push_back(j);
							log.push_back(i - 1);
							log.push_back(j - 2);
						}
					}
				}
			}

			//log all possible bishop moves
			if (board[i][j] == 7 || board[i][j] == 9)
			{
				for (int k = 1; k <= 7; k++)
				{
					if (i + k <= 7)
					{
						if (j + k <= 7)
						{
							if (board[i + k][j + k] % 2 == 1)
							{
								break;
							}

							if (board[i + k][j + k] == 0)
							{
								log.push_back(i);
								log.push_back(j);
								log.push_back(i + k);
								log.push_back(j + k);
							}
							else if (board[i + k][j + k] % 2 == 0)
							{
								log.push_back(i);
								log.push_back(j);
								log.push_back(i + k);
								log.push_back(j + k);
								break;
							}
						}
					}
				}
				for (int k = 1; k <= 7; k++)
				{
					if (i + k <= 7)
					{
						if (j - k >= 0)
						{
							if (board[i + k][j - k] % 2 == 1)
							{
								break;
							}

							if (board[i + k][j - k] == 0)
							{
								log.push_back(i);
								log.push_back(j);
								log.push_back(i + k);
								log.push_back(j - k);
							}
							else if (board[i + k][j - k] % 2 == 0)
							{
								log.push_back(i);
								log.push_back(j);
								log.push_back(i + k);
								log.push_back(j - k);
								break;
							}
						}
					}
				}
				for (int k = 1; k <= 7; k++)
				{
					if (i - k >= 0)
					{
						if (j + k <= 7)
						{
							if (board[i - k][j + k] % 2 == 1)
							{
								break;
							}

							if (board[i - k][j + k] == 0)
							{
								log.push_back(i);
								log.push_back(j);
								log.push_back(i - k);
								log.push_back(j + k);
							}
							else if (board[i - k][j + k] % 2 == 0)
							{
								log.push_back(i);
								log.push_back(j);
								log.push_back(i - k);
								log.push_back(j + k);
								break;
							}
						}
					}
				}
				for (int k = 1; k <= 7; k++)
				{
					if (i - k >= 0)
					{
						if (j - k >= 0)
						{
							if (board[i - k][j - k] % 2 == 1)
							{
								break;
							}

							if (board[i - k][j - k] == 0)
							{
								log.push_back(i);
								log.push_back(j);
								log.push_back(i - k);
								log.push_back(j - k);
							}
							else if (board[i - k][j - k] % 2 == 0)
							{
								log.push_back(i);
								log.push_back(j);
								log.push_back(i - k);
								log.push_back(j - k);
								break;
							}
						}
					}
				}
			}
			
			//log all possible king moves
			if (board[i][j] == 11)
			{
				if (i + 1 <= 7)
				{
					if (board[i + 1][j] % 2 == 0)
					{
						log.push_back(i);
						log.push_back(j);
						log.push_back(i + 1);
						log.push_back(j);
					}
					if (j + 1 <= 7)
					{
						if (board[i + 1][j + 1] % 2 == 0)
						{
							log.push_back(i);
							log.push_back(j);
							log.push_back(i + 1);
							log.push_back(j + 1);
						}
					}
					if (j - 1 >= 0)
					{
						if (board[i + 1][j - 1] % 2 == 0)
						{
							log.push_back(i);
							log.push_back(j);
							log.push_back(i + 1);
							log.push_back(j - 1);
						}
					}
				}
				if (i - 1 >= 0)
				{
					if (board[i - 1][j] % 2 == 0)
					{
						log.push_back(i);
						log.push_back(j);
						log.push_back(i - 1);
						log.push_back(j);
					}
					if (j + 1 <= 7)
					{
						if (board[i - 1][j + 1] % 2 == 0)
						{
							log.push_back(i);
							log.push_back(j);
							log.push_back(i - 1);
							log.push_back(j + 1);
						}
					}
					if (j - 1 >= 0)
					{
						if (board[i - 1][j - 1] % 2 == 0)
						{
							log.push_back(i);
							log.push_back(j);
							log.push_back(i - 1);
							log.push_back(j - 1);
						}
					}
				}
				if (j + 1 <= 7)
				{
					if (board[i][j + 1] % 2 == 0)
					{
						log.push_back(i);
						log.push_back(j);
						log.push_back(i);
						log.push_back(j + 1);
					}
				}
				if (j - 1 >= 0)
				{
					if (board[i][j - 1] % 2 == 0)
					{
						log.push_back(i);
						log.push_back(j);
						log.push_back(i);
						log.push_back(j - 1);
					}
				}
			}
		}

	//filter out moves that keep us in check
	if (inCheck())
	{
		//log all that moves that escape us from checks
		std::vector<int> escapeLog;
		for (int i = 0; i < log.size(); i += 4)
		{
			int fromValue = board[log[i]][log[i + 1]];
			int toValue = board[log[i + 2]][log[i + 3]];

			//make the move
			board[log[i + 2]][log[i + 3]] = board[log[i]][log[i + 1]];
			board[log[i]][log[i + 1]] = 0;

			//if we escape the check, record the move
			if (!inCheck())
			{
				for (int j = i; j <= i + 3; j++)
				{
					escapeLog.push_back(log[j]);
				}
			}

			//undo the move
			board[log[i]][log[i + 1]] = fromValue;
			board[log[i + 2]][log[i + 3]] = toValue;
		}

		log = escapeLog;
	}

	moveLog = log;
	log.clear();
}
