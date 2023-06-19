#include<stdio.h>
#include<queue>
#pragma warning(disable:4996)
#pragma warning(disable:6031)
#pragma warning(disable:6386)
int Lx, Ly, sx, sy, sz, dx, dy, dz;
int** map;
struct point {
	int x, y, z, dist, prev;
	point() : x(-1), y(-1), z(-1), dist(-1), prev(-1) {}
	point(int xx, int yy, int zz, int distt, int prevv) : x(xx), y(yy), z(zz), dist(distt), prev(prevv) {}
};
std::queue<point*> q;
int main()
{
	// �������ʼ��
	scanf("%d%d", &Lx, &Ly);
	map = new int* [Ly + 2];
	for (int i = 0; i < Ly + 2; i++) {
		map[i] = new int[Lx + 2];
	}
	for (int i = 1; i <= Ly; i++) {
		for (int j = 1; j <= Lx; j++) {
			scanf("%d", &map[i][j]);
		}
	}
	for (int i = 0; i < Lx + 2; i++) {
		map[0][i] = map[Ly + 1][i] = 1;
	}
	for (int i = 1; i <= Ly; i++) {
		map[i][0] = map[i][Lx + 1] = 1;
	}
	scanf("%d %d %d %d %d %d", &sx, &sy, &sz, &dx, &dy, &dz);
	sz = sz - 1; dz = dz - 1; // ����״̬�ı�����ʹ1234��Ϊ0123
	point*** p = new point * *[4];
	for (int i = 0; i < 4; i++) {
		p[i] = new point * [Ly];
		for (int j = 0; j < Ly; j++) {
			p[i][j] = new point[Lx];
			for (int k = 0; k < Lx; k++) {
				p[i][j][k] = point(k, j, i, 1e6, 0); // ����״̬Ĭ��distΪ1e6��Ĭ��prevΪ0
			}
		}
	}
	p[sz][sy][sx].dist = 0; // ����ʼ״̬�ľ�����Ϊ0
	// �����������
	point* t;
	int x, y, z;
	q.push(&p[sz][sy][sx]);
	while (!q.empty())
	{
		t = q.front(); // ��ȡ��������ǰ��״̬
		q.pop(); // ����
		// ��ת
		z = (t->z + 1) % 4;
		if (p[z][t->y][t->x].prev == 0) { // �����ת��״̬δ���ʹ�
			q.push(&p[z][t->y][t->x]); // ����
			p[z][t->y][t->x].prev = 1; // ��¼���﷽ʽ
			p[z][t->y][t->x].dist = t->dist + 1; // ��¼����
			if (t->x == dx && t->y == dy && z == dz)	break; // ������Ŀ��״̬������ѭ��
		}
		// ��ת
		z = (t->z + 3) % 4;
		if (p[z][t->y][t->x].prev == 0) {
			q.push(&p[z][t->y][t->x]);
			p[z][t->y][t->x].prev = 2;
			p[z][t->y][t->x].dist = t->dist + 1;
			if (t->x == dx && t->y == dy && z == dz)	break;
		}
		// ǰ��һ��
		y = t->y + ((t->z + 1) % 2) * (t->z - 1);
		x = t->x + (t->z % 2) * (t->z - 2);
		if (map[y + 1][x + 1] != 1) // ���ǰ�治��ǽ
			if (p[t->z][y][x].prev == 0) {
				q.push(&p[t->z][y][x]);
				p[t->z][y][x].prev = 3;
				p[t->z][y][x].dist = t->dist + 1;
				if (x == dx && y == dy && t->z == dz)	break;
			}
		// ǰ������
		y = t->y + 2 * ((t->z + 1) % 2) * (t->z - 1);
		x = t->x + 2 * (t->z % 2) * (t->z - 2);
		if (map[(y + t->y) / 2 + 1][(x + t->x) / 2 + 1] != 1) // ���ǰ�治��ǽ
			if (map[y + 1][x + 1] != 1) // �����ǰ��Ҳ����ǽ
				if (p[t->z][y][x].prev == 0) {
					q.push(&p[t->z][y][x]);
					p[t->z][y][x].prev = 4;
					p[t->z][y][x].dist = t->dist + 1;
					if (x == dx && y == dy && t->z == dz)	break;
				}
	}
	int dist = p[dz][dy][dx].dist;
	printf("%d\n", dist); // �����С��������
	char* path = new char[3 * dist];
	int count = 0;
	point* pr = &p[dz][dy][dx];
	while (pr->x != sx || pr->y != sy || pr->z != sz) // �Ӻ���ǰ���ÿ������
	{
		switch (pr->prev) { // ����״̬�ĵ��﷽ʽ����
		case 1:
			path[count++] = 'L'; // д�������
			path[count++] = '\0';
			path[count++] = '\0';
			pr = &p[(pr->z + 3) % 4][pr->y][pr->x]; // ��ת����һ��״̬
			break;
		case 2:
			path[count++] = 'R';
			path[count++] = '\0';
			path[count++] = '\0';
			pr = &p[(pr->z + 1) % 4][pr->y][pr->x];
			break;
		case 3:
			path[count++] = 'F';
			path[count++] = '\0';
			path[count++] = '\0';
			pr = &p[pr->z][pr->y - ((pr->z + 1) % 2) * (pr->z - 1)][pr->x - (pr->z % 2) * (pr->z - 2)];
			break;
		case 4:
			path[count++] = 'F';
			path[count++] = '2';
			path[count++] = '\0';
			pr = &p[pr->z][pr->y - 2 * ((pr->z + 1) % 2) * (pr->z - 1)][pr->x - 2 * (pr->z % 2) * (pr->z - 2)];
			break;
		}
	}
	for (int i = 0; i < dist; i++) { // �������������������˳��
		printf("%s\n", &path[3 * (dist - i - 1)]);
	}
	return 0;
}