#pragma once
#include <iostream>
#include <vector>
using namespace std;

class node
{
private:
	int win; //노드를 포함하는 루트를 통해 승리한 횟수
	int arrive; //노드 도달 횟수
	int index; //tictactoe board에서의 index
	int level; //node가 tree에서 속한 level
	double per; //이전 상황에서 이 수가 놓여질 확률

	node* parent; //부모 노드
	vector<node*> childnodes; //자식 노드들

public:
	node() { win = 0; arrive = 0; index = 0; level = -1; per = 0; parent = NULL; childnodes.clear(); }

	int getwin() { return win; }
	int getarrive() { return arrive; }
	int getindex() { return index; }
	int getlevel() { return level; }
	double getper() { return per; }
	node* getparent() { return parent; }
	vector<node*> getchildnodes() { return childnodes; }

	void addwin() { win++; }
	void addarrive() { arrive++; }
	void setindex(int i) { index = i; }
	void setlevel(int l) { level = l; }
	void setper(double p) { per = p; }
	void setparent(node* node) { parent = node; }
	void setchildnodes(vector<node*> nodes) { childnodes = nodes; }

};
