#include "tree.cpp"

#include <iostream>
#include <ctime>
using namespace std;

int main(void)
{
	srand((size_t)time(NULL));

	tree* MCTS = new tree();
	int treecount; //tree 구성을 위해 simulation을 진행할 횟수
	cout << "simulation을 진행할 횟수 : ";
	cin >> treecount;

	for (int i = 0; i < treecount; i++)
	{
		MCTS->MCTSwithUCT(); //simulation
		cout << i + 1 << endl;
	}
	
	cout << "simulation 완료" << endl << endl;

	int gamecount;
	cout << "Game을 진행할 횟수(100만회 이상) : ";
	cin >> gamecount;

	int totalgame = 0, wingame = 0;
	for (int i = 0; i < gamecount; i++)
	{
		totalgame++;
		bool result = MCTS->playgame();
		cout << i << " : " << result << endl;
		if (result == true)
			wingame++;
	}
	cout << endl;
	double winrate = (double)wingame / totalgame;
	cout << "게임 : " << totalgame << " 승리 : " << wingame << " 승률 : " << winrate << endl;

	return 0;
}
