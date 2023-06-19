#include<stdio.h>
#include<algorithm>
#include<queue>
#define D 2 // ά��
#define inf 2147483647 // int���͵����ֵ
#define ll long long // �������Ϻܶ�����ôд��ѧ����
#pragma warning(disable:4996)
#pragma warning(disable:6031)
//#pragma GCC optimize("Ofast, no-stack-protecter")
//#pragma GCC target("sse, sse2m sse3, ssse3, sse4, popcnt, abm, mmx, avx, tune=native")
//#pragma GCC optimize("unroll-loops")
using namespace std;
int N, M, k, cutting_dim = 0;

// ��������ࣺ����㡣Ϊ����ʹ�ã�������num������Ϊ���
struct point {
	int coord[D], num; // Dά��������
	const bool operator < (const point& p) const {
		return this->coord[cutting_dim] < p.coord[cutting_dim];
	}
}m; // mΪ���

// �㷨�ĺ��ģ�k-d tree��
struct kdt {
	// Ĭ�Ϲ��캯�������ڹ���null����
	kdt() {
		for (int i = 0; i < D; i++) {
			mn.coord[i] = inf;
			mx.coord[i] = -inf;
			self.coord[i] = 0;
		}
		ch[0] = ch[1] = this;
	}
	// ��ͨ�Ĺ��캯����������construct�����л��޸�ch���飬�˴�ֱ�Ӹ�ֵΪNULL
	kdt(point& p) {
		self = mn = mx = p;
		mn.num = mx.num = -1;
		ch[0] = ch[1] = NULL;
	}
	// �������ԣ��Լ����ڵ�λ�ã��ڸ���ά����С��λ�ã��ڸ���ά������λ��
	point self, mn, mx;
	// ��������
	kdt* ch[2];
	// �����������ú������ƺ�����Ҫ
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
}*null; // nullΪ����NULL��ָ�룬ʹ�ø��ӷ���

// ����������ʹ�õ��ݹ�
kdt* construct(point* pool, int beg, int end, int d) {
	if (beg >= end) return null;
	cutting_dim = d;
	int mid = (beg + end) / 2;
	// �Ե�kά���м�Ϊ�磬���ӿռ��ٷ�Ϊ������
	nth_element(pool + beg, pool + mid, pool + end); // �˴��õ�cutting_dim
	kdt* p = new kdt(pool[mid]);
	// �ݹ齨������
	p->ch[0] = construct(pool, beg, mid, (d + 1) % D);
	p->ch[1] = construct(pool, mid + 1, end, (d + 1) % D);
	for (int i = 0; i < D; i++) { // ��¼�¸���ά�ȵ���ֵ
		p->mn.coord[i] = min(p->self.coord[i], min(p->ch[0] != null ? p->ch[0]->mn.coord[i] : inf, p->ch[1] != null ? p->ch[1]->mn.coord[i] : inf));
		p->mx.coord[i] = max(p->self.coord[i], max(p->ch[0] != null ? p->ch[0]->mx.coord[i] : -inf, p->ch[1] != null ? p->ch[1]->mx.coord[i] : -inf));
	}
	return p; // ����ָ���˫�׽ڵ�
}

// ������࣬Ϊ�󶨹�λ��Ŷ�д
struct dist {
	int num; // ���
	ll d; // ���루��ƽ����
	// ��С�ں�����Ϊ���ںţ�ֻ����Ϊstl�е�priority_queueĬ��Ϊ����
	const bool operator<(const dist& di) const {
		return (d > di.d || (d == di.d && num < di.num));
	}
	// Ĭ�Ϲ��캯�����ڲ�ѯǰ��հ���
	dist() :num(-1), d(-1) {}
	dist(int number, ll distance) :num(number), d(distance) {}
};

// ��������Dά��ľ��벢���أ�dist�ı��Ϊǰ���Ǹ���ı��
dist get_dist(point& a, point& b) {
	ll distance = 0;
	for (int i = 0; i < D; i++) {
		distance += ((ll)a.coord[i] - (ll)b.coord[i]) * ((ll)a.coord[i] - (ll)b.coord[i]);
	}
	return dist(a.num, distance);
}

// ������ܵ����ֵ�����أ���֦�ã�������ֵ��û��Ŀǰ�ĵ�k���󣬷���-1�����ã�
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

// ��ѯ���������Ҹ��л��ᷢ�ָ�Զ�������
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

// ������������
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