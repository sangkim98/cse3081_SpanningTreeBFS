#include "Graph_adj_list.h"
#include <queue>

extern void errorExit(const char *s);

SLList2 pool;
unsigned long UsedMemoryForArray = 0;

void Read_Graph_adj_list(int Vnum, vertex *V, int Enum, edge *E) {
	// 파일에서 그래프 정보를 읽어 V와 E 배열을 구성한다.
	// V[ ].name과 E[ ].name은 설정할 필요가 없으나 편의상 각 배열에서의
	//   index를 저장한다.
	// V[ ].S는 adjacent list header로 스택 헤더로도 사용된다.
	// 따라서 edge (i,j)를 읽으면 V[i].S에 edge index k를 push하고
	//  마찬가지로 V[j].S에 k를 push한다.
	// V[].flag과 E[].flag은 반드시 false로 설정해야 한다.

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
	// V 배열의 adjacency list의 원소를 pop()을 사용하여
	// 모두  pool로 되돌려 준다
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
	// BFS를 사용하여 BFS tree에 속하는 에지의 flag을 true로 설정한다.
	// BFS 시작 vertex는 입력 파일에서 지정된다(src).
	// V[]의 adjacent list 탐색은 .top() 멤버 함수를 사용하여
	//  adj list 헤더를 얻고 이를 따라가며 인접 vertex를 탐색한다.
	// BFS tree에 속한 에지의 cost 함을 return 한다
	//   (not unique but 모든 각 edge cost가 1이면 unique)
	// -- 함수의 모든 parameter가 사용될 필요는 없다.
	// -- BFS를 위한 queue가 필요하면 STL의 queue를 사용해도 좋다

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
// 아래 함수들은 02_04_CSE3081_CPrg&DataStructure_추가자료.pdf를
//   참조하여 각 함수들을 채워 넣자.
// 스택만 필요하므로 큐의 멤버 함수는 아래 함수 리스트에 없다

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