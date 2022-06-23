#include "tree.cpp"

#include <iostream>
#include <ctime>
using namespace std;

int main(void)
{
	srand((size_t)time(NULL));

	tree* MCTS = new tree();
	int treecount; //tree ������ ���� simulation�� ������ Ƚ��
	cout << "simulation�� ������ Ƚ�� : ";
	cin >> treecount;

	for (int i = 0; i < treecount; i++)
	{
		MCTS->MCTSwithUCT(); //simulation
		cout << i + 1 << endl;
	}
	
	cout << "simulation �Ϸ�" << endl << endl;

	int gamecount;
	cout << "Game�� ������ Ƚ��(100��ȸ �̻�) : ";
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
	cout << "���� : " << totalgame << " �¸� : " << wingame << " �·� : " << winrate << endl;

	return 0;
}
