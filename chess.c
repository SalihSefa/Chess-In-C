#include <stdio.h>
#include <stdlib.h>

void itworks(int num)
{
	return;
}

void itworksp(int num)
{
	return;
}

typedef struct Piece
{
	char name[2];
	char sign;
	char coordinate[2];
} piece;

typedef struct MoveTreeNode
{
	int startX;
	int startY;
	int finishX;
	int finishY;
	int score;
	int is_rock;
	struct MoveTreeNode** children;
	int children_count;
} MoveTreeNode;

typedef struct PossMove
{
	int startX;
	int startY;
	int finishX;
	int finishY;
	int score;
	int is_rock;
} PossMove;

PossMove poss_move;

MoveTreeNode* createMoveTreeNode(int startX, int startY, int finishX, int finishY)
{
	MoveTreeNode* newNode = (MoveTreeNode*)malloc(sizeof(MoveTreeNode));
	newNode->startX = startX;
	newNode->startY = startY;
	newNode->finishX = finishX;
	newNode->finishY = finishY;
	newNode->children = NULL;
	newNode->score = 0;
	newNode->is_rock = 0;
	newNode->children_count = 0;
	return newNode;
}

MoveTreeNode* root;

void add_child(MoveTreeNode* parent, int startX, int startY, int finishX, int finishY)
{
	parent->children_count++;
	parent->children = (MoveTreeNode**)realloc(parent->children, parent->children_count * sizeof(MoveTreeNode*));
	MoveTreeNode* child = (MoveTreeNode*)malloc(sizeof(MoveTreeNode));
	parent->children[parent->children_count - 1] = child;
	child->startX = startX;
	child->startY = startY;
	child->finishX = finishX;
	child->finishY = finishY;
	child->children = NULL;
	child->score = parent->score;
	child->is_rock = 0;
	child->children_count = 0;
}

void add_child_rock(MoveTreeNode* parent, int startX, int startY, int finishX, int finishY)
{
	parent->children_count++;
	parent->children = (MoveTreeNode**)realloc(parent->children, parent->children_count * sizeof(MoveTreeNode*));
	MoveTreeNode* child = (MoveTreeNode*)malloc(sizeof(MoveTreeNode));
	parent->children[parent->children_count - 1] = child;
	child->startX = startX;
	child->startY = startY;
	child->finishX = finishX;
	child->finishY = finishY;
	child->children = NULL;
	child->score = parent->score;
	child->is_rock = 1;
	child->children_count = 0;
}

void clear_root(MoveTreeNode** root)
{
	for(int i = 0; i < (*root)->children_count; i++)
	{
		free((*root)->children[i]);
	}
	free((*root));
	root = NULL;
}

char map[18][27] = {
	{' ', ' ', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+'},
	{'8', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|'},
	{' ', ' ', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+'},
	{'7', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|'},
	{' ', ' ', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+'},
	{'6', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|'},
	{' ', ' ', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+'},
	{'5', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|'},
	{' ', ' ', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+'},
	{'4', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|'},
	{' ', ' ', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+'},
	{'3', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|'},
	{' ', ' ', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+'},
	{'2', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|'},
	{' ', ' ', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+'},
	{'1', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|'},
	{' ', ' ', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+', '-', '-', '+'},
	{' ', ' ', ' ', 'a', ' ', ' ', 'b', ' ', ' ', 'c', ' ', ' ', 'd', ' ', ' ', 'e', ' ', ' ', 'f', ' ', ' ', 'g', ' ', ' ', 'h', ' ', ' '},

};

char player_piece_coordinates[16][3] = {
	{"a1r"}, {"b1k"}, {"c1b"}, {"d1Q"}, {"e1W"}, {"f1b"}, {"g1k"}, {"h1r"},
	{"a2p"}, {"b2p"}, {"c2p"}, {"d2p"}, {"e2p"}, {"f2p"}, {"g2p"}, {"h2p"}
};

char computer_piece_coordinates[16][3] = {
	{"a7p"}, {"b7p"}, {"c7p"}, {"d7p"}, {"e7p"}, {"f7p"}, {"g7p"}, {"h7p"},
	{"a8r"}, {"b8k"}, {"c8b"}, {"d8Q"}, {"e8W"}, {"f8b"}, {"g8k"}, {"h8r"}
};

int p_value_map[8][8] = {
	{2, 3, 4, 5, 5, 4, 3, 2},
	{3, 4, 5, 6, 6, 5, 4, 3},
	{4, 5, 6, 7, 7, 6, 5, 4},
	{5, 6, 7, 8, 8, 7, 6, 5},
	{5, 6, 7, 8, 8, 7, 6, 5},
	{4, 5, 6, 7, 7, 6, 5, 4},
	{3, 4, 5, 6, 6, 5, 4, 3},
	{2, 3, 4, 5, 5, 4, 3, 2}
};

int r_value_map[8][8] = {
	{3, 4, 4, 4, 4, 4, 4, 3},
	{4, 6, 6, 6, 6, 6, 6, 4},
	{4, 6, 8, 8, 8, 8, 6, 4},
	{4, 6, 8,10,10, 8, 6, 4},
	{4, 6, 8,10,10, 8, 6, 4},
	{4, 6, 8, 8, 8, 8, 6, 4},
	{4, 6, 6, 6, 6, 6, 6, 4},
	{3, 4, 4, 4, 4, 4, 4, 3}
};

int k_value_map[8][8] = {
	{2, 3, 4, 4, 4, 4, 3, 2},
	{3, 4, 6, 6, 6, 6, 4, 3},
	{4, 6, 8, 8, 8, 8, 6, 4},
	{4, 6, 8, 8, 8, 8, 6, 4},
	{4, 6, 8, 8, 8, 8, 6, 4},
	{4, 6, 8, 8, 8, 8, 6, 4},
	{3, 4, 6, 6, 6, 6, 4, 3},
	{2, 3, 4, 4, 4, 4, 3, 2}
};

int b_value_map[8][8] = {
	{1, 2, 3, 4, 4, 3, 2, 1},
	{2, 4, 6, 7, 7, 6, 4, 2},
	{3, 6, 8, 9, 9, 8, 6, 3},
	{4, 7, 9,10,10, 9, 7, 4},
	{4, 7, 9,10,10, 9, 7, 4},
	{3, 6, 8, 9, 9, 8, 6, 3},
	{2, 4, 6, 7, 7, 6, 4, 2},
	{1, 2, 3, 4, 4, 3, 2, 1}
};

int Q_value_map[8][8] = {
	{4, 5, 6, 7, 7, 6, 5, 4},
	{5, 7, 8, 9, 9, 8, 7, 5},
	{6, 8,10,11,11,10, 8, 6},
	{7, 9,11,12,12,11, 9, 7},
	{7, 9,11,12,12,11, 9, 7},
	{6, 8,10,11,11,10, 8, 6},
	{5, 7, 8, 9, 9, 8, 7, 5},
	{4, 5, 6, 7, 7, 6, 5, 4}
};

int W_value_map[8][8] = {
	{6, 5, 4, 3, 3, 4, 5, 6},
	{5, 4, 3, 2, 2, 3, 4, 5},
	{4, 3, 2, 1, 1, 2, 3, 4},
	{3, 2, 1, 0, 0, 1, 2, 3},
	{3, 2, 1, 0, 0, 1, 2, 3},
	{4, 3, 2, 1, 1, 2, 3, 4},
	{5, 4, 3, 2, 2, 3, 4, 5},
	{6, 5, 4, 3, 3, 4, 5, 6}
};

int piece_value(char piece)
{
	if(piece == 'p') return 1;
	else if(piece == 'k') return 3;
	else if(piece == 'b') return 3;
	else if(piece == 'r') return 5;
	else if(piece == 'Q') return 9;
	else return 0;
}

int convert_X_to_real_index(char Xcoordinate)
{
	int x = Xcoordinate - 96;
	return x;
}

int convert_Y_to_real_index(char Ycoordinate)
{
	int y = -1*Ycoordinate + 57;
	return y;
}

int convert_X_to_board_index(char Xcoordinate)
{
	int x = 3*Xcoordinate - 288;
	return x;
}

int convert_Y_to_board_index(char Ycoordinate)
{
	int y = -2*Ycoordinate + 113;
	return y;
}

char convert_board_index_to_X(int x)
{
	char c;
	c = (x + 288) / 3;
	c = (char)c;
	return c;
}

char convert_board_index_to_Y(int y)
{
	char c;
	c = (y - 113) / -2;
	c = (char)c;
	return c;
}

int player_piece_values()
{
	int value = 0;
	for(int i = 0; i < 16; i++)
	{
		int x_board = convert_X_to_board_index(player_piece_coordinates[i][0]);
		int y_board = convert_Y_to_board_index(player_piece_coordinates[i][1]);
		
		char piece = map[y_board][x_board];
		value += piece_value(piece);
	}
	return value;
}

int computer_piece_values()
{
	int value = 0;
	for(int i = 0; i < 16; i++)
	{
		int x_board = convert_X_to_board_index(computer_piece_coordinates[i][0]);
		int y_board = convert_Y_to_board_index(computer_piece_coordinates[i][1]);
		
		char piece = map[y_board][x_board];
		value += piece_value(piece);
	}
	return value;
}


int player_adjustment_values()
{return 1;}

int computer_adjustment_values(int startX, int startY, int finishX, int finishY)
{
	int value_board_start_x = (startX / 3) - 1;
	int value_board_finish_x = (finishX / 3) - 1;
	
	int value_board_start_y = (startY / 2) - (1 / 2);
	int value_board_finish_y = (finishY / 2) - (1 / 2);
	
	int score;
	
	if(map[startY][startX] == 'p')
	{
		score = p_value_map[value_board_finish_y][value_board_finish_x] - p_value_map[value_board_start_y][value_board_start_x];
	}
	else if(map[startY][startX] == 'r')
	{
		score = r_value_map[value_board_finish_y][value_board_finish_x] - r_value_map[value_board_start_y][value_board_start_x];
	}
	else if(map[startY][startX] == 'k')
	{
		score = k_value_map[value_board_finish_y][value_board_finish_x] - k_value_map[value_board_start_y][value_board_start_x];
	}
	else if(map[startY][startX] == 'b')
	{
		score = b_value_map[value_board_finish_y][value_board_finish_x] - b_value_map[value_board_start_y][value_board_start_x];
	}
	else if(map[startY][startX] == 'Q')
	{
		score = Q_value_map[value_board_finish_y][value_board_finish_x] - Q_value_map[value_board_start_y][value_board_start_x];
	}
	else if(map[startY][startX] == 'W')
	{
		// rok puan hesaplama
		if(finishX == startX + 9)
		{
			score = W_value_map[value_board_finish_y][value_board_start_x + 2] - W_value_map[value_board_start_y][value_board_start_x]; // wf - ws
			score += r_value_map[value_board_finish_y][value_board_start_x + 1] - r_value_map[value_board_finish_y][value_board_finish_x];// rf - rs
			// score = wfark + rfark
		}
		// rok puan hesaplama
		else if(finishX = startX - 12)
		{
			score = W_value_map[value_board_finish_y][value_board_start_x - 2] - W_value_map[value_board_start_y][value_board_start_x]; // wf - ws
			score += r_value_map[value_board_finish_y][value_board_start_x - 1] - r_value_map[value_board_finish_y][value_board_finish_x];// rf - rs
			// score = wfark + rfark
		}
		// normal oynamada puan hesaplama
		else
		{
			score = W_value_map[value_board_finish_y][value_board_finish_x] - W_value_map[value_board_start_y][value_board_start_x];			
		}
	}
	
	return score;
	
}

int isPlayerPiece(int board_X, int board_Y)
{
	char x = convert_board_index_to_X(board_X);
	char y = convert_board_index_to_Y(board_Y);
	
	int is_player_piece = 0;
	
	for(int i = 0; i < 16; i++)
	{
		if(x == player_piece_coordinates[i][0] && y == player_piece_coordinates[i][1] && player_piece_coordinates[i][2] != ' ')
		{
			is_player_piece = 1;
			break;
		}
	}
	return is_player_piece;
}

int isComputerPiece(int board_X, int board_Y)
{
	char x = convert_board_index_to_X(board_X);
	char y = convert_board_index_to_Y(board_Y);
	
	int is_computer_piece = 0;
	
	for(int i = 0; i < 16; i++)
	{
		if(x == computer_piece_coordinates[i][0] && y == computer_piece_coordinates[i][1] && computer_piece_coordinates[i][2] != ' ')
		{
			is_computer_piece = 1;
			break;
		}
	}
	return is_computer_piece;
}

void change_player_piece_coordinates(char start[2], char finish[2])
{
	for(int i = 0; i < 16; i++)
	{
		if(start[0] == player_piece_coordinates[i][0] && start[1] == player_piece_coordinates[i][1])
		{
			player_piece_coordinates[i][0] = finish[0];
			player_piece_coordinates[i][1] = finish[1];
			break;
		}
	}
}

void change_computer_piece_coordinates(char start[2], char finish[2])
{
	for(int i = 0; i < 16; i++)
	{
		if(start[0] == computer_piece_coordinates[i][0] && start[1] == computer_piece_coordinates[i][1])
		{
			computer_piece_coordinates[i][0] = finish[0];
			computer_piece_coordinates[i][1] = finish[1];
			break;
		}
	}
}

int is_player_checked()
{
	// bilgisayarın tüm taşları kontrol edilsin,
	// taşların hedeflerinde şah var mı,
	// varsa arada oyuncunun taşı var mı kontrol edilsin
	
	// oyuncunun şahının konumuna erişme
	int sah_board_x = convert_X_to_board_index(player_piece_coordinates[4][0]);
	int sah_board_y = convert_Y_to_board_index(player_piece_coordinates[4][1]);
	
	char sah_x = player_piece_coordinates[4][0];
	char sah_y = player_piece_coordinates[4][1];
	
	for(int i = 0; i < 16; i++)
	{
		char piece_x = computer_piece_coordinates[4][0];
		char piece_y = computer_piece_coordinates[4][1];
		
		int piece_board_x = convert_X_to_board_index(piece_x);
		int piece_board_y = convert_Y_to_board_index(piece_y);

		// piyonun şah çekmesi
		if(computer_piece_coordinates[i][2] == 'p' && ((sah_board_x - 3 == piece_board_x && sah_board_y - 2 == piece_board_y) || (sah_board_x + 3 == piece_board_x && sah_board_y - 2 == piece_board_y) ))
		{
			return 1;
		}
		
		// kalenin şah çekmesi
		else if(computer_piece_coordinates[i][2] == 'r' && (piece_board_x == sah_board_x || piece_board_y == sah_board_y))
		{
			if(!isInterceptLine(piece_board_x, piece_board_y, sah_board_x, sah_board_y))
			{
				return 1;
			}
		}
		
		// atın şah çekmesi
		else if(computer_piece_coordinates[i][2] == 'k')
		{
			if(((sah_board_x == piece_board_x - 3 || sah_board_x == piece_board_x + 3) && (sah_board_y == piece_board_y - 4 || sah_board_y == piece_board_y + 4)) || ((sah_board_x == piece_board_x - 6 || sah_board_x == piece_board_x + 6) && (sah_board_y == piece_board_y - 2 || sah_board_y == piece_board_y + 2)))
			{
				return 1;
			}
		}
		
		// filin şah çekmesi
		else if(computer_piece_coordinates[i][2] == 'b' && (float)(abs(piece_board_x - sah_board_x) / 3) / (abs(piece_board_y - sah_board_y) / 2) == 1)
		{
			if(!isInterceptCross(piece_board_x, piece_board_y, sah_board_x, sah_board_y))
			{
				return 1;
			}
		}
		
		// vezirin şah çekmesi
		else if(computer_piece_coordinates[i][2] == 'Q')
		{
			if(piece_x == sah_x || piece_y == sah_y)
			{
				if(!isInterceptLine(piece_board_x, piece_board_y, sah_board_x, sah_board_y))
				{
					return 1;
				}
			}
			else if((float)(abs(piece_board_x - sah_board_x) / 3) / (abs(piece_board_y - sah_board_y) / 2) == 1)
			{
				if(!isInterceptCross(piece_board_x, piece_board_y, sah_board_x, sah_board_y))
				{
					return 1;
				}
			}
		}
	}
	
	return 0;
}

int is_computer_checked()
{
	// oyuncunun tüm taşları kontrol edilsin,
	// taşların hedeflerinde şah var mı,
	// varsa arada bilgisayarın taşı var mı kontrol edilsin
	
	// bilgisayarın şahının konumuna erişme
	int sah_board_x = convert_X_to_board_index(computer_piece_coordinates[4][0]);
	int sah_board_y = convert_Y_to_board_index(computer_piece_coordinates[4][1]);
	
	char sah_x = computer_piece_coordinates[4][0];
	char sah_y = computer_piece_coordinates[4][1];
	
	for(int i = 0; i < 16; i++)
	{
		char piece_x = player_piece_coordinates[4][0];
		char piece_y = player_piece_coordinates[4][1];
		
		int piece_board_x = convert_X_to_board_index(piece_x);
		int piece_board_y = convert_Y_to_board_index(piece_y);

		// piyonun şah çekmesi
		if(player_piece_coordinates[i][2] == 'p' && ((sah_board_x - 3 == piece_board_x && sah_board_y + 2 == piece_board_y) || (sah_board_x + 3 == piece_board_x && sah_board_y + 2 == piece_board_y) ))
		{
			return 1;
		}
		
		// kalenin şah çekmesi
		else if(player_piece_coordinates[i][2] == 'r' && (piece_board_x == sah_board_x || piece_board_y == sah_board_y))
		{
			if(!isInterceptLine(piece_board_x, piece_board_y, sah_board_x, sah_board_y))
			{
				return 1;
			}
		}
		
		// atın şah çekmesi
		else if(player_piece_coordinates[i][2] == 'k')
		{
			if(((sah_board_x == piece_board_x - 3 || sah_board_x == piece_board_x + 3) && (sah_board_y == piece_board_y - 4 || sah_board_y == piece_board_y + 4)) || ((sah_board_x == piece_board_x - 6 || sah_board_x == piece_board_x + 6) && (sah_board_y == piece_board_y - 2 || sah_board_y == piece_board_y + 2)))
			{
				return 1;
			}
		}
		
		// filin şah çekmesi
		else if(player_piece_coordinates[i][2] == 'b' && (float)(abs(piece_board_x - sah_board_x) / 3) / (abs(piece_board_y - sah_board_y) / 2) == 1)
		{
			if(!isInterceptCross(piece_board_x, piece_board_y, sah_board_x, sah_board_y))
			{
				return 1;
			}
		}
		
		// vezirin şah çekmesi
		else if(player_piece_coordinates[i][2] == 'Q')
		{
			if(piece_x == sah_x || piece_y == sah_y)
			{
				if(!isInterceptLine(piece_board_x, piece_board_y, sah_board_x, sah_board_y))
				{
					return 1;
				}
			}
			else if((float)(abs(piece_board_x - sah_board_x) / 3) / (abs(piece_board_y - sah_board_y) / 2) == 1)
			{
				if(!isInterceptCross(piece_board_x, piece_board_y, sah_board_x, sah_board_y))
				{
					return 1;
				}
			}
		}
	}
	
	return 0;
}

int player_mated()
{return 1;}

int computer_mated()
{return 1;}

int is_draw()
{return 1;}

void print_map()
{
	for(int i = 0; i < 18; i++)
	{
		for(int j = 0; j < 27; j++)
		{
			printf("%c", map[i][j]);
		}
		printf("\n");
	}
}

void add_pieces(char sign)
{
	// add pawns
	for(int i = 3; i < 27; i += 3)
	{
		map[3][i] = 'p';
		map[13][i] = 'p';
	}
	
	// add rock
	map[1][3] = 'r';
	map[1][24] = 'r';
	map[15][3] = 'r';
	map[15][24] = 'r';
	
	// knight
	map[1][6] = 'k';
	map[1][21] = 'k';
	map[15][6] = 'k';
	map[15][21] = 'k';
	
	// bishop
	map[1][9] = 'b';
	map[1][18] = 'b';
	map[15][9] = 'b';
	map[15][18] = 'b';
	
	// queen
	map[1][12] = 'Q';
	map[15][12] = 'Q';
	
	// king
	map[1][15] = 'W';
	map[15][15] = 'W';
	
	// oyuncunun işaretini ekleme
	for(int i = 3; i < 27; i += 3)
	{
		map[13][i+1] = sign;
		map[15][i+1] = sign;
	}
 	
}

int isInterceptLine(startX, startY, finishX, finishY)
{
	int big;
	int intercept = 0;
	if (startY > finishY)
	{
		big = startY;
		for(int i = big - 2; i > finishY; i -= 2)
		{
			if(map[i][startX] != ' ')
			{
				intercept = 1;
				break;
			}
		}
	}
	else
	{
		big = finishY;
		for(int i = big - 2; i > startY; i -= 2)
		{
			if(map[i][startX] != ' ')
			{
				intercept = 1;
				break;
			}
		}
	}
	if (startX > finishX)
	{
		big = startX;
		for(int i = big - 3; i > finishX; i -= 3)
		{
			if(map[startY][i] != ' ')
			{
				intercept = 1;
				break;
			}
		}
	}
	else
	{
		big = finishX;
		for(int i = big - 3; i > startX; i -= 3)
		{
			if(map[startY][i] != ' ')
			{
				intercept = 1;
				break;
			}
		}
	}
	
	return intercept;
}

int isInterceptCross(startX, startY, finishX, finishY)
{
	int i = startY;
	int j = startX;
	int intercept = 0;
	if (startY > finishY && startX > finishX)
	{
		for(i = i - 2, j = j - 3; i > finishY && j > finishX; i -= 2, j -= 3)
		{
			if(map[i][j] != ' ')
			{
				intercept = 1;
				break;
			}
		}
	}
	else if (startY > finishY && startX < finishX)
	{
		for(i = i - 2, j = j + 3; i > finishY && j < finishX; i -= 2, j += 3)
		{
			if(map[i][j] != ' ')
			{
				intercept = 1;
				break;
			}
		}
	}
	else if (startY < finishY && startX > finishX)
	{
		for(i = i + 2, j = j - 3; i < finishY && j > finishX; i += 2, j -= 3)
		{
			if(map[i][j] != ' ')
			{
				intercept = 1;
				break;
			}
		}
	}
	else if (startY < finishY && startX < finishX)
	{
		for(i = i + 2, j = j + 3; i < finishY && j < finishX; i += 2, j += 3)
		{
			if(map[i][j] != ' ')
			{
				intercept = 1;
				break;
			}
		}
	}
	return intercept;
}

int depth;
int depth_counter;
int is_p_w_played;
int is_c_w_played;

// kullanılacak olan hamleye verilerin atanması
void poss_movement_assignment( int startX, int startY, int finishX, int finishY, int score)
{
	poss_move.startX = startX;
	poss_move.startY = startY;
	poss_move.finishX = finishX;
	poss_move.finishY = finishY;
	poss_move.score = score;
}

void calculate_score(MoveTreeNode* parent, int startX, int startY, int finishX, int finishY)
{
	int score;
	
	score = parent->score;
	score += computer_piece_values();
	score += computer_adjustment_values(startX, startY, finishX, finishY); // bu fonksiyona oyuncunun piyonlarını yeme puanı da eklenmeli
	char merhaba = map[finishY][finishX];
	score += piece_value(merhaba);
	
	if(score > poss_move.score)
	{
		poss_movement_assignment(startX, startY, finishX, finishY, score);
	}
}

void player_movements(MoveTreeNode* head_of_tree)
{
	int startX;
	int startY;
	int finishX;
	int finishY;
	
	MoveTreeNode* root = head_of_tree;
	
	
	// hareket olasılıklarını ağaca ekleme (rakip taşlarını yeme olasılıkları eklenecek)
	for(int i = 0; i < 16; i++)
	{
		startX = convert_X_to_board_index(player_piece_coordinates[i][0]);
		startY = convert_Y_to_board_index(player_piece_coordinates[i][1]);
		itworksp(1);
		
		//piyonların hareket olasılıkları
		if(player_piece_coordinates[i][2] == 'p')
		{
			if(map[startY - 4][startX] == ' ' && map[startY - 2][startX] == ' ' && startY == 13)
			{
				itworksp(2);
				add_child(root, startX, startY, startX, startY - 4);
			}
			if(map[startY - 2][startX] == ' ')
			{
				itworksp(12);
				add_child(root, startX, startY, startX, startY - 2);
			}
			
			// rakip piyonu yeme olasılıklarından: 
			if(map[startY - 2][startX + 3] != ' ' && startY - 2 > 0 && startX + 3 < 25 && !isPlayerPiece(startX + 3, startY - 2))
			{
				add_child(root, startX, startY, startX + 3, startY - 2);
				// oyuncu taşını yeme
				
			}
			if(map[startY - 2][startX - 3] != ' ' && startY - 2 > 0 && startX - 3 > 2 && !isPlayerPiece(startX - 3, startY - 2))
			{
				add_child(root, startX, startY, startX - 3, startY - 2);
				// oyuncu taşını yeme
				
			}
		}
		
		// kalelerin hareket olasılıkları
		else if(player_piece_coordinates[i][2] == 'r')
		{
			itworksp(3);
			// x eksenindeki hareketleri olasılık ağacına ekleme
			for(int i = startX + 3; i < 25; i += 3)
			{
				if(!isInterceptLine(startX, startY, i, startY) && i != startX && !isPlayerPiece(i, startY)) // i != startX -> kısmına gerek kalmamış olabilir
				{
					itworksp(16);
					add_child(root, startX, startY, i, startY);
				}
				else
				{
					itworksp(26);
					break;
				}
			}
			for(int i = startX - 3; i > 2; i -= 3)
			{
				if(!isInterceptLine(startX, startY, i, startY) && i != startX && !isPlayerPiece(i, startY))
				{
					itworksp(36);
					add_child(root, startX, startY, i, startY);
				}
				else
				{
					itworksp(46);
					break;
				}
			}
			
			// y eksenindeki hareketleri olasılık ağacına ekleme
			for(int i = startY + 2; i < 16; i += 2)
			{
				if(!isInterceptLine(startX, startY, startX, i) && i != startY && !isPlayerPiece(startX, i))
				{
					itworksp(56);
					add_child(root, startX, startY, startX, i);
				}
				else
				{
					itworksp(66);
					break;
				}
			}
			for(int i = startY - 2; i > 0; i -= 2)
			{
				if(!isInterceptLine(startX, startY, startX, i) && i != startY && !isPlayerPiece(startX, i))
				{
					add_child(root, startX, startY, startX, i);
				}
				else
				{
					break;
				}
			}
		}
		
		// atların hareket olasılıkları
		else if(player_piece_coordinates[i][2] == 'k')
		{
			itworksp(4);
			if(startX + 3 < 25 && startY + 4 < 16 && !isPlayerPiece(startX + 3, startY + 4))
			{
				add_child(root, startX, startY, startX + 3, startY + 4);
			}
			if(startX - 3 > 2 && startY + 4 < 16 && !isPlayerPiece(startX - 3, startY + 4))
			{
				add_child(root, startX, startY, startX - 3, startY + 4);
			}
			if(startX + 3 < 25 && startY - 4 > 0 && !isPlayerPiece(startX + 3, startY - 4))
			{
				add_child(root, startX, startY, startX + 3, startY - 4);
			}
			if(startX - 3 > 2 && startY - 4 > 0 && !isPlayerPiece(startX - 3, startY - 4))
			{
				add_child(root, startX, startY, startX - 3, startY - 4);
			}
			
			if(startX + 6 < 25 && startY + 2 < 16 && !isPlayerPiece(startX + 6, startY + 2))
			{
				add_child(root, startX, startY, startX + 6, startY + 2);
			}
			if(startX - 6 > 2 && startY + 2 < 16 && !isPlayerPiece(startX - 6, startY + 2))
			{
				add_child(root, startX, startY, startX - 6, startY + 2);
			}
			if(startX + 6 < 25 && startY - 2 < 16 && !isPlayerPiece(startX + 6, startY + 2))
			{
				add_child(root, startX, startY, startX + 6, startY + 2);
			}
			if(startX - 6 > 2 && startY - 2 < 16 && !isPlayerPiece(startX - 6, startY + 2))
			{
				add_child(root, startX, startY, startX - 6, startY + 2);
			}
		}
		
		
		// fillerin hareket olasılıkları
		else if(player_piece_coordinates[i][2] == 'b')
		{
			itworksp(5);
			// sağ aşağı
			for(int i = startX + 3, j = startY + 2; i < 25 && j < 16; i += 3, j += 2)
			{
				if(!isInterceptCross(startX, startY, i, j) && i != startX && !isPlayerPiece(i, j))
				{
					add_child(root, startX, startY, i, j);
				}
				else
				{
					break;
				}
			}
			// sağ yukarı
			for(int i = startX + 3, j = startY - 2; i < 25 && j > 0; i += 3, j -= 2)
			{
				if(!isInterceptCross(startX, startY, i, j) && i != startX && !isPlayerPiece(i, j))
				{
					add_child(root, startX, startY, i, j);
				}
				else
				{
					break;
				}
			}
			// sol aşağı
			for(int i = startX - 3, j = startY + 2; i > 2 && j < 16; i -= 3, j += 2)
			{
				itworksp(35);
				if(!isInterceptCross(startX, startY, i, j) && i != startX && !isPlayerPiece(i, j))
				{
					add_child(root, startX, startY, i, j);
				}
				else
				{
					break;					
				}
			}
			// sol yukarı
			for(int i = startX - 3, j = startY - 2; i > 2 && j > 0; i -= 3, j -= 2)
			{
				itworksp(45);
				if(!isInterceptCross(startX, startY, i, j) && i != startX && !isPlayerPiece(i, j))
				{
					add_child(root, startX, startY, i, j);
				}
				else
				{
					break;
				}
			}
		}
		
		
		// vezirin hareket olasılıkları
		else if(player_piece_coordinates[i][2] == 'Q')
		{
			itworksp(6);
			// kale ile aynı olasılıklar
			// x eksenindeki hareketleri olasılık ağacına ekleme
			for(int i = startX + 3; i < 25; i += 3)
			{
				if(!isInterceptLine(startX, startY, i, startY) && i != startX && !isPlayerPiece(i, startY)) // i != startX -> kısmına gerek kalmamış olabilir
				{
					itworksp(16);
					add_child(root, startX, startY, i, startY);
				}
				else
				{
					itworksp(26);
					break;
				}
			}
			for(int i = startX - 3; i > 2; i -= 3)
			{
				if(!isInterceptLine(startX, startY, i, startY) && i != startX && !isPlayerPiece(i, startY))
				{
					itworksp(36);
					add_child(root, startX, startY, i, startY);
				}
				else
				{
					itworksp(46);
					break;
				}
			}
			
			// y eksenindeki hareketleri olasılık ağacına ekleme
			for(int i = startY + 2; i < 16; i += 2)
			{
				if(!isInterceptLine(startX, startY, startX, i) && i != startY && !isPlayerPiece(startX, i))
				{
					itworksp(56);
					add_child(root, startX, startY, startX, i);
				}
				else
				{
					itworksp(66);
					break;
				}
			}
			for(int i = startY - 2; i > 0; i -= 2)
			{
				if(!isInterceptLine(startX, startY, startX, i) && i != startY && !isPlayerPiece(startX, i))
				{
					add_child(root, startX, startY, startX, i);
				}
				else
				{
					break;
				}
			}
			
			
			// fil ile aynı olasılıklar
			// sağ aşağı
			for(int i = startX + 3, j = startY + 2; i < 25 && j < 16; i += 3, j += 2)
			{
				if(!isInterceptCross(startX, startY, i, j) && i != startX && !isPlayerPiece(i, j))
				{
					add_child(root, startX, startY, i, j);
				}
				else
				{
					break;
				}
			}
			// sağ yukarı
			for(int i = startX + 3, j = startY - 2; i < 25 && j > 0; i += 3, j -= 2)
			{
				if(!isInterceptCross(startX, startY, i, j) && i != startX && !isPlayerPiece(i, j))
				{
					add_child(root, startX, startY, i, j);
				}
				else
				{
					break;
				}
			}
			// sol aşağı
			for(int i = startX - 3, j = startY + 2; i > 2 && j < 16; i -= 3, j += 2)
			{
				itworksp(35);
				if(!isInterceptCross(startX, startY, i, j) && i != startX && !isPlayerPiece(i, j))
				{
					add_child(root, startX, startY, i, j);
				}
				else
				{
					break;					
				}
			}
			// sol yukarı
			for(int i = startX - 3, j = startY - 2; i > 2 && j > 0; i -= 3, j -= 2)
			{
				itworksp(45);
				if(!isInterceptCross(startX, startY, i, j) && i != startX && !isPlayerPiece(i, j))
				{
					add_child(root, startX, startY, i, j);
				}
				else
				{
					break;
				}
			}
		}
		
		// şahın hareket olasılıkları
		else if(player_piece_coordinates[i][2] == 'W')
		{
			itworksp(7);
			// "şah" olma durumları da yazılmalı
			// sağ
			if(startX + 3 < 25 && !isPlayerPiece(startX + 3, startY))
			{
				add_child(root, startX, startY, startX + 3, startY);
			}
			// sol
			else if(startX - 3 > 2 && !isPlayerPiece(startX - 3, startY))
			{
				add_child(root, startX, startY, startX - 3, startY);
			}
			// aşağı
			else if(startY + 2 < 16 && !isPlayerPiece(startX, startY + 2))
			{
				add_child(root, startX, startY, startX, startY + 2);
			}
			// yukarı
			else if(startY - 2 > 0 && !isPlayerPiece(startX, startY - 2))
			{
				add_child(root, startX, startY, startX, startY - 2);
			}
			// sağ aşağı
			else if(startX + 3 < 25 && startY + 2 < 16 && !isPlayerPiece(startX + 3, startY + 2))
			{
				add_child(root, startX, startY, startX + 3, startY + 2);
			}
			// sağ yukarı
			else if(startX + 3 < 25 && startY - 2 > 0 && !isPlayerPiece(startX + 3, startY - 2))
			{
				add_child(root, startX, startY, startX + 3, startY - 2);
			}
			// sol aşağı
			else if(startX - 3 > 2 && startY + 2 < 16 && !isPlayerPiece(startX - 3, startY + 2))
			{
				add_child(root, startX, startY, startX - 3, startY + 2);
			}
			// sol yukarı
			else if(startX - 3 > 2 && startY - 2 > 0 && !isPlayerPiece(startX - 3, startY - 2))
			{
				add_child(root, startX, startY, startX - 3, startY - 2);
			}
			// sağdan rok atma
			else if(map[startY][startX + 9] == 'r' && !isInterceptLine(startX, startY, startX + 9, startY) && isComputerPiece(startX + 9, startY) && !is_computer_checked() && !is_c_w_played)
			{
				add_child(root, startX, startY, startX + 9, startY);
			}
			// soldan rok atma
			else if(map[startY][startX - 12] == 'r' && !isInterceptLine(startX, startY, startX - 12, startY) && isComputerPiece(startX - 12, startY) && !is_computer_checked() && !is_c_w_played)
			{
				add_child(root, startX, startY, startX - 12, startY);
			}
		}
			
		// diğer harfler (mesela boşsa)
		else
		{
			itworks(8);
			continue;
		}
		
	}
}

void computer_movements(MoveTreeNode* head_of_tree)
{
	int startX;
	int startY;
	int finishX;
	int finishY;
	
	MoveTreeNode* root = head_of_tree;
	
	// if(root->children != NULL)
	// {
		// for(int i = 0; i < root->children_count; i++)
		// {
			// player_movements(root->children[i]);
		// }
	// }
	// else
	// {
	
	// hareket olasılıklarını ağaca ekleme (rakip taşlarını yeme olasılıkları eklenecek)
	for(int i = 0; i < 16; i++)
	{
		startX = convert_X_to_board_index(computer_piece_coordinates[i][0]);
		startY = convert_Y_to_board_index(computer_piece_coordinates[i][1]);
		itworks(1);
		
		//piyonların hareket olasılıkları
		if(computer_piece_coordinates[i][2] == 'p')
		{
			if(map[startY + 4][startX] == ' ' && map[startY + 2][startX] == ' ' && startY == 3)
			{
				itworks(2);
				add_child(root, startX, startY, startX, startY + 4);
				calculate_score(root, startX, startY, startX, startY + 4);
			}
			if(map[startY + 2][startX] == ' ')
			{
				itworks(12);
				add_child(root, startX, startY, startX, startY + 2);
				calculate_score(root, startX, startY, startX, startY + 2);
			}
			
			// rakip piyonu yeme olasılıklarından: 
			if(map[startY + 2][startX + 3] != ' ' && startY + 2 < 16 && startX + 3 < 25 && !isComputerPiece(startX + 3, startY + 2))
			{
				add_child(root, startX, startY, startX + 3, startY + 2);
				calculate_score(root, startX, startY, startX + 3, startY + 2);
				
			}
			if(map[startY + 2][startX - 3] != ' ' && startY + 2 < 16 && startX - 3 > 2 && !isComputerPiece(startX - 3, startY + 2))
			{
				add_child(root, startX, startY, startX - 3, startY + 2);
				calculate_score(root, startX, startY, startX - 3, startY + 2);
				
			}
		}
		
		// kalelerin hareket olasılıkları
		else if(computer_piece_coordinates[i][2] == 'r')
		{
			itworks(3);
			// x eksenindeki hareketleri olasılık ağacına ekleme
			for(int i = startX + 3; i < 25; i += 3)
			{
				if(!isInterceptLine(startX, startY, i, startY) && i != startX && !isComputerPiece(i, startY)) // i != startX -> kısmına gerek kalmamış olabilir
				{
					itworks(16);
					add_child(root, startX, startY, i, startY);
					calculate_score(root, startX, startY, i, startY);
				}
				else
				{
					itworks(26);
					break;
				}
			}
			for(int i = startX - 3; i > 2; i -= 3)
			{
				if(!isInterceptLine(startX, startY, i, startY) && i != startX && !isComputerPiece(i, startY))
				{
					itworks(36);
					add_child(root, startX, startY, i, startY);
					calculate_score(root, startX, startY, i, startY);
				}
				else
				{
					itworks(46);
					break;
				}
			}
			
			// y eksenindeki hareketleri olasılık ağacına ekleme
			for(int i = startY + 2; i < 16; i += 2)
			{
				if(!isInterceptLine(startX, startY, startX, i) && i != startY && !isComputerPiece(startX, i))
				{
					itworks(56);
					add_child(root, startX, startY, startX, i);
					calculate_score(root, startX, startY, startX, i);
				}
				else
				{
					itworks(66);
					break;
				}
			}
			for(int i = startY - 2; i > 0; i -= 2)
			{
				if(!isInterceptLine(startX, startY, startX, i) && i != startY && !isComputerPiece(startX, i))
				{
					add_child(root, startX, startY, startX, i);
					calculate_score(root, startX, startY, startX, i);
				}
				else
				{
					break;
				}
			}
		}
		
		// atların hareket olasılıkları
		else if(computer_piece_coordinates[i][2] == 'k')
		{
			itworks(4);
			if(startX + 3 < 25 && startY + 4 < 16 && !isComputerPiece(startX + 3, startY + 4))
			{
				add_child(root, startX, startY, startX + 3, startY + 4);
				calculate_score(root, startX, startY, startX + 3, startY + 4);
			}
			if(startX - 3 > 2 && startY + 4 < 16 && !isComputerPiece(startX - 3, startY + 4))
			{
				add_child(root, startX, startY, startX - 3, startY + 4);
				calculate_score(root, startX, startY, startX - 3, startY + 4);
			}
			if(startX + 3 < 25 && startY - 4 > 0 && !isComputerPiece(startX + 3, startY - 4))
			{
				add_child(root, startX, startY, startX + 3, startY - 4);
				calculate_score(root, startX, startY, startX + 3, startY - 4);
			}
			if(startX - 3 > 2 && startY - 4 > 0 && !isComputerPiece(startX - 3, startY - 4))
			{
				add_child(root, startX, startY, startX - 3, startY - 4);
				calculate_score(root, startX, startY, startX - 3, startY - 4);
			}
			
			if(startX + 6 < 25 && startY + 2 < 16 && !isComputerPiece(startX + 6, startY + 2))
			{
				add_child(root, startX, startY, startX + 6, startY + 2);
				calculate_score(root, startX, startY, startX + 6, startY + 2);
			}
			if(startX - 6 > 2 && startY + 2 < 16 && !isComputerPiece(startX - 6, startY + 2))
			{
				add_child(root, startX, startY, startX - 6, startY + 2);
				calculate_score(root, startX, startY, startX - 6, startY + 2);
			}
			if(startX + 6 < 25 && startY - 2 < 16 && !isComputerPiece(startX + 6, startY + 2))
			{
				add_child(root, startX, startY, startX + 6, startY + 2);
				calculate_score(root, startX, startY, startX + 6, startY + 2);
			}
			if(startX - 6 > 2 && startY - 2 < 16 && !isComputerPiece(startX - 6, startY + 2))
			{
				add_child(root, startX, startY, startX - 6, startY + 2);
				calculate_score(root, startX, startY, startX - 6, startY + 2);
			}
		}
		
		// fillerin hareket olasılıkları
		else if(computer_piece_coordinates[i][2] == 'b')
		{
			itworks(5);
			// sağ aşağı
			for(int i = startX + 3, j = startY + 2; i < 25 && j < 16; i += 3, j += 2)
			{
				if(!isInterceptCross(startX, startY, i, j) && i != startX && !isComputerPiece(i, j))
				{
					add_child(root, startX, startY, i, j);
					calculate_score(root, startX, startY, i, j);
				}
				else
				{
					break;
				}
			}
			// sağ yukarı
			for(int i = startX + 3, j = startY - 2; i < 25 && j > 0; i += 3, j -= 2)
			{
				if(!isInterceptCross(startX, startY, i, j) && i != startX && !isComputerPiece(i, j))
				{
					add_child(root, startX, startY, i, j);
					calculate_score(root, startX, startY, i, j);
				}
				else
				{
					break;
				}
			}
			// sol aşağı
			for(int i = startX - 3, j = startY + 2; i > 2 && j < 16; i -= 3, j += 2)
			{
				itworks(35);
				if(!isInterceptCross(startX, startY, i, j) && i != startX && !isComputerPiece(i, j))
				{
					add_child(root, startX, startY, i, j);
					calculate_score(root, startX, startY, i, j);
				}
				else
				{
					break;					
				}
			}
			// sol yukarı
			for(int i = startX - 3, j = startY - 2; i > 2 && j > 0; i -= 3, j -= 2)
			{
				itworks(45);
				if(!isInterceptCross(startX, startY, i, j) && i != startX && !isComputerPiece(i, j))
				{
					add_child(root, startX, startY, i, j);
					calculate_score(root, startX, startY, i, j);
				}
				else
				{
					break;
				}
			}
		}
		
		// vezirin hareket olasılıkları
		else if(computer_piece_coordinates[i][2] == 'Q')
		{
			itworks(6);
			// kale ile aynı olasılıklar
			// x eksenindeki hareketleri olasılık ağacına ekleme
			for(int i = startX + 3; i < 25; i += 3)
			{
				if(!isInterceptLine(startX, startY, i, startY) && i != startX && !isComputerPiece(i, startY)) // i != startX -> kısmına gerek kalmamış olabilir
				{
					itworks(16);
					add_child(root, startX, startY, i, startY);
					calculate_score(root, startX, startY, i, startY);
				}
				else
				{
					itworks(26);
					break;
				}
			}
			for(int i = startX - 3; i > 2; i -= 3)
			{
				if(!isInterceptLine(startX, startY, i, startY) && i != startX && !isComputerPiece(i, startY))
				{
					itworks(36);
					add_child(root, startX, startY, i, startY);
					calculate_score(root, startX, startY, i, startY);
				}
				else
				{
					itworks(46);
					break;
				}
			}
			
			// y eksenindeki hareketleri olasılık ağacına ekleme
			for(int i = startY + 2; i < 16; i += 2)
			{
				if(!isInterceptLine(startX, startY, startX, i) && i != startY && !isComputerPiece(startX, i))
				{
					itworks(56);
					add_child(root, startX, startY, startX, i);
					calculate_score(root, startX, startY, startX, i);
				}
				else
				{
					itworks(66);
					break;
				}
			}
			for(int i = startY - 2; i > 0; i -= 2)
			{
				if(!isInterceptLine(startX, startY, startX, i) && i != startY && !isComputerPiece(startX, i))
				{
					add_child(root, startX, startY, startX, i);
					calculate_score(root, startX, startY, startX, i);
				}
				else
				{
					break;
				}
			}
			
			
			// fil ile aynı olasılıklar
			// sağ aşağı
			for(int i = startX + 3, j = startY + 2; i < 25 && j < 16; i += 3, j += 2)
			{
				if(!isInterceptCross(startX, startY, i, j) && i != startX && !isComputerPiece(i, j))
				{
					add_child(root, startX, startY, i, j);
					calculate_score(root, startX, startY, i, j);
				}
				else
				{
					break;
				}
			}
			// sağ yukarı
			for(int i = startX + 3, j = startY - 2; i < 25 && j > 0; i += 3, j -= 2)
			{
				if(!isInterceptCross(startX, startY, i, j) && i != startX && !isComputerPiece(i, j))
				{
					add_child(root, startX, startY, i, j);
					calculate_score(root, startX, startY, i, j);
				}
				else
				{
					break;
				}
			}
			// sol aşağı
			for(int i = startX - 3, j = startY + 2; i > 2 && j < 16; i -= 3, j += 2)
			{
				itworks(35);
				if(!isInterceptCross(startX, startY, i, j) && i != startX && !isComputerPiece(i, j))
				{
					add_child(root, startX, startY, i, j);
					calculate_score(root, startX, startY, i, j);
				}
				else
				{
					break;					
				}
			}
			// sol yukarı
			for(int i = startX - 3, j = startY - 2; i > 2 && j > 0; i -= 3, j -= 2)
			{
				itworks(45);
				if(!isInterceptCross(startX, startY, i, j) && i != startX && !isComputerPiece(i, j))
				{
					add_child(root, startX, startY, i, j);
					calculate_score(root, startX, startY, i, j);
				}
				else
				{
					break;
				}
			}
		}
		
		// şahın hareket olasılıkları
		else if(computer_piece_coordinates[i][2] == 'W')
		{
			itworks(7);
			// "şah" olma durumları da yazılmalı
			// sağ
			if(startX + 3 < 25 && !isComputerPiece(startX + 3, startY))
			{
				add_child(root, startX, startY, startX + 3, startY);
				calculate_score(root, startX, startY, startX + 3, startY);
			}
			// sol
			else if(startX - 3 > 2 && !isComputerPiece(startX - 3, startY))
			{
				add_child(root, startX, startY, startX - 3, startY);
				calculate_score(root, startX, startY, startX - 3, startY);
			}
			// aşağı
			else if(startY + 2 < 16 && !isComputerPiece(startX, startY + 2))
			{
				add_child(root, startX, startY, startX, startY + 2);
				calculate_score(root, startX, startY, startX, startY + 2);
			}
			// yukarı
			else if(startY - 2 > 0 && !isComputerPiece(startX, startY - 2))
			{
				add_child(root, startX, startY, startX, startY - 2);
				calculate_score(root, startX, startY, startX, startY - 2);
			}
			// sağ aşağı
			else if(startX + 3 < 25 && startY + 2 < 16 && !isComputerPiece(startX + 3, startY + 2))
			{
				add_child(root, startX, startY, startX + 3, startY + 2);
				calculate_score(root, startX, startY, startX + 3, startY + 2);
			}
			// sağ yukarı
			else if(startX + 3 < 25 && startY - 2 > 0 && !isComputerPiece(startX + 3, startY - 2))
			{
				add_child(root, startX, startY, startX + 3, startY - 2);
				calculate_score(root, startX, startY, startX + 3, startY - 2);
			}
			// sol aşağı
			else if(startX - 3 > 2 && startY + 2 < 16 && !isComputerPiece(startX - 3, startY + 2))
			{
				add_child(root, startX, startY, startX - 3, startY + 2);
				calculate_score(root, startX, startY, startX - 3, startY + 2);
			}
			// sol yukarı
			else if(startX - 3 > 2 && startY - 2 > 0 && !isComputerPiece(startX - 3, startY - 2))
			{
				add_child(root, startX, startY, startX - 3, startY - 2);
				calculate_score(root, startX, startY, startX - 3, startY - 2);
			}
			// sağdan rok atma
			else if(map[startY][startX + 9] == 'r' && !isInterceptLine(startX, startY, startX + 9, startY) && isComputerPiece(startX + 9, startY) && !is_computer_checked() && !is_c_w_played)
			{
				add_child(root, startX, startY, startX + 9, startY);
				calculate_score(root, startX, startY, startX + 9, startY);
			}
			// soldan rok atma
			else if(map[startY][startX - 12] == 'r' && !isInterceptLine(startX, startY, startX - 12, startY) && isComputerPiece(startX - 12, startY) && !is_computer_checked() && !is_c_w_played)
			{
				add_child(root, startX, startY, startX - 12, startY);
				calculate_score(root, startX, startY, startX - 12, startY);
			}
		}
		
		// diğer harfler (mesela boşsa)
		else
		{
			itworks(8);
			continue;
		}
		
	}
	// }
}

int can_player_move()
{
	player_movements(root);
	if(root->children_count == 0)
	{
		clear_root(&root);
		return 0;
	}
	else
	{
		clear_root(&root);
		return 1;
	}
}

int can_computer_move()
{
	MoveTreeNode* temp_root = createMoveTreeNode(root->startX, root->startY, root->finishX, root->finishY);
	computer_movements(temp_root);
	if(temp_root->children_count == 0)
	{
		clear_root(&temp_root);
		return 0;
	}
	else
	{
		clear_root(&temp_root);
		return 1;
	}
}

void recursive_p_moves(MoveTreeNode* head_of_tree)
{
	if(head_of_tree->children == NULL)
	{
		player_movements(head_of_tree);
	}
	else
	{
		for(int i = 0; i < head_of_tree->children_count; i++)
		{
			recursive_p_moves(head_of_tree->children[i]);
		}
	}
}

void recursive_c_moves(MoveTreeNode* head_of_tree)
{
	MoveTreeNode* iter = head_of_tree;
	if(iter->children == NULL)
	{
		computer_movements(iter);
	}
	else
	{
		for(int i = 0; i < iter->children_count; i++)
		{
			recursive_c_moves(iter->children[i]);
		}
	}
}


void game_computer()
{
	// bilgisayarın yapabileceği hamleleri bulma
	// gerekli derinliğe inme
	if(depth == 1)
	{
		computer_movements(root);		
	}
	else
	{
		computer_movements(root);
		for(int i = 0; i < depth - 1; i++)
		{
			recursive_p_moves(root);
			recursive_c_moves(root);
		}
	}
	
	int startX;
	int startY;
	int finishX;
	int finishY;
	
	char x;
	char y;


	// girilen derinlik değerine göre olasılık ağacının o derinlikteki elemanlarına ulaşma
	// int go_depth = 2 * (depth_counter) - 1;
	// if(depth != 1)
	// {
		// while(go_depth > 0)
		// {
			// iter = iter->children[0];
			// depth_counter--;
		// }
	// }
	
	
	// en uygun hareketi bulma
	
	
	// en uygun hareketi oynama
	
	// oynanan hareket rok ise
	startX = poss_move.startX;
	startY = poss_move.startY;
	finishX = poss_move.finishX;
	finishY = poss_move.finishY;
	if(map[startY][startX] == 'W' && map[finishY][finishX] == 'r' && !is_c_w_played && isComputerPiece(startY, startX) && isComputerPiece(finishX, finishY)) // koşullar verimli hale getirilebilir
	{
		if(startX + 9 == finishX)
		{
			// şahın konumunu güncelleme
			map[startY][startX] = ' ';
			map[finishY][finishX - 3] = 'W';
			char W_new_place[2];
			W_new_place[0] = convert_board_index_to_X(finishX - 3);
			W_new_place[1] = convert_board_index_to_Y(finishY);
			char W_first_place[2];
			W_first_place[0] = convert_board_index_to_X(startX);
			W_first_place[1] = convert_board_index_to_Y(startY);
			change_computer_piece_coordinates(W_first_place, W_new_place);
			is_c_w_played = 1;
			
			// kalenin konumunu güncelleme
			map[finishY][startX + 3] = map[finishY][finishX];
			map[finishY][finishX] = ' ';
			char r_new_place[2];
			r_new_place[0] = convert_board_index_to_X(finishX - 6);
			r_new_place[1] = convert_board_index_to_Y(finishY);
			char r_first_place[2];
			r_first_place[0] = convert_board_index_to_X(finishX);
			r_first_place[1] = convert_board_index_to_Y(finishY);
			change_computer_piece_coordinates(r_first_place, r_new_place);
		}
		else if(startX - 12 == finishX)
		{
			// şahın konumunu güncelleme
			map[startY][startX] = ' ';
			map[finishY][finishX + 6] = 'W';
			char W_new_place[2];
			W_new_place[0] = convert_board_index_to_X(finishX + 6);
			W_new_place[1] = convert_board_index_to_Y(finishY);
			char W_first_place[2];
			W_first_place[0] = convert_board_index_to_X(startX);
			W_first_place[1] = convert_board_index_to_Y(startY);
			change_computer_piece_coordinates(W_first_place, W_new_place);
			is_c_w_played = 1;
			
			// kalenin konumunu güncelleme
			map[finishY][startX - 3] = map[finishY][finishX];
			map[finishY][finishX] = ' ';
			char r_new_place[2];
			r_new_place[0] = convert_board_index_to_X(startX - 3);
			r_new_place[1] = convert_board_index_to_Y(finishY);
			char r_first_place[2];
			r_first_place[0] = convert_board_index_to_X(finishX);
			r_first_place[1] = convert_board_index_to_Y(finishY);
			change_computer_piece_coordinates(r_first_place, r_new_place);
		}
	}
	else
	{
		map[finishY][finishX] = map[startY][startX];
		map[startY][startX] = ' ';
		// bilgisayarın taşının konumunu güncelleme
		for(int i = 0; i < 16; i++)
		{
			if(computer_piece_coordinates[i][0] == convert_board_index_to_X(startX) && computer_piece_coordinates[i][1] == convert_board_index_to_Y(startY))
			{
				computer_piece_coordinates[i][0] = convert_board_index_to_X(finishX);
				computer_piece_coordinates[i][1] = convert_board_index_to_Y(finishY);
				
				if(isPlayerPiece(finishX, finishY))
				{
					x = convert_board_index_to_X(finishX);
					y = convert_board_index_to_Y(finishY);
					//  oyuncunun taşını yeme
					for(int i = 0; i < 16; i++)
					{
						if(player_piece_coordinates[i][0] == x && player_piece_coordinates[i][1] == y)
						{
							player_piece_coordinates[i][2] = ' ';
							map[finishY][finishX + 1] = ' ';
						}
					}
				}
				
			}
		}		
	}
	
	// hareket olasılıklarını tutan ağacı sıfırlamak
	clear_root(&root);
}


char sign;
void game_player()
{
	char start[2];
	char finish[2];
	printf("\nEnter the coordinate of the piece you want to play: ");
	scanf("%s", start);
	printf("\nEnter the coordinate where you want to play there: ");
	scanf("%s", finish);
	
	// a -> 97 -> 98 | 3 -> 6
	// 1 -> 49 -> 50 | 15 -> 13
	int startX = convert_X_to_board_index(start[0]);
	int startY = convert_Y_to_board_index(start[1]);
	
	int finishX = convert_X_to_board_index(finish[0]);
	int finishY = convert_Y_to_board_index(finish[1]);
	
	char sx = convert_board_index_to_X(startX);
	char sy = convert_board_index_to_Y(startY);
	
	char fx = convert_board_index_to_X(finishX);
	char fy = convert_board_index_to_Y(finishY);
	
	char piece;
	
	root = createMoveTreeNode(startX, startY, finishX, finishY);
	
	// taşın oyuncunun olup olmadığını kontrol etme alternatifi
	// for(int i = 0; i < 16; i++)
	// {
		// if(player_piece_coordinates[i][0] == sx && player_piece_coordinates[i][1] == sy)
		// {
			// piece = player_piece_coordinates[i][2];
			// break;
		// }
	// }
	
	// piyonun hareketleri
	if(map[startY][startX] == 'p' && isPlayerPiece(startX, startY))
	{
		if(startY - 2 == finishY && startX == finishX && map[startY - 2][startX] == ' ')
		{
			map[startY][startX] = ' ';
			map[startY][startX+1] = ' ';
			map[startY - 2][startX] = 'p';
			map[finishY][finishX+1] = sign;
			change_player_piece_coordinates(start, finish);
		}
		// ilk hamlede iki kare oynayabilme
		else if(startY == 13 && finishY == 9 && map[startY - 2][startX] == ' ')
		{
			map[startY][startX] = ' ';
			map[startY][startX+1] = ' ';
			map[finishY][finishX] = 'p';
			map[finishY][finishX+1] = sign;
			change_player_piece_coordinates(start, finish);
		}
		// finishX != 2 değişebilir, bilgisayar taşını yeme
		else if(finishX != 2 && startX - 3 == finishX && startY - 2 == finishY && map[finishY][finishX] != ' ') // map[finishY][finishX] != ' ' değişmeli
		{
			map[startY][startX] = ' ';
			map[startY][startX+1] = ' ';
			map[finishY][finishX] = 'p';
			map[finishY][finishX+1] = sign;
			change_player_piece_coordinates(start, finish);
			
			//  bilgisayar taşını yeme
			for(int i = 0; i < 16; i++)
			{
				if(computer_piece_coordinates[i][0] == fx && computer_piece_coordinates[i][1] == fy)
				{
					computer_piece_coordinates[i][2] = ' ';
				}
			}
		}
		// finishX != 18 değişebilir, bilgisayar taşını yeme
		else if(finishX != 18 && startX + 3 == finishX && startY - 2 == finishY && map[finishY][finishX] != ' ')
		{
			map[startY][startX] = ' ';
			map[startY][startX+1] = ' ';
			map[finishY][finishX] = 'p';
			map[finishY][finishX+1] = sign;
			change_player_piece_coordinates(start, finish);
			
			// bilgisayar taşını yeme
			for(int i = 0; i < 16; i++)
			{
				if(computer_piece_coordinates[i][0] == fx && computer_piece_coordinates[i][1] == fy)
				{
					computer_piece_coordinates[i][2] = ' ';
				}
			}
		}

	}

	// kalenin hareketleri
	// kalenin hareketlerini tekrar kontrol et
	else if(map[startY][startX] == 'r' && isPlayerPiece(startX, startY))
	{
		if(finishY > 0 && finishY < 16  && (startY + finishY) % 2 == 0 && map[finishY][finishX] == ' ' && finishX > 2 && finishX < 26 )
		{
			int intercept = isInterceptLine(startX, startY, finishX, finishY);
			if(!intercept)
			{
				map[startY][startX] = ' ';
				map[startY][startX+1] = ' ';
				map[finishY][finishX] = 'r';
				map[finishY][finishX+1] = sign;
				change_player_piece_coordinates(start, finish);
			}
		}
		
		// bilgisayar taşını yeme
		else if(finishY > 0 && finishY < 16  && (startY + finishY) % 2 == 0 && !isPlayerPiece(finishX, finishY) && finishX > 2 && finishX < 26)
		{
			int intercept = isInterceptLine(startX, startY, finishX, finishY);
			if(!intercept)
			{
				map[startY][startX] = ' ';
				map[startY][startX+1] = ' ';
				map[finishY][finishX] = 'r';
				map[finishY][finishX+1] = sign;
				change_player_piece_coordinates(start, finish);
			}
			
			//  bilgisayar taşını yeme
			for(int i = 0; i < 16; i++)
			{
				if(computer_piece_coordinates[i][0] == fx && computer_piece_coordinates[i][1] == fy)
				{
					computer_piece_coordinates[i][2] = ' ';
				}
			}
		}
		
		
	}
	
	// atın hareketleri
	else if(map[startY][startX] == 'k' && isPlayerPiece(startX, startY))
	{
		if( finishX > 2 && finishX < 25 && finishY> 0 && finishY < 16 && (((finishX == startX - 3 || finishX == startX + 3) && (finishY == startY - 4 || finishY == startY + 4)) || ((finishX == startX - 6 || finishX == startX + 6) && (finishY == startY - 2 || finishY == startY + 2))) && map[finishY][finishX] == ' ')
		{
			map[startY][startX] = ' ';
			map[startY][startX+1] = ' ';
			map[finishY][finishX] = 'k';
			map[finishY][finishX+1] = sign;
			change_player_piece_coordinates(start, finish);
		}
		
		// bilgisayarın taşını yeme
		else if(finishX > 2 && finishX < 25 && finishY > 0 && finishY < 16 && (((finishX == startX - 3 || finishX == startX + 3) && (finishY == startY - 4 || finishY == startY + 4)) || ((finishX == startX - 6 || finishX == startX + 6) && (finishY == startY - 2 || finishY == startY + 2))) && !isPlayerPiece(finishX, finishY))
		{
			map[startY][startX] = ' ';
			map[startY][startX+1] = ' ';
			map[finishY][finishX] = 'k';
			map[finishY][finishX+1] = sign;
			change_player_piece_coordinates(start, finish);
			
			//  bilgisayar taşını yeme
			for(int i = 0; i < 16; i++)
			{
				if(computer_piece_coordinates[i][0] == fx && computer_piece_coordinates[i][1] == fy)
				{
					computer_piece_coordinates[i][2] = ' ';
				}
			}
		}
	}
	
	// filin hareketleri
	else if(map[startY][startX] == 'b' && isPlayerPiece(startX, startY))
	{
		if(finishX > 2 && finishX < 25 && finishY > 0 && finishY < 16 && (startX + finishX) % 3 == 0 && (startY + finishY) % 2 == 0 && startX != finishX  && startY != finishY && (float)(abs(startX - finishX) / 3) / (abs(startY - finishY) / 2) == 1 && map[finishY][finishX] == ' ')
		{
			int intercept = isInterceptCross(startX, startY, finishX, finishY);
			if(!intercept)
			{
				map[startY][startX] = ' ';
				map[startY][startX+1] = ' ';
				map[finishY][finishX] = 'b';
				map[finishY][finishX+1] = sign;
				change_player_piece_coordinates(start, finish);
			}
		}
		// bilgisayarın taşını yeme
		else if(finishX > 2 && finishX < 25 && finishY > 0 && finishY < 16 && (startX + finishX) % 3 == 0 && (startY + finishY) % 2 == 0 && startX != finishX  && startY != finishY && (float)(abs(startX - finishX) / 3) / (abs(startY - finishY) / 2) == 1 && !isPlayerPiece(finishX, finishY))
		{
			int intercept = isInterceptCross(startX, startY, finishX, finishY);
			if(!intercept)
			{
				map[startY][startX] = ' ';
				map[startY][startX+1] = ' ';
				map[finishY][finishX] = 'b';
				map[finishY][finishX+1] = sign;
				change_player_piece_coordinates(start, finish);
			}
			//  bilgisayar taşını yeme
			for(int i = 0; i < 16; i++)
			{
				if(computer_piece_coordinates[i][0] == fx && computer_piece_coordinates[i][1] == fy) // çalışmazsa vezirdeki gibi yap
				{
					computer_piece_coordinates[i][2] = ' ';
				}
			}
		}
		
	}
	
	
	// vezirin hareketleri
	else if(map[startY][startX] == 'Q' && isPlayerPiece(startX, startY))
	{
		if(finishX > 2 && finishX < 25 && finishY > 0 && finishY < 16 && (startX + finishX) % 3 == 0 && (startY + finishY) % 2 == 0 && map[finishY][finishX] == ' ')
		{
			int interceptLine;
			int interceptCross;
			if(startX == finishX || startY == finishY)
			{
				interceptLine = isInterceptLine(startX, startY, finishX, finishY);
				if(!interceptLine)
				{
					map[startY][startX] = ' ';
					map[startY][startX+1] = ' ';
					map[finishY][finishX] = 'Q';
					map[finishY][finishX+1] = sign;
					change_player_piece_coordinates(start, finish);
				}
			}
			else
			{
				interceptCross = isInterceptCross(startX, startY, finishX, finishY);
				if(!interceptCross && (float)(abs(startX - finishX) / 3) / (abs(startY - finishY) / 2) == 1)
				{
					map[startY][startX] = ' ';
					map[startY][startX+1] = ' ';
					map[finishY][finishX] = 'Q';
					map[finishY][finishX+1] = sign;
					change_player_piece_coordinates(start, finish);
				}
			}
		}
		
		// bilgisayarın taşını yeme
		else if(finishX > 2 && finishX < 25 && finishY > 0 && finishY < 16 && (startX + finishX) % 3 == 0 && (startY + finishY) % 2 == 0 && !isPlayerPiece(finishX, finishY))
		{
			int interceptLine;
			int interceptCross;
			if(startX == finishX || startY == finishY)
			{
				interceptLine = isInterceptLine(startX, startY, finishX, finishY);
				if(!interceptLine)
				{
					map[startY][startX] = ' ';
					map[startY][startX+1] = ' ';
					map[finishY][finishX] = 'Q';
					map[finishY][finishX+1] = sign;
					change_player_piece_coordinates(start, finish);
				}
				
				//  bilgisayar taşını yeme
				for(int i = 0; i < 16; i++)
				{
					if(convert_X_to_board_index(computer_piece_coordinates[i][0]) == finishX && convert_Y_to_board_index(computer_piece_coordinates[i][1]) == finishY)
					{
						computer_piece_coordinates[i][2] = ' ';
					}
				}
			}
			else
			{
				interceptCross = isInterceptCross(startX, startY, finishX, finishY);
				if(!interceptCross && (float)(abs(startX - finishX) / 3) / (abs(startY - finishY) / 2) == 1) // diğer float olması gereken yerleri de düzelt
				{
					map[startY][startX] = ' ';
					map[startY][startX+1] = ' ';
					map[finishY][finishX] = 'Q';
					map[finishY][finishX+1] = sign;
					change_player_piece_coordinates(start, finish);
				}
				
				//  bilgisayar taşını yeme
				for(int i = 0; i < 16; i++)
				{
					if(convert_X_to_board_index(computer_piece_coordinates[i][0]) == finishX && convert_Y_to_board_index(computer_piece_coordinates[i][1]) == finishY)
					{
						computer_piece_coordinates[i][2] = ' ';
					}
				}
			}
		}
	}
	
	// şahın hareketleri
	// şaha özel hareket koşulları eklenmeli
	// diğer piyonlara da açmaz ihtimali koşulu eklenmeli
	else if(map[startY][startX] == 'W' && isPlayerPiece(startX, startY))
	{
		if(finishX > 2 && finishX < 25 && finishY > 0 && finishY < 17 && (startX - 3 == finishX || startX + 3 == finishX || startX == finishX) && (startY - 2 == finishY || startY + 2 == finishY || startY == finishY) && map[finishY][finishX] == ' ')
		{
			map[startY][startX] = ' ';
			map[startY][startX+1] = ' ';
			map[finishY][finishX] = 'W';
			map[finishY][finishX+1] = sign;
			change_player_piece_coordinates(start, finish);
			is_p_w_played = 1;
		}
		
		// rok atma
		else if(map[finishY][finishX] == 'r' && !is_player_checked() && isPlayerPiece(finishX, finishY) && !is_p_w_played && !isInterceptLine(startX, startY, finishX, finishY))
		{
			if(startX + 9 == finishX)
			{
				// şahın konumunu güncelleme
				map[startY][startX] = ' ';
				map[startY][startX+1] = ' ';
				map[finishY][finishX - 3] = 'W';
				map[finishY][finishX - 3 + 1] = sign;
				char W_new_place[2];
				W_new_place[0] = convert_board_index_to_X(finishX - 3);
				W_new_place[1] = finish[1];
				change_player_piece_coordinates(start, W_new_place);
				is_p_w_played = 1;
				
				// kalenin konumunu güncelleme
				map[finishY][startX + 3] = map[finishY][finishX];
				map[finishY][startX + 4] = '.';
				map[finishY][finishX] = ' ';
				map[finishY][finishX+1] = ' ';
				char r_new_place[2];
				r_new_place[0] = convert_board_index_to_X(finishX - 6);
				r_new_place[1] = finish[1];
				change_player_piece_coordinates(finish, r_new_place);
			}
			else if(startX - 12 == finishX)
			{
				// şahın konumunu güncelleme
				map[startY][startX] = ' ';
				map[startY][startX+1] = ' ';
				map[finishY][finishX + 6] = 'W';
				map[finishY][finishX + 6 + 1] = sign;
				char W_new_place[2];
				W_new_place[0] = convert_board_index_to_X(finishX + 6);
				W_new_place[1] = finish[1];
				change_player_piece_coordinates(start, W_new_place);
				is_p_w_played = 1;
				
				// kalenin konumunu güncelleme
				map[finishY][startX - 3] = map[finishY][finishX];
				map[finishY][startX - 2] = '.';
				map[finishY][finishX] = ' ';
				map[finishY][finishX+1] = ' ';
				char r_new_place[2];
				r_new_place[0] = convert_board_index_to_X(startX - 3);
				r_new_place[1] = finish[1];
				change_player_piece_coordinates(finish, r_new_place);
			}
		}
		
		// bilgisayarın taşını yeme
		else if(finishX > 2 && finishX < 25 && finishY > 0 && finishY < 17 && (startX - 3 == finishX || startX + 3 == finishX || startX == finishX) && (startY - 2 == finishY || startY + 2 == finishY || startY == finishY) && isComputerPiece(finishX, finishY))
		{
			map[startY][startX] = ' ';
			map[startY][startX+1] = ' ';
			map[finishY][finishX] = 'W';
			map[finishY][finishX+1] = sign;
			change_player_piece_coordinates(start, finish);
			
			//  bilgisayar taşını yeme
			for(int i = 0; i < 16; i++)
			{
				if(computer_piece_coordinates[i][0] == fx && computer_piece_coordinates[i][1] == fy)
				{
					computer_piece_coordinates[i][2] = ' ';
				}
			}
		}
	}
	
	// diğer olasılıklar eklenecek
	
	poss_move.score = -100;
}


void print_computer_piece_coordinates() // hata ayıklama için kullanıldı
{
	for(int i = 0; i < 16; i++)
	{
		printf("%c, %c, %c,\n", computer_piece_coordinates[i][0], computer_piece_coordinates[i][1], computer_piece_coordinates[i][2]);
	}
}

void print_player_piece_coordinates() // hata ayıklama için kullanıldı
{
	for(int i = 0; i < 16; i++)
	{
		printf("%c, %c, %c,\n", player_piece_coordinates[i][0], player_piece_coordinates[i][1], player_piece_coordinates[i][2]);
	}
}


// yapılacaklar:
// şah çekme durumuna göre de gelecek hamleleri hesaplama
// açmaz x ve y ekseninde kale ve vezir ile, çaprazlarda ise fil ve veziri kontrol et
// oyunu kazanma  şah çekiliyse ve şah çekilmesi diğer taşlarla engellenemiyorsa ve şah başka yere hareket edemezse bitsin
// berabere kalma  şah çekili değilse ve kişi başka hamle yapamazsa oyun bitsin
// oyuncuya rok atma ihtimali ekleme + (tam emin değilim)
// bilgisayara rok atma ihtimali ekleme +
// rakip taşlarını yeme hamle olasılıklarını da ekleme +
// rakip taşlarını yeme +
// gereken derinlikte hamleler oluşturma onlara ulaşma + (ne kadar doğru olduğundan emin değilim)
// şah çekme +
// rok atma +
// rock sonrası taş konumlarını düzenleme +

int main()
{
	is_p_w_played = 0;
	is_c_w_played = 0; // bilgisayar için güncellenmeli
	
	printf("Chose a sign to represent your pieces: ");
	scanf("%c", &sign);
	printf("Choose a difficulty (1-5): ");
	scanf("%d", &depth);
	
	add_pieces(sign);
	while(1)
	{
		// system("cls");
		print_map();
		
		game_player();
		// system("cls");
		print_map();
		
		printf("To see computer's move press a button: ");
		getch();
		game_computer();
		print_computer_piece_coordinates();
		printf("\n");
		print_player_piece_coordinates();
		
	}
	return 0;
}