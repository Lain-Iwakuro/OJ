#include<stdio.h>
#include<vector>
#include<queue>
#define inf 2147483647
#pragma warning(disable:4996)
#pragma warning(disable:6031)
using std::vector;
using std::priority_queue;
bool flag = true; // 输出flag

// 结点
struct node {
	int v; // 序号
	int w; // 权重
	node* next; // 相关的边，使用邻接表
	bool isVisited; // 标记边用
	node(int vv = -1, int ww = -1, node* n = NULL) : v(vv), w(ww), next(n), isVisited(false) {}
};

// 距离
struct dist
{
	int di; // 距离
	int dest; // 目标点
	int pre; // 前驱
	dist(int d, int de, int p) : di(d), dest(de), pre(p) {}
	const bool operator<(const dist& d) const { return di > d.di; } // 比较的重载，在优先级队列里使用
};
priority_queue<dist> pq; // 优先级队列

// 无向图
struct graph {
	node** adj;
	vector<int>** label;
	graph(int N) : adj(new node* [N]), label(new vector<int>* [N]) {
		for (int i = 0; i < N; i++) {
			adj[i] = new node(i);
			label[i] = new vector<int>;
		}
	}
	// 在两个结点之间插入权重为w的边
	void insert(int v1, int v2, int w) {
		adj[v1]->next = new node(v2, w, adj[v1]->next);
		adj[v2]->next = new node(v1, w, adj[v2]->next);
	}
	// 使用dijkstra算法，将各点到s点最短路径的长度与第一个前驱放入d与prev数组
	void dijkstra(int d[], vector<int> prev[], int s) {
		d[s] = 0;
		node* temp = adj[s]->next;
		// 把源点相关边放入优先级队列
		while (temp != NULL) {
			unsigned i = 0;
			for (; i < label[s]->size(); i++) {
				if (label[s]->at(i) == temp->v)	break;
			}
			if (i == label[s]->size())	pq.push(dist(temp->w, temp->v, s));
			temp = temp->next;
		}
		// 不断添加新边
		while (!pq.empty()) {
			dist dist_t = pq.top();
			pq.pop();
			int n = dist_t.dest;
			if (dist_t.di <= d[n]) { //如果目前最小距离的点先前未被确定过最小距离
				prev[n].push_back(dist_t.pre); //记录下前驱
				if (dist_t.di < d[n]) {
					d[n] = dist_t.di; //记录距离
					temp = adj[n]->next; //把新结点的边放进堆
					while (temp != NULL) {
						unsigned i = 0;
						for (; i < label[n]->size(); i++) {
							if (label[n]->at(i) == temp->v)	break;
						}
						// 如果该边未被标记且生成了更小的距离，将dist放入优先级队列
						if (i == label[n]->size() && d[n] + temp->w <= d[temp->v]) {
							pq.push(dist(d[n] + temp->w, temp->v, dist_t.dest));
						}
						temp = temp->next;
					}
				}
			}
		}
	}
	// 根据prev数组找路，放入path，深度优先搜索
	void get_path(int d[], vector<int> prev[], int S, int D, vector<int>* path) {
		for (unsigned i = 0; i < prev[D].size(); i++) {
			if (flag) path->push_back(prev[D].at(i));
			if (prev[D].at(i) == S)	flag = false; //输出一条路径后不再输出
			label[prev[D].at(i)]->push_back(D); // 标记已走过的边
			get_path(d, prev, S, prev[D].at(i), path);
		}
	}
};

// 显示路径
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
	} // 图的初始化
	scanf("%d%d", &S, &D);
	d[S] = 0;
	g.dijkstra(d, prev, S); // 第一次搜索最短路径
	vector<int>* path = new vector<int>{ D };
	g.get_path(d, prev, S, D, path);
	show_path(path, d);
	printf("\n");
	for (int i = 0; i < N; i++) {
		d[i] = inf;
		prev[i].clear();
	}
	g.dijkstra(d, prev, S); // 第二次搜索最短路径
	path = new vector<int>{ D };
	flag = true;
	g.get_path(d, prev, S, D, path);
	show_path(path, d);
	return 0;
}