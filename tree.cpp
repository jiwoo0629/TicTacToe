#include "tree.h"

inline node* tree::selection() //��Ȳ�� ���� simulation�� �߻��� node ã�� (Pure Monte Carlo tree search ���)
{
	node* cur = root; //current node
	queue<node*> q; //level order�� ���� ť
	int level = 0; //�� �������� ���� �������� ����
	
	while (!cur->getchildnodes().empty()) //pure MCTS �˰��� ���� 
	{
		if (cur->getchildnodes().size() < 9 - level)
			break;		

		vector<node*> v = cur->getchildnodes();
		int check = 0;
		for (auto iter = v.begin(); iter != v.end(); iter++) {
			q.push(*iter);
		}

		level++;
		if(cur != root && (cur->getparent()->getlevel() == q.front()->getparent()->getlevel()) )
			level--;
		
		if (q.empty())
			break;
		cur = q.front();
		q.pop();
	}
	
	node* n = cur; 
	while (n->getparent()) //board�� Ʈ���� ��ġ�鼭 ���� ���� ��Ȳ ����
	{
		int num = n->getindex();
		int lev = n->getlevel();
		if (lev % 2 == 0)
			board[num] = 'o';
		else
			board[num] = 'x';
		n = n->getparent();
	}

	return cur;
}

inline node* tree::expansion(node* n) //n�� selection�� ���� ã�� node
{
	node* cur = n;
	int count = 0; //����� ���� Ƚ��
	while (cur->getparent())
	{
		count++;
		cur = cur->getparent();
	}

	vector<node*> childnode = n->getchildnodes();
	node* newnode = new node();
	int num;
	
	//childnode�� �߿� num�� index�� ���� node�� �ִ���, ���� �� �θ��� �߿� num�� index�� ���� node�� �ִ��� Ȯ��
	int check; 
	if ((9 - count) % 2 == 0) //���� ���� Ƚ���� ¦�� : ���� ����
	{
		do {
			check = 0;
			num = brother(board);
			for (auto iter = childnode.begin(); iter != childnode.end(); iter++)
			{
				if ((*iter)->getindex() == num) //���� ��� check
					check = 1;
			}
			node* cur = n;
			while (cur->getparent())
			{
				if (cur->getindex() == num)
					check = 1;
				cur = cur->getparent();
			}
		} while (check == 1);

		board[num] = 'x';
	}
	else //�� ����
	{
		do {
			check = 0;
			num = me(board);
			for (auto iter = childnode.begin(); iter != childnode.end(); iter++)
			{
				if ((*iter)->getindex() == num) //���� ��� check
					check = 1;
			}
			node* cur = n;
			while (cur->getparent())
			{
				if (cur->getindex() == num)
					check = 1;
				cur = cur->getparent();
			}
		} while (check == 1);
		board[num] = 'o';
	}
	newnode->setindex(num);
	newnode->setlevel(n->getlevel() + 1);

	childnode.push_back(newnode); //childnode�� node ����
	n->setchildnodes(childnode);
	newnode->setparent(n);

	return newnode;
}

inline int tree::brother(char* boardcop)
{
	bool vertex = false, edge = false, center = false;
	if (boardcop[0] == 0 || boardcop[2] == 0 || boardcop[6] == 0 || boardcop[8] == 0)
		vertex = true;
	if (boardcop[1] == 0 || boardcop[3] == 0 || boardcop[5] == 0 || boardcop[7] == 0)
		edge = true;
	if (boardcop[4] == 0)
		center = true;

	int num; //������ ��
	if (vertex == true && edge == true && center == true)
	{
		int n = rand() % 7;
		if (n < 4) //�Ϳ� ���� ���� ���
		{
			do {
				num = rand() % 5;
			} while (num == 2 && boardcop[2 * num] != 0);
			return 2 * num;
		}
		else if (n < 6) //���� ���� ���� ���
		{
			do {
				num = rand() % 4;
			} while (boardcop[2 * num + 1] != 0);
			return 2 * num + 1;
		}
		else
			return 4;

	}
	else if (vertex == true && edge == true)
	{
		int n = rand() % 3;
		if (n < 2) //�Ϳ� ���� ���� ���
		{
			do {
				num = rand() % 5;
			} while (num == 2 && boardcop[2 * num] != 0);
			return 2 * num;
		}
		else //���� ���� ���� ���
		{
			do {
				num = rand() % 4;
			} while (boardcop[2 * num + 1] != 0);
			return 2 * num + 1;
		}

	}
	else if (vertex == true && center == true)
	{
		int n = rand() % 5;
		if (n < 4) //�Ϳ� ���� ���� ���
		{
			do {
				num = rand() % 5;
			} while (num == 2 && boardcop[2 * num] != 0);
			return 2 * num;
		}
		else //�߾ӿ� ���� ���� ���
			return 4;
	}
	else if (edge == true && center == true)
	{
		int n = rand() % 3;
		if (n < 2) //���� ���� ���� ���
		{
			do {
				num = rand() % 4;
			} while (boardcop[2 * num + 1] != 0);
			return 2 * num + 1;
		}
		else //�߾ӿ� ���� ���� ���
			return 4;
	}
	else if (vertex == true) //�Ϳ� ���� ���� ���
	{
		do {
			num = rand() % 5;
		} while (num == 2 || boardcop[2 * num] != 0);
		return 2 * num;
	}
	else if (edge == true) //���� ���� ���� ���
	{
		do {
			num = rand() % 4;
		} while (boardcop[2 * num + 1] != 0);
		return 2 * num + 1;
	}
	else if (center == true) //�߾ӿ� ���� ���� ���
		return 4;
	else //���� �� �ִ� ���� ���� ���(����ó��)
		return 9;

}

inline int tree::me(char* boardcop)
{
	int num;
	do {
		num = rand() % 9;
	} while (boardcop[num] != 0); //�� ������ board�� ������ ���� �� �� ����

	return num;
}

inline bool tree::win(char* boardcop)
{
	bool b;
	//0 3 6, 1 4 7, 2 5 8, 0 1 2, 3 4 5, 6 7 8, 0 4 8, 2 4 6
	if (boardcop[0] == boardcop[3] && boardcop[3] == boardcop[6] && boardcop[0] != 0)
		b = true;
	else if (boardcop[1] == boardcop[4] && boardcop[4] == boardcop[7] && boardcop[1] != 0)
		b = true;
	else if (boardcop[2] == boardcop[5] && boardcop[5] == boardcop[8] && boardcop[2] != 0)
		b = true;
	else if (boardcop[0] == boardcop[1] && boardcop[1] == boardcop[2] && boardcop[0] != 0)
		b = true;
	else if (boardcop[3] == boardcop[4] && boardcop[4] == boardcop[5] && boardcop[3] != 0)
		b = true;
	else if (boardcop[6] == boardcop[7] && boardcop[7] == boardcop[8] && boardcop[6] != 0)
		b = true;
	else if (boardcop[0] == boardcop[4] && boardcop[4] == boardcop[8] && boardcop[0] != 0)
		b = true;
	else if (boardcop[2] == boardcop[4] && boardcop[4] == boardcop[6] && boardcop[2] != 0)
		b = true;
	else
		b = false;
	return b;

}

inline bool tree::simulation()
{
	int count = 0; //���� �������� Ȯ��
	char boardcopy[9] = { 0, }; //simulation�� ���� board�� ���纻 ���
	for (int i = 0; i < 9; i++)
		boardcopy[i] = board[i];
	for (int i = 0; i < 9; i++)
	{
		if (boardcopy[i] == 'o' || boardcopy[i] == 'x')
			count++;
	}
	if (win(board) == true) //�ùķ��̼� ������ �̹� �¸��� �ִ� ���
	{
		if (count % 2 == 1)
			return true; //�� �¸�
		else
			return false; //���� �¸�
	}

	int leftturn = 9 - count; //���ӿ��� ���� �� ��
	if (leftturn % 2 == 0) //�������� ����
	{
		int i = 0; //�� Ƚ�� count
		while (i < leftturn)
		{
			int index = brother(boardcopy);
			boardcopy[index] = 'x';
			if (win(boardcopy) == true)
				return false; //���� �¸�
			i++;
			if (i == leftturn)
				break;
			index = me(boardcopy);
			boardcopy[index] = 'o';
			if (win(boardcopy) == true)
				return true;
			i++;
		}
	}
	else //������ ����
	{
		int i = 0; //�� Ƚ�� count
		while (i < leftturn)
		{
			int index = me(boardcopy);
			boardcopy[index] = 'o';
			if (win(boardcopy) == true)
				return true;
			i++;
			if (i == leftturn)
				break;
			index = brother(boardcopy);
			boardcopy[index] = 'x';
			if (win(boardcopy) == true)
				return false; //���� �¸�
			i++;
		}
	}
	return false;
}

inline void tree::backpropagation(node* child, bool game)
{
	node* cur = child;
	if (game == true) //�̰��� ���
	{
		while (cur) //root���� ����Ƚ��, �¸�Ƚ�� 1 ����
		{
			cur->addwin();
			cur->addarrive();
			cur = cur->getparent();
		}
	}
	else //���� ���
	{
		while (cur) //root���� ����Ƚ�� 1 ����
		{
			cur->addarrive();
			cur = cur->getparent();
		}
	}

}

inline void tree::MCTSwithUCT()
{
	clearboard();
	node* n = selection();
	node* child = expansion(n);
	bool game = simulation();
	backpropagation(child, game);
}

inline bool tree::playgame()
{
	clearboard();	
	node* cur = root;
	while (!cur->getchildnodes().empty()) //childnode�� ������� ���� ��� childnode ���� ������ �ִ��� ���� Ȯ��
	{
		vector<node*> v = cur->getchildnodes();
		if (cur == root || cur->getlevel() % 2 == 1) //�� ���ʿ� ���� ���� ���� ��
		{
			int num; //�� Ȯ���� ���� ���� node�� index
			int check = 0;

			num = me(board);
			for (auto iter = v.begin(); iter != v.end(); iter++) //Ʈ���� �ش� ������ ���� ���
			{
				if ((*iter)->getindex() == num)
				{
					cur = (*iter);
					board[num] = 'o';
					check = 1;
					break;
				}
			}
			if (check == 0) //Ʈ���� �ش� ������ ���� ���
			{
				board[num] = 'o';
				if (win(board) == true && cur->getlevel() % 2 == 1) //���� ���� �� ������ ���ӿ��� �¸��� ���
					return true;
				break; //���� ����� �������� �ʾ��� ��� while���� �������� ������ ������ ����
			}
		}
		else if (cur->getlevel() % 2 == 0) //���� ���ʿ� ���� ���� ���� ��
		{
			int num; //�̱� Ȯ���� ���� ���� node�� index
			int check = 0;
			
			num = brother(board);
			for (auto iter = v.begin(); iter != v.end(); iter++) //Ʈ���� �ش� ������ ���� ���
			{
				if ((*iter)->getindex() == num)
				{
					cur = (*iter);
					board[num] = 'x';
					check = 1;
					break;
				}
			}
			if (check == 0) //Ʈ���� �ش� ������ ���� ���
			{
				board[num] = 'x';
				if (win(board) == true && cur->getlevel() % 2 == 0) //������ ���� �� ������ ���ӿ��� �¸��� ���
					return false;
				break; //���� ����� �������� �ʾ��� ��� while���� �������� ������ ������ ����
			}
		}
		if (win(board) == true)
		{
			if (cur->getlevel() % 2 == 0) //������ ���� �� ���
				return false;
			else //���� ���� �� ���
				return true;
		}
	}
	bool result = simulation();
	return result;
}