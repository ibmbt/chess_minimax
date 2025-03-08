#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int SIZE = 8;
char **BOARD;
int turn = 0;
const int MAX_STATES = 200;
char board_history[MAX_STATES][SIZE][SIZE];
int current_state = 0;
int total_states = 0;

void save_board_state();
void undo();
void redo();
void initialize(const string &filename);
void setboard(const string &filename);
void saveboard(const string &filename, bool lastmove);
void display();
bool valid_source(int x, int y);
bool is_check(int turn);
bool is_stalemate();
bool is_checkmate();
bool valid_destination(int s_x, int s_y, int d_x, int d_y);
bool is_valid_black_pawn_move(int s_x, int s_y, int d_x, int d_y);
bool is_valid_white_pawn_move(int s_x, int s_y, int d_x, int d_y);
bool is_valid_rook_move(int s_x, int s_y, int d_x, int d_y);
bool is_valid_bishop_move(int s_x, int s_y, int d_x, int d_y);
bool is_valid_knight_move(int s_x, int s_y, int d_x, int d_y);
bool is_valid_king_move(int s_x, int s_y, int d_x, int d_y);
bool is_valid_queen_move(int s_x, int s_y, int d_x, int d_y);
bool is_castling_valid(int s_x, int s_y, int d_x, int d_y);
void do_castling(int s_x, int s_y, int d_x, int d_y);
bool is_enpassant_valid(int s_x, int s_y, int d_x, int d_y);
void do_enpassant(int s_x, int s_y, int d_x, int d_y);
void pawn_promotion(int d_x, int d_y);
int evaluate_board();
int minimax(int depth, bool is_maximizer);
void ai_move();
bool move_valid(int s_x, int s_y, int d_x, int d_y);
bool usermove_valid(int s_x, int s_y, int d_x, int d_y);
bool aimove_valid(int s_x, int s_y, int d_x, int d_y);
void make_move(int s_x, int s_y, int d_x, int d_y);
void change_turn();
void clearing_memory();
void clear_screen();
bool gameover();
void replay(char &choice);

int main()
{

	char repeat = 'Y';
	char choice;
	char resume;
	while (repeat == 'Y' || repeat == 'y')
	{
		cout << "\n===|CHESS|===\n"
			 << endl;
		cout << "\n[1] VS AI" << endl;
		cout << "[2] VS PLAYER 2\n"
			 << endl;
		cin >> choice;

		if (choice == '1')
		{
			char resume, undochoice, redochoice, continuegame;
			int s_x, s_y,
				d_x, d_y;

			cout << "[N] NEW GAME" << endl;
			cout << "[C] CONTINUE\n"
				 << endl;
			do
			{
				cin >> resume;
				if (resume == 'C')
				{
					initialize("currentmove.txt");
				}
				else if (resume == 'N')
				{
					initialize("chessboard.txt");
				}
			} while (resume != 'C' && resume != 'N');

			display();

			while (true)
			{

				if (turn == 0)
				{

					if (is_stalemate())
					{
						cout << "\ndraw stalemate!" << endl;
						break;
					}

					do
					{
						cout << "\nenter row, col of piece:\n";
						cin >> s_x >> s_y;
						if (!valid_source(s_x, s_y))
						{
							cout << "INVALID SOURCE!!" << endl;
						}
					} while (!valid_source(s_x, s_y));

					do
					{
						cout << "\nenter row, col of destination:\n";
						cin >> d_x >> d_y;
						if (!valid_destination(s_x, s_y, d_x, d_y))
						{
							cout << "INVALID DESTINATION!!" << endl;
						}
					} while (!valid_destination(s_x, s_y, d_x, d_y));

					if (move_valid(s_x, s_y, d_x, d_y))
					{

						make_move(s_x, s_y, d_x, d_y);

						save_board_state();
						change_turn();
						if (is_checkmate())
						{
							cout << "Player 1 wins by checkmate" << endl;
							display();
							break;
						}
						change_turn();

						do
						{

							cout << "\ndo you want to undo a move?\n";
							cin >> undochoice;
							if (undochoice == 'Y' || undochoice == 'y')
							{
								undo();
								clear_screen();
								display();
								change_turn();
							}

							cout << "\ndo you want to redo a move?\n";
							cin >> redochoice;
							if (redochoice == 'Y' || redochoice == 'y')
							{
								redo();
								clear_screen();
								display();
								change_turn();
							}

							cout << "\nundo/redo again?\n";
							cin >> continuegame;

						} while (continuegame != 'N' && continuegame != 'n');
					}
					else
					{
						change_turn();
					}
				}

				else if (turn == 1)
				{

					if (is_stalemate())
					{
						cout << "\ndraw stalemate!" << endl;
						break;
					}

					ai_move();

					change_turn();
					if (is_checkmate())
					{
						cout << "AI wins by checkmate" << endl;
						display();
						break;
					}
					change_turn();
				}

				if (turn == 0)
				{
					clear_screen();
				}
				change_turn();
				display();
			}
		}

		else if (choice == '2')
		{
			char resume, undochoice, redochoice, continuegame;
			int s_x, s_y,
				d_x, d_y;

			cout << "[N] NEW GAME" << endl;
			cout << "[C] CONTINUE\n"
				 << endl;
			do
			{
				cin >> resume;
				if (resume == 'C')
				{
					initialize("currentmove.txt");
				}
				else if (resume == 'N')
				{
					initialize("chessboard.txt");
				}
			} while (resume != 'C' && resume != 'N');

			display();

			while (true)
			{

				if (turn == 0)
				{

					if (is_stalemate())
					{
						cout << "\ndraw stalemate!" << endl;
						break;
					}

					cout << "player 1 thinking...\n";
					do
					{
						cin >> s_x >> s_y;
						if (!valid_source(s_x, s_y))
						{
							cout << "INVALID SOURCE!!" << endl;
						}
					} while (!valid_source(s_x, s_y));

					do
					{
						cin >> d_x >> d_y;
						if (!valid_destination(s_x, s_y, d_x, d_y))
						{
							cout << "INVALID DESTINATION!!" << endl;
						}
					} while (!valid_destination(s_x, s_y, d_x, d_y));

					if (move_valid(s_x, s_y, d_x, d_y))
					{

						make_move(s_x, s_y, d_x, d_y);

						save_board_state();
						change_turn();
						if (is_checkmate())
						{
							cout << "Player 1 wins by checkmate" << endl;
							display();
							break;
						}
						change_turn();

						do
						{

							cout << "\ndo you want to undo a move?\n";
							cin >> undochoice;
							if (undochoice == 'Y' || undochoice == 'y')
							{
								undo();
								clear_screen();
								display();
								change_turn();
							}

							cout << "\ndo you want to redo a move?\n";
							cin >> redochoice;
							if (redochoice == 'Y' || redochoice == 'y')
							{
								redo();
								clear_screen();
								display();
								change_turn();
							}

							cout << "\nundo/redo again?\n";
							cin >> continuegame;

						} while (continuegame != 'N' && continuegame != 'n');
					}
					else
					{
						change_turn();
					}
				}

				else if (turn == 1)
				{

					if (is_stalemate())
					{
						cout << "\ndraw stalemate!" << endl;
						break;
					}

					cout << "player 2 thinking...\n";

					do
					{
						cin >> s_x >> s_y;
						if (!valid_source(s_x, s_y))
						{
							cout << "INVALID SOURCE!!" << endl;
						}
					} while (!valid_source(s_x, s_y));

					do
					{
						cin >> d_x >> d_y;
						if (!valid_destination(s_x, s_y, d_x, d_y))
						{
							cout << "INVALID DESTINATION!!" << endl;
						}
					} while (!valid_destination(s_x, s_y, d_x, d_y));

					if (move_valid(s_x, s_y, d_x, d_y))
					{

						make_move(s_x, s_y, d_x, d_y);

						save_board_state();
						change_turn();
						if (is_checkmate())
						{
							cout << "Player 2 wins by checkmate" << endl;
							display();
							break;
						}
						change_turn();

						do
						{

							cout << "\ndo you want to undo a move?\n";
							cin >> undochoice;
							if (undochoice == 'Y' || undochoice == 'y')
							{
								undo();
								clear_screen();
								display();
								change_turn();
							}

							cout << "\ndo you want to redo a move?\n";
							cin >> redochoice;
							if (redochoice == 'Y' || redochoice == 'y')
							{
								redo();
								clear_screen();
								display();
								change_turn();
							}

							cout << "\nundo/redo again?\n";
							cin >> continuegame;

						} while (continuegame != 'N' && continuegame != 'n');
					}
					else
					{
						change_turn();
					}
				}

				clear_screen();
				change_turn();
				display();
			}
		}

		else
		{
			cout << "INVALID ENTRY" << endl;
		}

		replay(repeat);
		if ((repeat != 'y') && (repeat != 'Y'))
		{
			cout << "THANK YOU FOR PLAYING.." << endl;
			clearing_memory();
		}
	}

	return 0;
}

void save_board_state()
{
	if (current_state < MAX_STATES - 1)
	{
		current_state++;
		total_states = current_state;
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				board_history[current_state][i][j] = BOARD[i][j];
			}
		}
	}
	else
	{
		cout << "limit reached!" << endl;
	}
}

void undo()
{
	if (current_state > 0)
	{
		current_state--;
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				BOARD[i][j] = board_history[current_state][i][j];
			}
		}
	}
	else
	{
		cout << "undos available!" << endl;
	}
}

void redo()
{
	if (current_state < total_states)
	{
		current_state++;
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				BOARD[i][j] = board_history[current_state][i][j];
			}
		}
	}
	else
	{
		cout << "redos unavailable" << endl;
	}
}

void setboard(const string &filename)
{

	ifstream fin(filename);
	if (!fin)
	{
		cerr << "file not found!" << endl;
		return;
	}

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			fin >> BOARD[i][j];
		}
	}

	fin >> turn;

	fin.close();
}

void saveboard(const string &filename, bool lastmove)
{

	ofstream fout(filename);

	if (lastmove)
	{
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				fout << BOARD[i][j];
			}
			fout << endl;
		}
		fout << turn;
	}
	else
	{
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				fout << BOARD[i][j];
			}
			fout << endl;
		}
		fout << (turn + 1) % 2;
	}

	fout.close();
}

void initialize(const string &filename)
{
	if (BOARD != nullptr)
	{
		for (int i = 0; i < SIZE; i++)
		{
			delete[] BOARD[i];
		}
		delete[] BOARD;
	}

	BOARD = new char *[SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		BOARD[i] = new char[SIZE];
	}

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			BOARD[i][j] = '.';
		}
	}

	ifstream fin(filename);
	if (!fin)
	{
		cerr << "file not found!" << endl;
		return;
	}

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			fin >> BOARD[i][j];
		}
	}

	fin >> turn;

	fin.close();
	save_board_state();
}

void display()
{
	for (int i = 0; i < SIZE; i++)
	{
		if (i == 0)
		{
			cout << "    ------------------------------------------------" << endl;
		}

		cout << i << "  ";
		cout << "|";
		for (int j = 0; j < SIZE; j++)
		{
			if (j == 0)
			{
				cout << "  ";
			}
			if (BOARD[i][j] == '.')
			{
				cout << ' ';
			}
			else
			{
				cout << BOARD[i][j];
			}
			if (j < SIZE)
			{
				cout << "  |  ";
			}
		}
		cout << endl;
		if (i < SIZE - 1)
			cout << "    ------------------------------------------------" << endl;
	}
	cout << "    ------------------------------------------------\n"
		 << endl;
	cout << "      0  |  1  |  2  |  3  |  4  |  5  |  6  |  7   " << endl;
}

bool is_valid_rook_move(int s_x, int s_y, int d_x, int d_y)
{

	if ((turn == 0 && islower(BOARD[d_x][d_y])) || (turn == 1 && isupper(BOARD[d_x][d_y])))
	{
		return false;
	}

	// check if not diagonal
	if (s_x != d_x && s_y != d_y)
	{
		return false;
	}

	if (s_x == d_x)
	{
		if (s_y < d_y)
		{
			for (int col = s_y + 1; col < d_y; col++)
			{
				if (BOARD[s_x][col] != '.')
				{
					return false;
				}
			}
		}
		else
		{
			for (int col = s_y - 1; col > d_y; col--)
			{
				if (BOARD[s_x][col] != '.')
				{
					return false;
				}
			}
		}
	}

	else if (s_y == d_y)
	{
		if (s_x < d_x)
		{
			for (int row = s_x + 1; row < d_x; row++)
			{
				if (BOARD[row][s_y] != '.')
				{
					return false;
				}
			}
		}
		else
		{
			for (int row = s_x - 1; row > d_x; row--)
			{
				if (BOARD[row][s_y] != '.')
				{
					return false;
				}
			}
		}
	}

	return true;
}

bool is_valid_bishop_move(int s_x, int s_y, int d_x, int d_y)
{
	if ((turn == 0 && islower(BOARD[d_x][d_y])) || (turn == 1 && isupper(BOARD[d_x][d_y])))
	{
		return false;
	}

	if (abs(s_x - d_x) != abs(s_y - d_y))
	{
		return false;
	}

	int row_movement;

	if (d_x > s_x)
	{
		row_movement = 1;
	}
	else
	{
		row_movement = -1;
	}

	int col_movement;
	if (d_y > s_y)
	{
		col_movement = 1;
	}
	else
	{
		col_movement = -1;
	}

	int row = s_x + row_movement;
	int col = s_y + col_movement;

	while (row != d_x && col != d_y)
	{
		if (BOARD[row][col] != '.')
		{
			return false;
		}
		row = row + row_movement;
		col = col + col_movement;
	}

	return true;
}

bool is_valid_queen_move(int s_x, int s_y, int d_x, int d_y)
{

	return (is_valid_bishop_move(s_x, s_y, d_x, d_y) || is_valid_rook_move(s_x, s_y, d_x, d_y));
}

bool is_valid_knight_move(int s_x, int s_y, int d_x, int d_y)
{

	int dx = abs(d_x - s_x);
	int dy = abs(d_y - s_y);

	if (!((dx == 2 && dy == 1) || (dx == 1 && dy == 2)))
	{
		return false;
	}

	if ((turn == 0 && islower(BOARD[d_x][d_y])) || (turn == 1 && isupper(BOARD[d_x][d_y])))
	{
		return false;
	}

	return true;
}

bool is_valid_king_move(int s_x, int s_y, int d_x, int d_y)
{
	int dx = abs(d_x - s_x);
	int dy = abs(d_y - s_y);

	if (!(dx <= 1 && dy <= 1))
	{
		return false;
	}

	if ((turn == 0 && islower(BOARD[d_x][d_y])) || (turn == 1 && isupper(BOARD[d_x][d_y])))
	{
		return false;
	}

	return true;
}

bool is_valid_black_pawn_move(int s_x, int s_y, int d_x, int d_y)
{
	int dx = d_x - s_x;
	int dy = abs(d_y - s_y);

	if (dx == -1 && dy == 0 && BOARD[d_x][d_y] == '.')
	{
		return true;
	}

	if (dx == -1 && dy == 1 && islower(BOARD[d_x][d_y]))
	{
		return true;
	}

	if (s_x == 6 && dx == -2 && dy == 0 && BOARD[d_x][d_y] == '.' && BOARD[s_x - 1][s_y] == '.')
	{
		return true;
	}

	return false;
}

bool is_valid_white_pawn_move(int s_x, int s_y, int d_x, int d_y)
{
	int dx = d_x - s_x;
	int dy = abs(d_y - s_y);

	if (dx == 1 && dy == 0 && BOARD[d_x][d_y] == '.')
	{
		return true;
	}

	if (dx == 1 && dy == 1 && isupper(BOARD[d_x][d_y]))
	{
		return true;
	}

	if (s_x == 1 && dx == 2 && dy == 0 && BOARD[d_x][d_y] == '.' && BOARD[s_x + 1][s_y] == '.')
	{
		return true;
	}

	return false;
}

bool is_castling_valid(int s_x, int s_y, int d_x, int d_y)
{

	char king = 'k';
	if (turn == 0)
	{
		king = 'k';
	}
	else if (turn == 1)
	{
		king = 'K';
	}

	char rook = 'r';
	if (turn == 0)
	{
		rook = 'r';
	}
	else if (turn == 1)
	{
		rook = 'R';
	}

	if (BOARD[s_x][s_y] != king || abs(d_y - s_y) != 2 || s_x != d_x)
	{
		return false;
	}

	// checking which side
	int rook_y;
	if (d_y > s_y)
	{
		rook_y = 7;
	}
	else
	{
		rook_y = 0;
	}

	if (BOARD[s_x][rook_y] != rook)
	{
		return false;
	}

	int move;
	if (d_y > s_y)
	{
		move = 1;
	}
	else
	{
		move = -1;
	}

	for (int y = s_y + move; y != d_y; y = y + move)
	{
		if (BOARD[s_x][y] != '.' || is_check(turn))
		{
			return false;
		}
	}

	return true;
}

void do_castling(int s_x, int s_y, int d_x, int d_y)
{

	char king = 'k';
	if (turn == 0)
	{
		king = 'k';
	}
	else if (turn == 1)
	{
		king = 'K';
	}

	char rook = 'r';
	if (turn == 0)
	{
		rook = 'r';
	}
	else if (turn == 1)
	{
		rook = 'R';
	}

	int rook_y;
	if (d_y > s_y)
	{
		rook_y = 7;
	}
	else
	{
		rook_y = 0;
	}

	int move;
	if (d_y > s_y)
	{
		move = 1;
	}
	else
	{
		move = -1;
	}

	BOARD[s_x][s_y] = '.';
	BOARD[d_x][d_y] = king;
	BOARD[s_x][rook_y] = '.';
	BOARD[s_x][s_y + move] = rook;
}

bool is_enpassant_valid(int s_x, int s_y, int d_x, int d_y)
{

	char pawn = 'p';
	if (turn == 0)
	{
		pawn = 'p';
	}
	else if (turn == 1)
	{
		pawn = 'P';
	}

	char opponent_pawn = 'P';
	if (turn == 0)
	{
		opponent_pawn = 'P';
	}
	else if (turn == 1)
	{
		opponent_pawn = 'p';
	}

	if (BOARD[s_x][s_y] != pawn || abs(d_x - s_x) != 1 || abs(d_y - s_y) != 1)
	{
		return false;
	}

	// Check opponent pawn's position
	int capture_x = 4;
	if (turn == 0)
	{
		if (d_x > 0)
		{
			capture_x = d_x - 1;
		}
	}
	else if (turn == 1)
	{
		if (d_x < 7)
		{
			capture_x = d_x + 1;
		}
	}

	if (BOARD[capture_x][d_y] != opponent_pawn)
	{
		return false;
	}

	return true;
}

void do_enpassant(int s_x, int s_y, int d_x, int d_y)
{

	char pawn = 'p';
	if (turn == 0)
	{
		pawn = 'p';
	}
	else if (turn == 1)
	{
		pawn = 'P';
	}

	int capture_x;
	if (turn == 0)
	{
		capture_x = d_x - 1;
	}
	else if (turn == 1)
	{
		capture_x = d_x + 1;
	}

	BOARD[capture_x][d_y] = '.';
	BOARD[d_x][d_y] = pawn;
	BOARD[s_x][s_y] = '.';
}

bool valid_source(int x, int y)
{
	if ((x < 0 || x >= SIZE) ||
		(y < 0 || y >= SIZE))
	{
		return false;
	}

	char piece;
	piece = BOARD[x][y];

	if (turn == 0 && islower(piece))
	{
		return true;
	}
	if (turn == 1 && isupper(piece))
	{
		return true;
	}

	return false;
}

bool is_check(int turn)
{

	char king;
	if (turn == 0)
	{
		king = 'k';
	}
	else if (turn == 1)
	{
		king = 'K';
	}

	int king_x = -1, king_y = -1;

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (BOARD[i][j] == king)
			{
				king_x = i;
				king_y = j;
			}
		}
		if (king_x != -1)
		{
			break;
		}
	}

	char piece;
	change_turn();
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			piece = BOARD[i][j];

			// other turn piece checking
			if ((turn == 0 && isupper(piece)) || (turn == 1 && islower(piece)))
			{

				if (valid_destination(i, j, king_x, king_y))
				{
					change_turn();
					return true;
				}
			}
		}
	}
	change_turn();
	return false;
}

bool is_stalemate()
{
	if (is_check(turn))
	{
		return false;
	}

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if ((turn == 0 && islower(BOARD[i][j])) || (turn == 1 && isupper(BOARD[i][j])))
			{
				for (int x = 0; x < SIZE; x++)
				{
					for (int y = 0; y < SIZE; y++)
					{
						if (valid_destination(i, j, x, y))
						{
							char temp = BOARD[x][y];
							BOARD[x][y] = BOARD[i][j];
							BOARD[i][j] = '.';

							if (!is_check(turn))
							{
								BOARD[i][j] = BOARD[x][y];
								BOARD[x][y] = temp;
								return false;
							}
							BOARD[i][j] = BOARD[x][y];
							BOARD[x][y] = temp;
						}
					}
				}
			}
		}
	}

	return true;
}

bool is_checkmate()
{
	if (!is_check(turn))
	{
		return false;
	}

	for (int s_x = 0; s_x < SIZE; s_x++)
	{
		for (int s_y = 0; s_y < SIZE; s_y++)
		{
			if (valid_source(s_x, s_y))
			{
				for (int d_x = 0; d_x < SIZE; d_x++)
				{
					for (int d_y = 0; d_y < SIZE; d_y++)
					{
						if (valid_destination(s_x, s_y, d_x, d_y))
						{
							char temp = BOARD[d_x][d_y];
							BOARD[d_x][d_y] = BOARD[s_x][s_y];
							BOARD[s_x][s_y] = '.';
							if (!is_check(turn))
							{
								BOARD[s_x][s_y] = BOARD[d_x][d_y];
								BOARD[d_x][d_y] = temp;
								return false;
							}

							BOARD[s_x][s_y] = BOARD[d_x][d_y];
							BOARD[d_x][d_y] = temp;
						}
					}
				}
			}
		}
	}

	return true;
}

bool valid_destination(int s_x, int s_y, int d_x, int d_y)
{

	if ((d_x < 0 || d_x >= SIZE) ||
		(d_y < 0 || d_y >= SIZE))
	{
		return false;
	}

	if (BOARD[s_x][s_y] == 'R' || BOARD[s_x][s_y] == 'r')
	{
		if (is_valid_rook_move(s_x, s_y, d_x, d_y))
		{
			return true;
		}
	}
	else if (BOARD[s_x][s_y] == 'B' || BOARD[s_x][s_y] == 'b')
	{
		if (is_valid_bishop_move(s_x, s_y, d_x, d_y))
		{
			return true;
		}
	}
	else if (BOARD[s_x][s_y] == 'N' || BOARD[s_x][s_y] == 'n')
	{
		if (is_valid_knight_move(s_x, s_y, d_x, d_y))
		{
			return true;
		}
	}
	else if (BOARD[s_x][s_y] == 'Q' || BOARD[s_x][s_y] == 'q')
	{
		if (is_valid_queen_move(s_x, s_y, d_x, d_y))
		{
			return true;
		}
	}
	else if (BOARD[s_x][s_y] == 'K' || BOARD[s_x][s_y] == 'k')
	{
		if (is_valid_king_move(s_x, s_y, d_x, d_y))
		{
			return true;
		}
		else if (is_castling_valid(s_x, s_y, d_x, d_y))
		{
			return true;
		}
	}
	else if (BOARD[s_x][s_y] == 'P')
	{
		if (is_valid_black_pawn_move(s_x, s_y, d_x, d_y))
		{
			return true;
		}
		else if (is_enpassant_valid(s_x, s_y, d_x, d_y))
		{
			return true;
		}
	}
	else if (BOARD[s_x][s_y] == 'p')
	{
		if (is_valid_white_pawn_move(s_x, s_y, d_x, d_y))
		{
			return true;
		}
		else if (is_enpassant_valid(s_x, s_y, d_x, d_y))
		{
			return true;
		}
	}

	return false;
}

void pawn_promotion(int d_x, int d_y)
{

	if (BOARD[d_x][d_y] == 'p' && d_x == 7)
	{
		char promotion_choice;
		do
		{
			cout << "promote pawn: \n";
			cout << "q: Queen, r: Rook, b: Bishop, n: Knight ";
			cin >> promotion_choice;
			promotion_choice = tolower(promotion_choice);
			if (promotion_choice == 'q')
			{
				BOARD[d_x][d_y] = 'q';
			}
			else if (promotion_choice == 'r')
			{
				BOARD[d_x][d_y] = 'r';
			}
			else if (promotion_choice == 'b')
			{
				BOARD[d_x][d_y] = 'b';
			}
			else if (promotion_choice == 'n')
			{
				BOARD[d_x][d_y] = 'n';
			}
		} while (promotion_choice != 'q' &&
				 promotion_choice != 'r' &&
				 promotion_choice != 'b' &&
				 promotion_choice != 'n');
	}
	else if (BOARD[d_x][d_y] == 'P' && d_x == 0)
	{
		char promotion_choice;
		do
		{
			cout << "promote pawn: \n";
			cout << "Q: Queen, R: Rook, B: Bishop, N: Knight ";
			cin >> promotion_choice;
			promotion_choice = toupper(promotion_choice);
			if (promotion_choice == 'Q')
			{
				BOARD[d_x][d_y] = 'Q';
			}
			else if (promotion_choice == 'R')
			{
				BOARD[d_x][d_y] = 'R';
			}
			else if (promotion_choice == 'B')
			{
				BOARD[d_x][d_y] = 'B';
			}
			else if (promotion_choice == 'N')
			{
				BOARD[d_x][d_y] = 'N';
			}
		} while (promotion_choice != 'Q' &&
				 promotion_choice != 'R' &&
				 promotion_choice != 'B' &&
				 promotion_choice != 'N');
	}
}

int evaluate_board()
{

	int score = 0;
	char piece;

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			piece = BOARD[i][j];

			if (piece == 'P')
			{
				score = score + 1;
			}
			else if (piece == 'N')
			{
				score = score + 3;
			}
			else if (piece == 'B')
			{
				score = score + 3;
			}
			else if (piece == 'R')
			{
				score = score + 5;
			}
			else if (piece == 'Q')
			{
				score = score + 9;
			}
			else if (piece == 'K')
			{
				score = score + 1000;
			}
			else if (piece == 'p')
			{
				score = score - 1;
			}
			else if (piece == 'n')
			{
				score = score - 3;
			}
			else if (piece == 'b')
			{
				score = score - 3;
			}
			else if (piece == 'r')
			{
				score = score - 5;
			}
			else if (piece == 'q')
			{
				score = score - 9;
			}
			else if (piece == 'k')
			{
				score = score - 1000;
			}
		}
	}

	return score;
}

int minimax(int depth, bool is_maximizer)
{

	// critical ndoes
	if (is_checkmate() || is_stalemate())
	{
		return evaluate_board();
	}

	if (depth == 0)
	{
		return evaluate_board();
	}

	if (is_maximizer)
	{
		int max_evaluation = -1000;

		for (int s_x = 0; s_x < SIZE; s_x++)
		{
			for (int s_y = 0; s_y < SIZE; s_y++)
			{
				if (valid_source(s_x, s_y))
				{
					for (int d_x = 0; d_x < SIZE; d_x++)
					{
						for (int d_y = 0; d_y < SIZE; d_y++)
						{
							if (valid_destination(s_x, s_y, d_x, d_y))
							{
								// move testing

								char temp = BOARD[d_x][d_y];
								BOARD[d_x][d_y] = BOARD[s_x][s_y];
								BOARD[s_x][s_y] = '.';

								if (usermove_valid(s_x, s_y, d_x, d_y))
								{

									int eval = minimax(depth - 1, false);

									BOARD[s_x][s_y] = BOARD[d_x][d_y];
									BOARD[d_x][d_y] = temp;

									if (eval > max_evaluation)
									{
										max_evaluation = eval;
									}
								}
								else
								{
									BOARD[s_x][s_y] = BOARD[d_x][d_y];
									BOARD[d_x][d_y] = temp;
								}
							}
						}
					}
				}
			}
		}
		return max_evaluation;
	}
	else
	{
		int min_evaluation = 1000;

		for (int s_x = 0; s_x < SIZE; s_x++)
		{
			for (int s_y = 0; s_y < SIZE; s_y++)
			{
				if (valid_source(s_x, s_y))
				{
					for (int d_x = 0; d_x < SIZE; d_x++)
					{
						for (int d_y = 0; d_y < SIZE; d_y++)
						{
							if (valid_destination(s_x, s_y, d_x, d_y))
							{

								char temp = BOARD[d_x][d_y];
								BOARD[d_x][d_y] = BOARD[s_x][s_y];
								BOARD[s_x][s_y] = '.';

								if (aimove_valid(s_x, s_y, d_x, d_y))
								{

									int eval = minimax(depth - 1, true);

									BOARD[s_x][s_y] = BOARD[d_x][d_y];
									BOARD[d_x][d_y] = temp;

									if (eval < min_evaluation)
									{
										min_evaluation = eval;
									}
								}
								else
								{
									BOARD[s_x][s_y] = BOARD[d_x][d_y];
									BOARD[d_x][d_y] = temp;
								}
							}
						}
					}
				}
			}
		}
		return min_evaluation;
	}
}

void ai_move()
{
	int best_score = 10000;
	int best_move[4] = {-1, -1, -1, -1};

	for (int s_x = 0; s_x < SIZE; s_x++)
	{
		for (int s_y = 0; s_y < SIZE; s_y++)
		{
			if (valid_source(s_x, s_y))
			{
				for (int d_x = 0; d_x < SIZE; d_x++)
				{
					for (int d_y = 0; d_y < SIZE; d_y++)
					{
						if (valid_destination(s_x, s_y, d_x, d_y))
						{

							char temp = BOARD[d_x][d_y];
							BOARD[d_x][d_y] = BOARD[s_x][s_y];
							BOARD[s_x][s_y] = '.';

							if (!is_check(turn))
							{

								int score = minimax(2, false); // depth > 3 takes long time

								BOARD[s_x][s_y] = BOARD[d_x][d_y];
								BOARD[d_x][d_y] = temp;

								if (score < best_score)
								{
									best_score = score;
									best_move[0] = s_x;
									best_move[1] = s_y;
									best_move[2] = d_x;
									best_move[3] = d_y;
								}
							}
							else
							{

								BOARD[s_x][s_y] = BOARD[d_x][d_y];
								BOARD[d_x][d_y] = temp;
							}
						}
					}
				}
			}
		}
	}

	if (best_move[0] != -1)
	{
		cout << "ai moved from " << best_move[0] << ", " << best_move[1] << " to " << best_move[2] << ", " << best_move[3] << endl;
		make_move(best_move[0], best_move[1], best_move[2], best_move[3]);
	}
}

bool move_valid(int s_x, int s_y, int d_x, int d_y)
{

	char temp;
	temp = BOARD[d_x][d_y];
	BOARD[d_x][d_y] = BOARD[s_x][s_y];
	BOARD[s_x][s_y] = '.';

	if (is_check(turn))
	{

		BOARD[s_x][s_y] = BOARD[d_x][d_y];
		BOARD[d_x][d_y] = temp;
		cout << "king is in check!!\n";
		return false;
	}
	BOARD[s_x][s_y] = BOARD[d_x][d_y];
	BOARD[d_x][d_y] = temp;

	return true;
}

bool usermove_valid(int s_x, int s_y, int d_x, int d_y)
{

	char temp;
	temp = BOARD[d_x][d_y];
	BOARD[d_x][d_y] = BOARD[s_x][s_y];
	BOARD[s_x][s_y] = '.';

	if (is_check(turn))
	{

		BOARD[s_x][s_y] = BOARD[d_x][d_y];
		BOARD[d_x][d_y] = temp;
		return false;
	}
	BOARD[s_x][s_y] = BOARD[d_x][d_y];
	BOARD[d_x][d_y] = temp;

	return true;
}

bool aimove_valid(int s_x, int s_y, int d_x, int d_y)
{

	char temp;
	temp = BOARD[d_x][d_y];
	BOARD[d_x][d_y] = BOARD[s_x][s_y];
	BOARD[s_x][s_y] = '.';

	if (is_check(turn))
	{

		BOARD[s_x][s_y] = BOARD[d_x][d_y];
		BOARD[d_x][d_y] = temp;
		return false;
	}
	BOARD[s_x][s_y] = BOARD[d_x][d_y];
	BOARD[d_x][d_y] = temp;

	return true;
}

void make_move(int s_x, int s_y, int d_x, int d_y)
{

	saveboard("perviousmove.txt", true);

	if (is_castling_valid(s_x, s_y, d_x, d_y))
	{
		cout << "castling...\n";
		do_castling(s_x, s_y, d_x, d_y);
	}
	else if (is_enpassant_valid(s_x, s_y, d_x, d_y))
	{
		cout << "enpassant...\n";
		do_enpassant(s_x, s_y, d_x, d_y);
	}
	else
	{

		BOARD[d_x][d_y] = BOARD[s_x][s_y];
		BOARD[s_x][s_y] = '.';

		pawn_promotion(d_x, d_y);
	}

	saveboard("currentmove.txt", false);
}

void change_turn()
{
	{
		turn++;
		turn = turn % 2;
	}
}

void clearing_memory()
{

	if (BOARD != nullptr)
	{
		for (int i = 0; i < SIZE; i++)
		{
			delete[] BOARD[i];
		}
		delete[] BOARD;
		BOARD = nullptr;
	}
}

void clear_screen()
{
	system("cls");
}

bool gameover()
{
	/*
	if (is_checkmate(turn)) {

	}
	*/

	return false;
}

void replay(char &choice)
{
	cout << "DO YOU WANT TO PLAY AGAIN? (Y/N):" << endl;
	cin >> choice;
}