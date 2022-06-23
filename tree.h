#pragma once
#pragma once
#include "node.h"
#include <algorithm>
#include <queue>
using namespace std;

class tree : public node
{
private:
	node* root;
public:
	tree() { root = new node(); }
	char board[9] = { 0, };

	node* selection();
	node* expansion(node*);
	int brother(char*); //동생의 수
	int me(char*); //나의 수 
	bool win(char*); //승리하는 경우 
	bool simulation();
	void backpropagation(node*, bool);
	void clearboard() {
		for (int i = 0; i < 9; i++) //initialize board;
			board[i] = 0;
	}
	void MCTSwithUCT();
	bool playgame();
};

