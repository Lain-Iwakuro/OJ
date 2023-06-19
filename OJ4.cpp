#include<stdio.h>
#include<algorithm>
#include<queue>
#define D 2 // 维数
#define inf 2147483647 // int类型的最大值
#define ll long long // 发现网上很多人这么写，学来的
#pragma warning(disable:4996)
#pragma warning(disable:6031)
//#pragma GCC optimize("Ofast, no-stack-protecter")
//#pragma GCC target("sse, sse2m sse3, ssse3, sse4, popcnt, abm, mmx, avx, tune=native")
//#pragma GCC optimize("unroll-loops")
using namespace std;
int N, M, k, cutting_dim = 0;

// 最基本的类：坐标点。为便于使用，增加了num属性作为编号
struct point {
	int coord[D], num; // D维坐标与编号
	const bool operator < (const point& p) const {
		return this->coord[cutting_dim] < p.coord[cutting_dim];
	}
}m; // m为领班

// 算法的核心：k-d tree。
struct kdt {
	// 默认构造函数，用于构造null对象
	kdt() {
		for (int i = 0; i < D; i++) {
			mn.coord[i] = inf;
			mx.coord[i] = -inf;
			self.coord[i] = 0;
		}
		ch[0] = ch[1] = this;
	}
	// 普通的构造函数，由于在construct函数中会修改ch数组，此处直接赋值为NULL
	kdt(point& p) {
		self = mn = mx = p;
		mn.num = mx.num = -1;
		ch[0] = ch[1] = NULL;
	}
	// 三个属性：自己所在的位置，在各个维度最小的位置，在各个维度最大的位置
	point self, mn, mx;
	// 两棵子树
	kdt* ch[2];
	// “析构”所用函数，似乎不必要
	void destruct() {
		if (ch[0] != NULL) {
			ch[0]->destruct();
			delete ch[0];
		}
		if (ch[1] != NULL) {
			ch[1]->destruct();
			delete ch[1];
		}
	}
}*null; // null为代替NULL的指针，使用更加方便

// 建树函数，使用到递归
kdt* construct(point* pool, int beg, int end, int d) {
	if (beg >= end) return null;
	cutting_dim = d;
	int mid = (beg + end) / 2;
	// 以第k维的中间为界，把子空间再分为两部分
	nth_element(pool + beg, pool + mid, pool + end); // 此处用到cutting_dim
	kdt* p = new kdt(pool[mid]);
	// 递归建造子树
	p->ch[0] = construct(pool, beg, mid, (d + 1) % D);
	p->ch[1] = construct(pool, mid + 1, end, (d + 1) % D);
	for (int i = 0; i < D; i++) { // 记录下各个维度的最值
		p->mn.coord[i] = min(p->self.coord[i], min(p->ch[0] != null ? p->ch[0]->mn.coord[i] : inf, p->ch[1] != null ? p->ch[1]->mn.coord[i] : inf));
		p->mx.coord[i] = max(p->self.coord[i], max(p->ch[0] != null ? p->ch[0]->mx.coord[i] : -inf, p->ch[1] != null ? p->ch[1]->mx.coord[i] : -inf));
	}
	return p; // 返回指针给双亲节点
}

// 距离的类，为绑定工位编号而写
struct dist {
	int num; // 编号
	ll d; // 距离（的平方）
	// 把小于号重载为大于号，只是因为stl中的priority_queue默认为最大堆
	const bool operator<(const dist& di) const {
		return (d > di.d || (d == di.d && num < di.num));
	}
	// 默认构造函数，在查询前填补空白用
	dist() :num(-1), d(-1) {}
	dist(int number, ll distance) :num(number), d(distance) {}
};

// 计算两个D维点的距离并返回，dist的编号为前面那个点的编号
dist get_dist(point& a, point& b) {
	ll distance = 0;
	for (int i = 0; i < D; i++) {
		distance += ((ll)a.coord[i] - (ll)b.coord[i]) * ((ll)a.coord[i] - (ll)b.coord[i]);
	}
	return dist(a.num, distance);
}

// 计算可能的最大值并返回，剪枝用，如果最大值还没有目前的第k个大，返回-1（摆烂）
ll possible_dist(kdt* tree, priority_queue<dist>* pq) {
	if (tree == null) return -1;
	ll mxdist, mndist, farthest_dist = 0;
	for (int i = 0; i < D; i++) {
		mxdist = ((ll)m.coord[i] - (ll)tree->mx.coord[i]) * ((ll)m.coord[i] - (ll)tree->mx.coord[i]);
		mndist = ((ll)m.coord[i] - (ll)tree->mn.coord[i]) * ((ll)m.coord[i] - (ll)tree->mn.coord[i]);
		farthest_dist += mxdist > mndist ? mxdist : mndist;
	}
	if (pq->top().d <= farthest_dist)	return farthest_dist;
	return -1;
}

// 查询函数，先找更有机会发现更远点的子树
void query(kdt* tree, priority_queue<dist>* pq) {
	pq->push(get_dist(tree->self, m));
	if (pq->size() > (unsigned)k)	pq->pop();
	ll ldist = possible_dist(tree->ch[0], pq);
	ll rdist = possible_dist(tree->ch[1], pq);
	if (ldist > rdist) {
		query(tree->ch[0], pq);
		if (rdist >= 0) query(tree->ch[1], pq);
	}
	else if (ldist < rdist) {
		query(tree->ch[1], pq);
		if (rdist >= 0) query(tree->ch[0], pq);
	}
	else if (ldist >= 0) {
		query(tree->ch[0], pq);
		query(tree->ch[1], pq);
	}
}

// 主函数。。。
int main() {
	scanf("%d%d", &N, &M);
	point* pool = new point[N];
	point* ms = new point[M];
	int* ks = new int[M];
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < D; j++)
			scanf("%d", &pool[i].coord[j]);
		pool[i].num = i;
	}
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < D; j++) {
			scanf("%d", &ms[i].coord[j]);
		}
		scanf("%d", &ks[i]);
	}
	kdt* kdtree = construct(pool, 0, N, 0);
	for (int i = 0; i < M; i++) {
		priority_queue<dist>* pq = new priority_queue<dist>;
		m = ms[i];
		k = ks[i];
		for (int j = 0; j < k; j++) {
			pq->push(dist());
		}
		query(kdtree, pq);
		printf("%d\n", pq->top().num);
		delete pq;
	}
	kdtree->destruct();
	delete[] pool;
	delete[] ms;
	delete[] ks;
	return 0;
}