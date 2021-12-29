#include "Graph_adj_list.h"
#include <queue>

extern void errorExit(const char *s);

SLList2 pool;
unsigned long UsedMemoryForArray = 0;

void Read_Graph_adj_list(int Vnum, vertex *V, int Enum, edge *E) {
	// ���Ͽ��� �׷��� ������ �о� V�� E �迭�� �����Ѵ�.
	// V[ ].name�� E[ ].name�� ������ �ʿ䰡 ������ ���ǻ� �� �迭������
	//   index�� �����Ѵ�.
	// V[ ].S�� adjacent list header�� ���� ����ε� ���ȴ�.
	// ���� edge (i,j)�� ������ V[i].S�� edge index k�� push�ϰ�
	//  ���������� V[j].S�� k�� push�Ѵ�.
	// V[].flag�� E[].flag�� �ݵ�� false�� �����ؾ� �Ѵ�.

	int vf, vr, ec;
	SLL* p;
	// Add each edge to the adj list
	for (int e = 0; e < Enum; ++e) {
		scanf_s("%d %d %d", &vf, &vr, &ec);
		 
		p = pool.allocSLL();
		p->i = e;
		V[vf].S.push(p);
		
		p = pool.allocSLL();
		p->i = e;
		V[vr].S.push(p);

		E[e].name = e;
		E[e].cost = ec;
		E[e].flag = false;
		E[e].vf = vf;
		E[e].vr = vr;
	}
	// Initialize each vertex
	for (int v = 0; v < Vnum; ++v) {
		V[v].name = v;
		V[v].flag = false;
	}
}

void Free_Graph_adj_list(int Vnum, vertex *V) {
	// V �迭�� adjacency list�� ���Ҹ� pop()�� ����Ͽ�
	// ���  pool�� �ǵ��� �ش�
	int v;
	SLL *p;
	for (v = 0; v < Vnum; ++v) {
		while (V[v].S.empty() == false) {
			p = V[v].S.pop();
			pool.freeSLL(p);
		}
	}
}

int BFS_Tree_adj_list(
	int     src,   // source node index
	int     Vnum,  // number of vertices
	vertex *V,     // vertex structure array (starting index 0)
	int     Enum,  // number of edges
	edge   *E     // edge structure array (starting index 0)
) {
	// BFS�� ����Ͽ� BFS tree�� ���ϴ� ������ flag�� true�� �����Ѵ�.
	// BFS ���� vertex�� �Է� ���Ͽ��� �����ȴ�(src).
	// V[]�� adjacent list Ž���� .top() ��� �Լ��� ����Ͽ�
	//  adj list ����� ��� �̸� ���󰡸� ���� vertex�� Ž���Ѵ�.
	// BFS tree�� ���� ������ cost ���� return �Ѵ�
	//   (not unique but ��� �� edge cost�� 1�̸� unique)
	// -- �Լ��� ��� parameter�� ���� �ʿ�� ����.
	// -- BFS�� ���� queue�� �ʿ��ϸ� STL�� queue�� ����ص� ����

	std::queue <int> q;
	SLL* e;
	int curr_v, next_v, cost = 0;
	// Push source node to the queue and mark it as visited
	q.push(src);
	V[src].flag = true;

	while (q.empty() == false) {
		curr_v = q.front();
		q.pop();
		if (V[curr_v].S.empty() == false) {
			e = V[curr_v].S.top();
			// Examine all adjacent nodes
			do{
				// Find the adjacent node of the current node using incident edge
				(E[e->i].vf == curr_v) ? (next_v = E[e->i].vr) : (next_v = E[e->i].vf);
				// Push the unvisited adjacent node to the queue
				if (V[next_v].flag == false) {
					q.push(next_v);
					E[e->i].flag = V[next_v].flag = true;
					cost += E[e->i].cost;
				}
				e = e->p;
			} while (e != NULL);

		}
	}
	
	return cost;
}

// SLL management functions
// �Ʒ� �Լ����� 02_04_CSE3081_CPrg&DataStructure_�߰��ڷ�.pdf��
//   �����Ͽ� �� �Լ����� ä�� ����.
// ���ø� �ʿ��ϹǷ� ť�� ��� �Լ��� �Ʒ� �Լ� ����Ʈ�� ����

// stack member fuctions
void sllStack2::push(SLL *p) {
	// insert p to the top of stack ST
	p->p = ST;
	ST = p;
}

SLL *sllStack2::pop(void) {
	// remove and return p at the top of ST
	SLL *p = ST;
	if (ST->p == NULL)
		ST = NULL;
	else
		ST = ST->p;

	return(p);
}

SLL *sllStack2::top(void) {
	// return p at the top of ST
	return ST;
}

bool sllStack2::empty(void) {
	// return true if empty
	if (ST == NULL)
		return true;
	else
		return false;
}

// SLList2 member functions
SLL *SLList2::allocSLL(void) {		// allocate an SLL element
	SLL* p;

	if (SLL_pool == NULL) {
		p = (SLL*)malloc(sizeof(SLL));
		if (p == NULL)
			errorExit("Error in allocSLL");
		UsedMemoryForSLLs += sizeof(*p);
		p->i = NONE;
	}
	else {
		p = SLL_pool;
		SLL_pool = SLL_pool->p;
	}
	SLL_cnt++;
	p->p = NULL;
	return(p);
}

void SLList2::freeSLL(SLL *p) {		// free *p (move it to SLL_pool
	if (p->i == NONE) {
		errorExit("This is already freed(freeSLL).");
	}
	else {
		p->i == NONE;
		p->p = SLL_pool;
		SLL_pool = p;
		SLL_cnt--;
	}
}

void SLList2::freeSLL_pool(void) {	// clear SLL_pool
	SLL *p = SLL_pool;

	while (p != NULL) {
		SLL_pool = p->p;
		free(p);
		UsedMemoryForSLLs -= sizeof(*p);
		p = SLL_pool;
	}

	if (SLL_cnt != 0) {
		errorExit("Non-zero SLL_cnt after cleanup.");
	}
}