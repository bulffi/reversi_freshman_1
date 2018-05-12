
#include <stdio.h>
#include<string.h>

#define SPACE 0
#define BLACK 1
#define WHITE 2
#define ACTUAL 10
#define IMAGINE 11
#define MAX_DEPTH 4
#define MMAX 20000
#define MMIN -20000
#define READ 20
#define RECORD 30

const int dir[8][2] = { { -1, 0 },{ -1, -1 },{ 0, -1 },{ 1, -1 },{ 1, 0 },{ 1, 1 },{ 0, 1 },{ -1, 1 } };
int chessmap[MAX_DEPTH + 1][16][16];
typedef struct {
	int x;
	int y;
	int side;
	int score;
} placement;
placement bestmove;

placement movelist[MAX_DEPTH + 1][80];
int move_num;
int my_side;
int other_side;
int step_count;
int value[16][16] = {
	{ 200,-8,10,5,5,5,5,5,5,5,5,5,5,10,-8,200 } ,
	{ -8,-100,1,1,1,1,1,1,1,1,1,1,1,1,-100,-8 }  ,
	{ 10,1,3,2,2,2,2,2,2,2,2,2,2,3,1,10 } ,
	{ 5,1,2,1,1,1,1,1,1,1,1,1,1,2,1,5 } ,
	{ 5,1,2,1,1,1,1,1,1,1,1,1,1,2,1,5 } ,
	{ 5,1,2,1,1,1,1,1,1,1,1,1,1,2,1,5 } ,
	{ 5,1,2,1,1,1,1,1,1,1,1,1,1,2,1,5 } ,
	{ 5,1,2,1,1,1,1,1,1,1,1,1,1,2,1,5 } ,
	{ 5,1,2,1,1,1,1,1,1,1,1,1,1,2,1,5 } ,
	{ 5,1,2,1,1,1,1,1,1,1,1,1,1,2,1,5 } ,
	{ 5,1,2,1,1,1,1,1,1,1,1,1,1,2,1,5 } ,
	{ 5,1,2,1,1,1,1,1,1,1,1,1,1,2,1,5 } ,
	{ 5,1,2,1,1,1,1,1,1,1,1,1,1,2,1,5 } ,
	{ 10,1,3,2,2,2,2,2,2,2,2,2,2,3,1,10 },
	{ -8,-100,1,1,1,1,1,1,1,1,1,1,1,1,-100,-8 },
	{ 200,-8,10,5,5,5,5,5,5,5,5,5,5,10,-8,200 },
};

void initialize(void);
_Bool judge_8_dir(int x, int y, int side, int depth);
_Bool judge_1_dir(int x, int y, int dir, int side, int depth);

void make_move(int type, int depth, int side, int number);
int alphabeta(int depth, int alpha, int beta);
int max(int a, int b);
int min(int a, int b);
int find_move(int choice, int depth, int side);

void add_list(int depth, int side, int x, int y);
int evaluate(int depth);
void submit(void);
void cleanup(void);
void turnover(int depth, int side, int x, int y);

_Bool gameover(int depth);
_Bool cohesion(int x, int y, int depth);
int main(void)
{
	char commond[5];
	int j;
	step_count = -1;
	scanf("%s", commond);
	while (commond[0] != 'E')
	{
		step_count++;
		switch (commond[0])
		{
		case('S') :
			initialize();
			break;
		case('P') :
			make_move(ACTUAL, MAX_DEPTH, other_side, 0);
			alphabeta(MAX_DEPTH, MMIN, MMAX);
			break;
		case('T') :
			submit();
			make_move(ACTUAL, MAX_DEPTH, my_side, 0);
			cleanup();
			j = find_move(READ, MAX_DEPTH, other_side);
			if (j == 0)
			{
				alphabeta(MAX_DEPTH, MMIN, MMAX);
				cleanup();
			}
			else
			{
				move_num = 0;
				cleanup();
				break;
			}
		}
		scanf("%s", commond);
	}
	return 0;
}

void initialize(void)
{
	chessmap[MAX_DEPTH][8][7] = chessmap[MAX_DEPTH][7][8] = BLACK;
	chessmap[MAX_DEPTH][7][7] = chessmap[MAX_DEPTH][8][8] = WHITE;
	scanf(" %d", &my_side);
	if (my_side == BLACK)
	{
		other_side = WHITE;
		bestmove.x = 8;
		bestmove.y = 9;
		bestmove.side = BLACK;
	}
	else
	{
		other_side = BLACK;
	}
	bestmove.score = MMIN;
	printf("OK\n");
	fflush(stdout);
}
int max(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}
int min(int a, int b)
{
	if (a < b)
		return a;
	else
		return b;
}

int alphabeta(int depth, int alpha, int beta)
{
	int side;
	int count;
	int i;

	if (depth == 0 || gameover(depth))
		return evaluate(depth);

	if ((MAX_DEPTH - depth) % 2 == 0)
		side = my_side;
	else
		side = other_side;

	if (side == my_side)
	{
		count = find_move(RECORD, depth, side);
		for (i = 0; i < count; i++)
		{
			make_move(IMAGINE, depth, side, i);
			alpha = max(alpha, alphabeta(depth - 1, alpha, beta));
			if (depth == MAX_DEPTH)
			{
				movelist[MAX_DEPTH][i].score = alpha;
				if (movelist[MAX_DEPTH][i].score > bestmove.score)
				{
					bestmove = movelist[MAX_DEPTH][i];
				}
			}
			if (beta <= alpha)
				break;
		}
		return alpha;
	}
	else
	{
		count = find_move(RECORD, depth, side);
		for (i = 0; i < count; i++)
		{
			make_move(IMAGINE, depth, side, i);
			beta = min(beta, alphabeta(depth - 1, alpha, beta));
			if (beta <= alpha)
				break;
		}
		return beta;
	}
}
void cleanup(void)
{
	bestmove.score = -50000;
	memset(movelist, 0, sizeof(movelist));
}
int evaluate(int depth)
{
	int i, j;
	int count_my = 0, count_other = 0;
	_Bool my_lose = 1;
	_Bool other_lose = 1;

	for (i = 0; i < 16; i++)
							
	{
		for (j = 0; j < 16; j++)
		{
			if (chessmap[depth][i][j] == my_side)
			{
				my_lose = 0;
				count_my += value[i][j];
				if (cohesion(i, j, depth))
					count_my -= 3;

			}
			else if (chessmap[depth][i][j] == other_side)
			{
				other_lose = 0;
				count_other += value[i][j];
				if (cohesion(i, j, depth))
					count_other -= 3;
			}
			/*else if(step_count<80)
			{
			if (judge_8_dir(i, j, my_side, depth))
			count_my += 3;
			if (judge_8_dir(i, j, other_side, depth))
			count_other += 3;
			}*/
		}
	}
	if (my_lose)
		return -20000;
	if (other_lose)
		return 20000;
	return count_my - count_other;
}
_Bool cohesion(int x, int y, int depth)
{
	int dir_chos, row_dir, col_dir;
	int count = 0;
	for (dir_chos = 0; dir_chos < 8; dir_chos++)
	{
		row_dir = dir[dir_chos][0];
		col_dir = dir[dir_chos][1];
		if (y + col_dir > -1 && y + col_dir < 16 &&
			x + row_dir > -1 && x + row_dir < 16)
		{
			if (chessmap[depth][x + row_dir][y + col_dir] == SPACE)
			{
				return 1;
			}
		}
	}
	return 0;
}
void submit(void)
{
	printf("%d %d\n", bestmove.x, bestmove.y);
	fflush(stdout);
}
void add_list(int depth, int side, int x, int y)
{
	movelist[depth][move_num].x = x;
	movelist[depth][move_num].y = y;
	movelist[depth][move_num].side = side;
	move_num++;
}

void make_move(int type, int depth, int side, int number)
{
	int x;
	int y;

	if (type == ACTUAL)
	{
		if (side == my_side)
		{
			x = bestmove.x;
			y = bestmove.y;
			side = bestmove.side;
			turnover(MAX_DEPTH, side, x, y);
		}
		else
		{
			scanf(" %d %d", &x, &y);
			turnover(MAX_DEPTH, side, x, y);
		}
	}
	else
	{
		memcpy(chessmap[depth - 1], chessmap[depth], sizeof(chessmap[depth]));
		x = movelist[depth][number].x;
		y = movelist[depth][number].y;
		turnover(depth - 1, side, x, y);
	}
}
int find_move(int choice, int depth, int side)
{
	move_num = 0;
	int mobility = 0;
	int i, j;
	for (i = 0; i < 16; i++)
	{
		for (j = 0; j < 16; j++)
		{
			if (chessmap[depth][i][j] == SPACE&&judge_8_dir(i, j, side, depth))
			{
				mobility++;
				if (choice == RECORD)
					add_list(depth, side, i, j);
				continue;
			}
		}
	}
	return mobility;
}
_Bool judge_1_dir(int x, int y, int dir_cho, int side, int depth)
{
	int opp_side;
	if (side == WHITE)
		opp_side = BLACK;
	else
		opp_side = WHITE;
	int cur_x = x + dir[dir_cho][0];
	int cur_y = y + dir[dir_cho][1];
	if (cur_x >= 0 && cur_x <= 15 && cur_y >= 0 && cur_y <= 15
		&& chessmap[depth][cur_x][cur_y] == opp_side)
	{
		while (chessmap[depth][cur_x][cur_y] == opp_side
			&& (cur_x + dir[dir_cho][0]) >= 0 && (cur_x + dir[dir_cho][0]) <= 15
			&& (cur_y + dir[dir_cho][1]) >= 0 && (cur_y + dir[dir_cho][1]) <= 15)
		{
			cur_x += dir[dir_cho][0];
			cur_y += dir[dir_cho][1];
		}
		return (chessmap[depth][cur_x][cur_y] == side) ? 1 : 0;
	}
	return 0;
}
_Bool judge_8_dir(int x, int y, int side, int depth)
{
	int i;
	for (i = 0; i<8; i++)
	{
		if (judge_1_dir(x, y, i, side, depth))
			return 1;
	}
	return 0;
}
void turnover(int depth, int side, int x, int y)
{
	int dir_chos;
	int opp_side;

	if (side == WHITE)
		opp_side = BLACK;
	else
		opp_side = WHITE;
	chessmap[depth][x][y] = side;
	int cur_x;
	int cur_y;
	for (dir_chos = 0; dir_chos < 8; dir_chos++)
	{
		if (judge_1_dir(x, y, dir_chos, side, depth))
													 
		{
			cur_x = x + dir[dir_chos][0];
			cur_y = y + dir[dir_chos][1];
			while (chessmap[depth][cur_x][cur_y] == opp_side
				&& (cur_x + dir[dir_chos][0]) >= 0 && (cur_x + dir[dir_chos][0]) <= 15
				&& (cur_y + dir[dir_chos][1]) >= 0 && (cur_y + dir[dir_chos][1]) <= 15)
			{
				chessmap[depth][cur_x][cur_y] = side;
				cur_x += dir[dir_chos][0];
				cur_y += dir[dir_chos][1];
			}
		}
	}
}
_Bool gameover(int depth)
{
	int i, j;
	_Bool flag = 1;
	for (i = 0; i < 16; i++)
	{
		for (j = 0; j < 16; j++)
		{
			if (chessmap[depth][i][j] == SPACE)
			{
				flag = 0;
			}
		}
	}
	return flag;
}