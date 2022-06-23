#pragma once
#include <iostream>
#include <vector>
using namespace std;

class node
{
private:
	int win; //��带 �����ϴ� ��Ʈ�� ���� �¸��� Ƚ��
	int arrive; //��� ���� Ƚ��
	int index; //tictactoe board������ index
	int level; //node�� tree���� ���� level
	double per; //���� ��Ȳ���� �� ���� ������ Ȯ��

	node* parent; //�θ� ���
	vector<node*> childnodes; //�ڽ� ����

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
