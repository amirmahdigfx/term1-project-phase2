#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <wingdi.h>
#include <unistd.h>
#include <ctype.h>

#define INP_BSIZE 10
#define MAX_T_SIZE 50
#define MAX_ROW 5
#define MAX_COL 10
#define MAX_FILE_PATH 250
#define INF_DIST 1000000
#define WALLC '#'
#define PACMANC '0'
#define NORMALC '1'
#define PRIZEC '*'
int has_st = 0;
int curdir;
int newdir = -1;
int curfr;
const int CYCLELENGTH = 10;
const int PACMANH = 50;
const int PACMANW = 50;
const int n = 5,m = 10;
const int totalSize = 50;
const int nCell[4] = {1,4,-1,-4};
const char comtochar[4] = {'r','d','l','u'};
const int dx[4] = {1,0,-1,0};
const int dy[4] = {0,1,0,-1};

struct {
	int pR,pC;
	int pX,pY;
	int curCycle;
	int state[MAX_ROW][MAX_COL];
	int comssize,curcom;
	int fruitnum;
} game;


int getnum(int i,int j) {return i * 10 + j;}
int getrow(int c){return c / m;}
int getcol(int c) {return c % m;}
int get_input(){
	static int cnt = 0;
	int fl = 0;
	if (GetAsyncKeyState(VK_LEFT)& (1 << 16)) newdir = 2,fl = 1;
	if (GetAsyncKeyState(VK_RIGHT) & (1 << 16)) newdir = 0,fl = 1;
	if (GetAsyncKeyState(VK_DOWN) & (1 << 16)) newdir = 1,fl = 1;
	if (GetAsyncKeyState(VK_UP) & (1 << 16)) newdir = 3,fl = 1;
	if (curfr == 0 && fl == 0) newdir = -1;
	if (fl) curfr = INP_BSIZE;
	else curfr = max(curfr - 1,0);
	return fl;
}

void preproc(){

	// reading the game table
	char fpath[MAX_FILE_PATH];
	gets(fpath);
	FILE *tfile = fopen(fpath,"r");
	char currow[MAX_COL + 10];
	for (int i = 0;i < n;i++){
		fgets(currow,10,tfile);
		for (int j = 0;j < m;j++){
			switch(currow[j]){
				case WALLC:
					game.state[i][j] = -1;
					break;
				case PRIZEC:
					game.state[i][j] = 1;
					game.fruitnum++;
					break;
				case NORMALC:
					game.state[i][j] = 0;
					break;
				case PACMANC:
					game.state[i][j] = 0;
					game.pR = i;
					game.pC = j;
					break;
			}
				//printf("%d ",game.state[i][j]);
		}
	}
	fclose(tfile);
	// end of input
}
int update(){
	//printf("%d\n",has_st);
	if (has_st == 0) return 0;
	game.pX += dx[curdir] * PACMANW / CYCLELENGTH;
	game.pY += dy[curdir] * PACMANH / CYCLELENGTH;
	if (game.curCycle % CYCLELENGTH == 0){
		if (game.state[game.pR][game.pC] == 1) game.state[game.pR][game.pC] = 0,game.fruitnum--;
		if (game.fruitnum == 0) return 1;
		if (newdir != -1) curdir = newdir;
		game.pR += dy[curdir];
		game.pC += dx[curdir];
		if (game.pR >= n || game.pR < 0 || game.pC < 0 || game.pC >= m || game.state[game.pR][game.pC] == -1 ) return -1;
		//printf("at %d %d\n",game.pR,game.pC );
		
	}
	game.curCycle++;
	return 0;
}

