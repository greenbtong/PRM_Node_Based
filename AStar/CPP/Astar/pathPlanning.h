#pragma once
#include <list>
#include <algorithm>
#include <iostream>

#include "maps.h"

class point {
public:
	point(int a = 0, int b = 0) { x = a; y = b; }
	bool operator == (const point& op) { return op.x == x && op.y == y; }
	point operator + (const point& op) { return point(op.x + x, op.y + y); }
	int x, y;
};

class node {
public:
	bool operator == (const node& op) { return post == op.post; }
	bool operator == (const point& op) { return post == op; }
	bool operator < (const node& op) { return dist + cost < op.dist + op.cost; }
	point post, parent;
	int dist, cost;
};

class aStar {
public:
	aStar() {
		directions[0] = point(-1, -1); // NW
		directions[1] = point(1, -1); // SW
		directions[2] = point(-1, 1); // NE
		directions[3] = point(1, 1); // SE
		directions[4] = point(0, -1); // W
		directions[5] = point(-1, 0); // N
		directions[6] = point(0, 1);  // E
		directions[7] = point(1, 0); // S
	}

	int calcDist(point & p) {
		// Euclidean Distance
		int x = end.x - p.x, y = end.y - p.y;
		return((int)sqrt(x * x + y * y));
	}

	bool checkValid(point & p) {
		return (p.x > -1 && p.y > -1 && p.x < m.height && p.y < m.width);
	}

	bool checkPoint(point & p, int cost) {
		list<node>::iterator i;
		i = find(closedN.begin(), closedN.end(), p);
		if (i != closedN.end()) {
			if ((*i).cost + (*i).dist < cost) return true;
			else { closedN.erase(i); return false; }
		}
		i = find(openN.begin(), openN.end(), p);
		if (i != openN.end()) {
			if ((*i).cost + (*i).dist < cost) return true;
			else { openN.erase(i); return false; }
		}
		return false;
	}

	bool fillOpenNode(node & n) {
		int nc, dist;
		point direction;

		for (int x = 0; x < 8; x++) {
			direction = n.post + directions[x];
			if (direction == end) return true;

			if (checkValid(direction) && m(direction.x, direction.y) != 1) {
				nc = 1 + n.cost; // nc = cost of each step + remaining distance estimate
				dist = calcDist(direction);
				if (!checkPoint(direction, nc + dist)) {
					node m;
					m.cost = nc; m.dist = dist;
					m.post = direction;
					m.parent = n.post;
					openN.push_back(m);
				}
			}
		}
		return false;
	}


	bool check(point & s, point & e, maps & ms) {
		node n; end = e; start = s; m = ms;
		n.cost = 0; n.post = s; n.parent = 0; n.dist = calcDist(s);
		openN.push_back(n);
		while (!openN.empty()) {
			node n = openN.front();
			openN.pop_front();
			closedN.push_back(n);
			if (fillOpenNode(n)) return true;
		}
		return false;
	}

	int path(list<point> & path) {
		path.push_front(end);
		int cost = 1 + closedN.back().cost;
		path.push_front(closedN.back().post);
		point parent = closedN.back().parent;

		for (list<node>::reverse_iterator i = closedN.rbegin(); i != closedN.rend(); i++) {
			if ((*i).post == parent && !((*i).post == start)) {
				path.push_front((*i).post);
				parent = (*i).parent;
			}
		}
		path.push_front(start);
		return cost;
	}

	maps m; 
	point end, start, directions[8];
	list<node> openN, closedN;
};