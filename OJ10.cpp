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
	// 输入与初始化
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
	sz = sz - 1; dz = dz - 1; // 调整状态的表述，使1234变为0123
	point*** p = new point * *[4];
	for (int i = 0; i < 4; i++) {
		p[i] = new point * [Ly];
		for (int j = 0; j < Ly; j++) {
			p[i][j] = new point[Lx];
			for (int k = 0; k < Lx; k++) {
				p[i][j][k] = point(k, j, i, 1e6, 0); // 所有状态默认dist为1e6，默认prev为0
			}
		}
	}
	p[sz][sy][sx].dist = 0; // 将初始状态的距离设为0
	// 广度优先搜索
	point* t;
	int x, y, z;
	q.push(&p[sz][sy][sx]);
	while (!q.empty())
	{
		t = q.front(); // 获取队列中最前的状态
		q.pop(); // 出队
		// 左转
		z = (t->z + 1) % 4;
		if (p[z][t->y][t->x].prev == 0) { // 如果左转后状态未访问过
			q.push(&p[z][t->y][t->x]); // 入列
			p[z][t->y][t->x].prev = 1; // 记录到达方式
			p[z][t->y][t->x].dist = t->dist + 1; // 记录距离
			if (t->x == dx && t->y == dy && z == dz)	break; // 若到达目标状态，跳出循环
		}
		// 右转
		z = (t->z + 3) % 4;
		if (p[z][t->y][t->x].prev == 0) {
			q.push(&p[z][t->y][t->x]);
			p[z][t->y][t->x].prev = 2;
			p[z][t->y][t->x].dist = t->dist + 1;
			if (t->x == dx && t->y == dy && z == dz)	break;
		}
		// 前进一格
		y = t->y + ((t->z + 1) % 2) * (t->z - 1);
		x = t->x + (t->z % 2) * (t->z - 2);
		if (map[y + 1][x + 1] != 1) // 如果前面不是墙
			if (p[t->z][y][x].prev == 0) {
				q.push(&p[t->z][y][x]);
				p[t->z][y][x].prev = 3;
				p[t->z][y][x].dist = t->dist + 1;
				if (x == dx && y == dy && t->z == dz)	break;
			}
		// 前进两格
		y = t->y + 2 * ((t->z + 1) % 2) * (t->z - 1);
		x = t->x + 2 * (t->z % 2) * (t->z - 2);
		if (map[(y + t->y) / 2 + 1][(x + t->x) / 2 + 1] != 1) // 如果前面不是墙
			if (map[y + 1][x + 1] != 1) // 如果再前面也不是墙
				if (p[t->z][y][x].prev == 0) {
					q.push(&p[t->z][y][x]);
					p[t->z][y][x].prev = 4;
					p[t->z][y][x].dist = t->dist + 1;
					if (x == dx && y == dy && t->z == dz)	break;
				}
	}
	int dist = p[dz][dy][dx].dist;
	printf("%d\n", dist); // 输出最小操作次数
	char* path = new char[3 * dist];
	int count = 0;
	point* pr = &p[dz][dy][dx];
	while (pr->x != sx || pr->y != sy || pr->z != sz) // 从后往前获得每个操作
	{
		switch (pr->prev) { // 根据状态的到达方式分类
		case 1:
			path[count++] = 'L'; // 写入操作名
			path[count++] = '\0';
			path[count++] = '\0';
			pr = &p[(pr->z + 3) % 4][pr->y][pr->x]; // 跳转到上一个状态
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
	for (int i = 0; i < dist; i++) { // 逆序输出，得正常操作顺序
		printf("%s\n", &path[3 * (dist - i - 1)]);
	}
	return 0;
}