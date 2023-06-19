#include<stdio.h>
#include<vector>
#include<queue>
#define inf 2147483647
#pragma warning(disable:4996)
#pragma warning(disable:6031)
using std::vector;
using std::priority_queue;
bool flag = true; // ���flag

// ���
struct node {
	int v; // ���
	int w; // Ȩ��
	node* next; // ��صıߣ�ʹ���ڽӱ�
	bool isVisited; // ��Ǳ���
	node(int vv = -1, int ww = -1, node* n = NULL) : v(vv), w(ww), next(n), isVisited(false) {}
};

// ����
struct dist
{
	int di; // ����
	int dest; // Ŀ���
	int pre; // ǰ��
	dist(int d, int de, int p) : di(d), dest(de), pre(p) {}
	const bool operator<(const dist& d) const { return di > d.di; } // �Ƚϵ����أ������ȼ�������ʹ��
};
priority_queue<dist> pq; // ���ȼ�����

// ����ͼ
struct graph {
	node** adj;
	vector<int>** label;
	graph(int N) : adj(new node* [N]), label(new vector<int>* [N]) {
		for (int i = 0; i < N; i++) {
			adj[i] = new node(i);
			label[i] = new vector<int>;
		}
	}
	// ���������֮�����Ȩ��Ϊw�ı�
	void insert(int v1, int v2, int w) {
		adj[v1]->next = new node(v2, w, adj[v1]->next);
		adj[v2]->next = new node(v1, w, adj[v2]->next);
	}
	// ʹ��dijkstra�㷨�������㵽s�����·���ĳ������һ��ǰ������d��prev����
	void dijkstra(int d[], vector<int> prev[], int s) {
		d[s] = 0;
		node* temp = adj[s]->next;
		// ��Դ����ر߷������ȼ�����
		while (temp != NULL) {
			unsigned i = 0;
			for (; i < label[s]->size(); i++) {
				if (label[s]->at(i) == temp->v)	break;
			}
			if (i == label[s]->size())	pq.push(dist(temp->w, temp->v, s));
			temp = temp->next;
		}
		// ��������±�
		while (!pq.empty()) {
			dist dist_t = pq.top();
			pq.pop();
			int n = dist_t.dest;
			if (dist_t.di <= d[n]) { //���Ŀǰ��С����ĵ���ǰδ��ȷ������С����
				prev[n].push_back(dist_t.pre); //��¼��ǰ��
				if (dist_t.di < d[n]) {
					d[n] = dist_t.di; //��¼����
					temp = adj[n]->next; //���½��ı߷Ž���
					while (temp != NULL) {
						unsigned i = 0;
						for (; i < label[n]->size(); i++) {
							if (label[n]->at(i) == temp->v)	break;
						}
						// ����ñ�δ������������˸�С�ľ��룬��dist�������ȼ�����
						if (i == label[n]->size() && d[n] + temp->w <= d[temp->v]) {
							pq.push(dist(d[n] + temp->w, temp->v, dist_t.dest));
						}
						temp = temp->next;
					}
				}
			}
		}
	}
	// ����prev������·������path�������������
	void get_path(int d[], vector<int> prev[], int S, int D, vector<int>* path) {
		for (unsigned i = 0; i < prev[D].size(); i++) {
			if (flag) path->push_back(prev[D].at(i));
			if (prev[D].at(i) == S)	flag = false; //���һ��·���������
			label[prev[D].at(i)]->push_back(D); // ������߹��ı�
			get_path(d, prev, S, prev[D].at(i), path);
		}
	}
};

// ��ʾ·��
void show_path(vector<int>* path, int d[]) {
	printf("start\n");
	for (int i = path->size() - 1; i >= 0; i--) {
		printf("%d\n", path->at(i));
	}
	printf("end\n%d", d[path->at(0)]);
}



int main() {
	int N, M, S, D;
	scanf("%d%d", &N, &M);
	int* d = new int[N];
	vector<int>* prev = new vector<int>[N];
	graph g(N);
	for (int i = 0; i < N; i++) {
		d[i] = inf;
	}
	int v1, v2, w;
	for (int i = 0; i < M; i++) {
		scanf("%d%d%d", &v1, &v2, &w);
		g.insert(v1, v2, w);
	} // ͼ�ĳ�ʼ��
	scanf("%d%d", &S, &D);
	d[S] = 0;
	g.dijkstra(d, prev, S); // ��һ���������·��
	vector<int>* path = new vector<int>{ D };
	g.get_path(d, prev, S, D, path);
	show_path(path, d);
	printf("\n");
	for (int i = 0; i < N; i++) {
		d[i] = inf;
		prev[i].clear();
	}
	g.dijkstra(d, prev, S); // �ڶ����������·��
	path = new vector<int>{ D };
	flag = true;
	g.get_path(d, prev, S, D, path);
	show_path(path, d);
	return 0;
}