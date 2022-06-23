#include "tree.h"

inline node* tree::selection() //상황에 따라 simulation이 발생할 node 찾기 (Pure Monte Carlo tree search 방법)
{
	node* cur = root; //current node
	queue<node*> q; //level order를 위한 큐
	int level = 0; //내 차례인지 동생 차례인지 구분
	
	while (!cur->getchildnodes().empty()) //pure MCTS 알고리즘 적용 
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
	while (n->getparent()) //board에 트리를 거치면서 얻은 게임 상황 적용
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

inline node* tree::expansion(node* n) //n은 selection을 통해 찾은 node
{
	node* cur = n;
	int count = 0; //진행된 게임 횟수
	while (cur->getparent())
	{
		count++;
		cur = cur->getparent();
	}

	vector<node*> childnode = n->getchildnodes();
	node* newnode = new node();
	int num;
	
	//childnode들 중에 num을 index로 갖는 node가 있는지, 거쳐 온 부모노드 중에 num을 index로 갖는 node가 있는지 확인
	int check; 
	if ((9 - count) % 2 == 0) //남은 게임 횟수가 짝수 : 동생 차례
	{
		do {
			check = 0;
			num = brother(board);
			for (auto iter = childnode.begin(); iter != childnode.end(); iter++)
			{
				if ((*iter)->getindex() == num) //있을 경우 check
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
	else //내 차례
	{
		do {
			check = 0;
			num = me(board);
			for (auto iter = childnode.begin(); iter != childnode.end(); iter++)
			{
				if ((*iter)->getindex() == num) //있을 경우 check
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

	childnode.push_back(newnode); //childnode에 node 삽입
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

	int num; //동생의 수
	if (vertex == true && edge == true && center == true)
	{
		int n = rand() % 7;
		if (n < 4) //귀에 수를 놓는 경우
		{
			do {
				num = rand() % 5;
			} while (num == 2 && boardcop[2 * num] != 0);
			return 2 * num;
		}
		else if (n < 6) //변에 수를 놓는 경우
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
		if (n < 2) //귀에 수를 놓는 경우
		{
			do {
				num = rand() % 5;
			} while (num == 2 && boardcop[2 * num] != 0);
			return 2 * num;
		}
		else //변에 수를 놓는 경우
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
		if (n < 4) //귀에 수를 놓는 경우
		{
			do {
				num = rand() % 5;
			} while (num == 2 && boardcop[2 * num] != 0);
			return 2 * num;
		}
		else //중앙에 수를 놓는 경우
			return 4;
	}
	else if (edge == true && center == true)
	{
		int n = rand() % 3;
		if (n < 2) //변에 수를 놓는 경우
		{
			do {
				num = rand() % 4;
			} while (boardcop[2 * num + 1] != 0);
			return 2 * num + 1;
		}
		else //중앙에 수를 놓는 경우
			return 4;
	}
	else if (vertex == true) //귀에 수를 놓는 경우
	{
		do {
			num = rand() % 5;
		} while (num == 2 || boardcop[2 * num] != 0);
		return 2 * num;
	}
	else if (edge == true) //변에 수를 놓는 경우
	{
		do {
			num = rand() % 4;
		} while (boardcop[2 * num + 1] != 0);
		return 2 * num + 1;
	}
	else if (center == true) //중앙에 수를 놓는 경우
		return 4;
	else //놓을 수 있는 수가 없을 경우(예외처리)
		return 9;

}

inline int tree::me(char* boardcop)
{
	int num;
	do {
		num = rand() % 9;
	} while (boardcop[num] != 0); //현 노드까지 board에 나온적 없는 값 중 선택

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
	int count = 0; //누구 차례인지 확인
	char boardcopy[9] = { 0, }; //simulation을 위해 board의 복사본 사용
	for (int i = 0; i < 9; i++)
		boardcopy[i] = board[i];
	for (int i = 0; i < 9; i++)
	{
		if (boardcopy[i] == 'o' || boardcopy[i] == 'x')
			count++;
	}
	if (win(board) == true) //시뮬레이션 이전에 이미 승리해 있는 경우
	{
		if (count % 2 == 1)
			return true; //내 승리
		else
			return false; //동생 승리
	}

	int leftturn = 9 - count; //게임에서 남은 턴 수
	if (leftturn % 2 == 0) //동생부터 시작
	{
		int i = 0; //턴 횟수 count
		while (i < leftturn)
		{
			int index = brother(boardcopy);
			boardcopy[index] = 'x';
			if (win(boardcopy) == true)
				return false; //동생 승리
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
	else //나부터 시작
	{
		int i = 0; //턴 횟수 count
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
				return false; //동생 승리
			i++;
		}
	}
	return false;
}

inline void tree::backpropagation(node* child, bool game)
{
	node* cur = child;
	if (game == true) //이겼을 경우
	{
		while (cur) //root까지 게임횟수, 승리횟수 1 더함
		{
			cur->addwin();
			cur->addarrive();
			cur = cur->getparent();
		}
	}
	else //졌을 경우
	{
		while (cur) //root까지 게임횟수 1 더함
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
	while (!cur->getchildnodes().empty()) //childnode가 비어있지 않을 경우 childnode 내에 정보가 있는지 먼저 확인
	{
		vector<node*> v = cur->getchildnodes();
		if (cur == root || cur->getlevel() % 2 == 1) //내 차례에 대한 수를 정할 때
		{
			int num; //질 확률이 가장 높은 node의 index
			int check = 0;

			num = me(board);
			for (auto iter = v.begin(); iter != v.end(); iter++) //트리에 해당 정보가 있을 경우
			{
				if ((*iter)->getindex() == num)
				{
					cur = (*iter);
					board[num] = 'o';
					check = 1;
					break;
				}
			}
			if (check == 0) //트리에 해당 정보가 없을 경우
			{
				board[num] = 'o';
				if (win(board) == true && cur->getlevel() % 2 == 1) //내가 수를 둔 순간에 게임에서 승리할 경우
					return true;
				break; //게임 결과가 결정되지 않았을 경우 while문을 빠져나가 나머지 게임을 진행
			}
		}
		else if (cur->getlevel() % 2 == 0) //동생 차례에 대한 수를 정할 때
		{
			int num; //이길 확률이 가장 높은 node의 index
			int check = 0;
			
			num = brother(board);
			for (auto iter = v.begin(); iter != v.end(); iter++) //트리에 해당 정보가 있을 경우
			{
				if ((*iter)->getindex() == num)
				{
					cur = (*iter);
					board[num] = 'x';
					check = 1;
					break;
				}
			}
			if (check == 0) //트리에 해당 정보가 없을 경우
			{
				board[num] = 'x';
				if (win(board) == true && cur->getlevel() % 2 == 0) //동생이 수를 둔 순간에 게임에서 승리할 경우
					return false;
				break; //게임 결과가 결정되지 않았을 경우 while문을 빠져나가 나머지 게임을 진행
			}
		}
		if (win(board) == true)
		{
			if (cur->getlevel() % 2 == 0) //동생이 수를 둔 경우
				return false;
			else //내가 수를 둔 경우
				return true;
		}
	}
	bool result = simulation();
	return result;
}