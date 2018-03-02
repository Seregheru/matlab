#include <iostream>
#include <list>
#include <string>

#define MAX_DEPTH 5

using namespace std;

struct Point {
 double x;
 double y;
 double z;
 Point() : x(0),y(0),z(0) {} 
 Point(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
 void set(double _x, double _y, double _z) {x=_x;y=_y;z=_z;}
};

struct Box {
    Point p1;
    Point p2;
    Box() : p1(), p2() {}
    Box(double _x1, double _y1, double _z1,double _x2, double _y2, double _z2) : p1(_x1,_y1,_z1), p2(_x2,_y2,_z2) {}
    void set(double _x1, double _y1, double _z1,double _x2, double _y2, double _z2) {p1.set(_x1,_y1,_z1);p2.set(_x2,_y2,_z2);}
    int inside(Point & p) { return (( (p1.x <= p.x) && (p.x <= p2.x) && (p1.y <= p.y) && (p.y <= p2.y) && (p1.z <= p.z) && (p.z <= p2.z) ) ? 1 : 0); }
};

class Octree {
    private:
    Box * b;
    int index;
    int depth;
    int leaf;
    Octree * c1; Octree * c2;
    Octree * c3; Octree * c4;
    Octree * c5; Octree * c6;
    Octree * c7; Octree * c8;
    Octree * father;
    public:
    Octree();
    Octree(double _x1, double _y1, double _z1,double _x2, double _y2, double _z2, int _index, int _depth, Octree* _father);
    ~Octree();
    Octree* getChild(int i);
    Octree* getFather();
    int getLeaves(list<Octree*> &leaves);
    Box* getBox();
    int getDepth();
    int isLeaf();
    void makeLeaf();
    void setIndex(int i);
    void merge();
    void split();
    int search(Point &p);
    void String2Octree(string str);
    string Octree2String();
};