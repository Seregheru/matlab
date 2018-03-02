#include "octree.h"
#include <math.h>

Octree::Octree() : c1(0), c2(0), c3(0), c4(0), c5(0), c6(0), c7(0), c8(0), b(0), index(0), depth(0), leaf(1), father(0)
{
    
}

Octree::Octree(double _x1, double _y1, double _z1,double _x2, double _y2, double _z2, int _index, int _depth, Octree* _father) : c1(0), c2(0), c3(0), c4(0), c5(0), c6(0), c7(0), c8(0), b(new Box(_x1, _y1, _z1, _x2, _y2, _z2)), index(_index), depth(_depth), leaf(1), father(_father)
{

}

Octree::~Octree()
{
    delete b;
    if( c1!=0 )
        delete c1;
    if( c2!=0 )
        delete c2;
    if( c3!=0 )
        delete c3;
    if( c4!=0 )
        delete c4;
    if( c5!=0 )
        delete c5;
    if( c6!=0 )
        delete c6;
    if( c7!=0 )
        delete c7;
    if( c8!=0 )
        delete c8;
    //merge();
}

Octree* Octree::getChild(int i)
{
  if(i == 1)
    return c1;
  if(i == 2)
    return c2;
  if(i == 3)
    return c3;
  if(i == 4)
    return c4;
  if(i == 5)
    return c5;
  if(i == 6)
    return c6;
  if(i == 7)
    return c7;
  if(i == 8)
    return c8;
}

Octree* Octree::getFather()
{
    return father;
}

int Octree::isLeaf()
{
    return leaf;
}

void Octree::makeLeaf()
{
    leaf = 1;
}

void Octree::setIndex(int i)
{
    index = i;
}

Box* Octree::getBox()
{
    return b;
}

int Octree::getDepth()
{
    return depth;
}

void Octree::merge()
{
    if ( c1->isLeaf()==1 ) 
    {
      if(c1!=0)
        delete c1;
      c1 = 0;
    }
    else
    {
      c1->merge();
      if(c1!=0)
        delete c1;
      c1 = 0;
    }
    if ( c2->isLeaf()==1 ) 
    {
      if(c2!=0)
        delete c2;
      c2 = 0;
    }
    else
    {
      c2->merge();
      if(c2!=0)
        delete c2;
      c2 = 0;
    }
    if ( c3->isLeaf()==1 ) 
    {
      if(c3!=0)
        delete c3;
      c3 = 0;
    }
    else
    {
      c3->merge();
      if(c3!=0)
        delete c3;
      c3 = 0;
    }
    if ( c4->isLeaf()==1 ) 
    {
      if(c4!=0)
        delete c4;
      c4 = 0;
    }
    else
    {
      c4->merge();
      if(c4!=0)
        delete c4;
      c4 = 0;
    }
    if ( c5->isLeaf()==1 ) 
    {
      if(c5!=0)
        delete c5;
      c5 = 0;
    }
    else
    {
      c5->merge();
      if(c5!=0)
        delete c5;
      c5 = 0;
    }
    if ( c6->isLeaf()==1 ) 
    {
      if(c6!=0)
        delete c6;
      c6 = 0;
    }
    else
    {
      c6->merge();
      if(c6!=0)
        delete c6;
      c6 = 0;
    }
    if ( c7->isLeaf()==1 ) 
    {
      if(c7!=0)
        delete c7;
      c7 = 0;
    }
    else
    {
      c7->merge();
      if(c7!=0)
        delete c7;
      c7 = 0;
    }
    if ( c8->isLeaf()==1 ) 
    {
      if(c8!=0)
        delete c8;
      c8 = 0;
    }
    else
    {
      c8->merge();
      if(c8!=0)
        delete c8;
      c8 = 0;
    }
    leaf = 1;
}

void Octree::split()
{
    if( (leaf==1) && (depth<MAX_DEPTH) )
    {
        double x1 = b->p1.x; double y1 = b->p1.y; double z1 = b->p1.z;
        double x2 = b->p2.x; double y2 = b->p2.y; double z2 = b->p2.z;
        double h = x2-x1; double w = y2-y1; double k = z2-z1;
        //cout << this << "\n";
        c1 = new Octree(x1,y1,z1,x1+h/2,y1+w/2,z1+k/2,9*index+1,depth+1,this);
        c2 = new Octree(x1+h/2,y1,z1,x1+h,y1+w/2,z1+k/2,9*index+2,depth+1,this);
        c3 = new Octree(x1,y1+w/2,z1,x1+h/2,y1+w,z1+k/2,9*index+3,depth+1,this);
        c4 = new Octree(x1+h/2,y1+w/2,z1,x1+h,y1+w,z1+k/2,9*index+4,depth+1,this);
        c5 = new Octree(x1,y1,z1+k/2,x1+h/2,y1+w/2,z1+k,9*index+5,depth+1,this);
        c6 = new Octree(x1+h/2,y1,z1+k/2,x1+h,y1+w/2,z1+k,9*index+6,depth+1,this);
        c7 = new Octree(x1,y1+w/2,z1+k/2,x1+h,y1+w,z1+k,9*index+7,depth+1,this);
        c8 = new Octree(x1+h/2,y1+w/2,z1+k/2,x1+h,y1+w,z1+k,9*index+8,depth+1,this);
        leaf = 0;
    }
}

int Octree::search(Point &p)
{
    if( leaf==1 )
    {
        if ( b->inside(p)==1 )
            return index;
        else
            return -1;
    }
    else
    {
        int t1 = c1->search(p);
        if (t1 > 0)
            return t1;
        int t2 = c2->search(p);
        if (t2 > 0)
            return t2;
        int t3 = c3->search(p);
        if (t3 > 0)
            return t3;
        int t4 = c4->search(p);
        if (t4 > 0)
            return t4;
        int t5 = c5->search(p);
        if (t5 > 0)
            return t5;
        int t6 = c6->search(p);
        if (t6 > 0)
            return t6;
        int t7 = c7->search(p);
        if (t7 > 0)
            return t7;
        int t8 = c8->search(p);
        if (t8 > 0)
            return t8;
        return -1;
    }
}

int Octree::getLeaves(list<Octree*> &leaves)
{
    if( leaf == 1 )
    {
        leaves.push_back(this);
        return 1;
    }
    else
    {
        int sum = 0;
        sum = sum + c1->getLeaves(leaves);
        sum = sum + c2->getLeaves(leaves);
        sum = sum + c3->getLeaves(leaves);
        sum = sum + c4->getLeaves(leaves);
        sum = sum + c5->getLeaves(leaves);
        sum = sum + c6->getLeaves(leaves);
        sum = sum + c7->getLeaves(leaves);
        sum = sum + c8->getLeaves(leaves);
        return sum;
    }
}

string Octree::Octree2String()
{
  if( leaf == 1 )
  {
    return string("");
  }
  else
  {
    string s1 = c1->Octree2String();
    string s2 = c2->Octree2String();
    string s3 = c3->Octree2String();
    string s4 = c4->Octree2String();
    string s5 = c5->Octree2String();
    string s6 = c6->Octree2String();
    string s7 = c7->Octree2String();
    string s8 = c8->Octree2String();
    return string("(")+s1+")"+"("+s2+")"+"("+s3+")"+"("+s4+")"+"("+s5+")"+"("+s6+")"+"("+s7+")"+"("+s8+")";
  } 
}

void Octree::String2Octree(string str)
{
  if(str.size()>0)
  {
      int count = 0; int subtree = 0;
      int start = 0;
      string str1,str2,str3,str4,str5,str6,str7,str8;
      for(int i=0;i<str.size();i++) {
        if(str[i] == '(')
            count++;
        else
            count--;
        if(count==0)
        {
           subtree++;
           if(subtree==1)
               str1 = str.substr(start+1, i - start-1);
           if(subtree==2)
               str2 = str.substr(start+1, i - start-1);
           if(subtree==3)
               str3 = str.substr(start+1, i - start-1);
           if(subtree==4)
               str4 = str.substr(start+1, i - start-1);
           if(subtree==5)
               str5 = str.substr(start+1, i - start-1);
           if(subtree==6)
               str6 = str.substr(start+1, i - start-1);
           if(subtree==7)
               str7 = str.substr(start+1, i - start-1); 
           if(subtree==8)
           {
               str8 = str.substr(start+1, i - start-1);
               subtree = 0;
           }
           start = i+1;
        }
      }
      split();
      c1->String2Octree(str1);
      c2->String2Octree(str2);
      c3->String2Octree(str3);
      c4->String2Octree(str4);
      c5->String2Octree(str5);
      c6->String2Octree(str6);
      c7->String2Octree(str7);
      c8->String2Octree(str8);
  }
}

