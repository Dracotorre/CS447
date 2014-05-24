// DSCells.cpp
/*
    David Shrock
    CS447 - PSU
    2004.07.25
	
    implementation of DSCells.

    A linear linked list of nodes each containing a DSsolid.

    Each cell must contain a DSsolid that has an adjoining face that
    "should" have a point in common with another DSsolid forming a cell
    structure. No DSsolid may intersect.

    This current implementation does not check for common point. 
*/
#include "DSCells.h"
#include "../intersections.h"      // for aabb test

namespace ds_cells
{
  // default constructor
  DSCells::DSCells()
  {
    head = NULL;
  }

  // *********************************************
  DSCells::DSCells(const DSCells &source)
  { 
    node *cur;             // current for this
    node *orig;            // current for source

    head = NULL;

    if (source.head)
    {
      orig = source.head;
      cur = head;
      head = new node;
      head->solid = source.head->solid;
      head->next = cur;
      cur = head;
      while(orig->next)
      {
        orig = orig->next;
        cur->next = new node;
        cur = cur->next;
        cur->solid = orig->solid;
      }
      cur->next = NULL;
    }
    m_bbmax = source.m_bbmax;
    m_bbmin = source.m_bbmin; 
  }

  // ******************************
  DSCells::~DSCells()
  {
    node *cur;   
  
    while(head)
    {
      cur = head->next;
      delete head;
      head = cur;
    }  
  }
 
  // **********************************************************************
  // add 
  // returns 1 for added, -1 else
  int DSCells::add(DSsolid &solid)
  {
    node *cur = head;
    node *prev = NULL;
    int result = 1;                    
    Vertex solidBBmax;
    Vertex solidBBmin;
    Vertex curBBmax;
    Vertex curBBmin;
    int intersect = 0;                    // 1 if intersection

    solid.getBBmax(solidBBmax);
    solid.getBBmin(solidBBmin);

    // check to see if list is empty
    if (!head)
    {
      head = new node;
      head->solid = &solid;
      head->solid->getBBmax(m_bbmax);     // updated cell bounding box
      head->solid->getBBmin(m_bbmin);
      head->next = cur;
    }
    else
    {
      // travel to end making sure solid is acceptable
      while(cur && result != -1)
      {
        prev = cur;
        
        // make sure solid does not intersect cur->solid
        cur->solid->getBBmax(curBBmax);
        cur->solid->getBBmin(curBBmin);
        intersect = testIntersect(curBBmax, curBBmin, solidBBmax, solidBBmin);
        if (intersect == 1) 
        {
          result = -1;                             // intersects, can not add
          break;
        }
        else if (intersect == 0) result = 1;        // common face
        else result = -1;
        
        cur = cur->next;
      }
      // check to see if ok to add to end of list
      if (result == 1)
      {
        prev->next = new node;
        prev = prev->next;
        prev->solid = &solid;
        prev->next = cur;
        // update cells bounding box
        if (solidBBmax.x > m_bbmax.x) m_bbmax.x = solidBBmax.x;
        if (solidBBmax.y > m_bbmax.y) m_bbmax.y = solidBBmax.y;
        if (solidBBmax.z > m_bbmax.z) m_bbmax.z = solidBBmax.z;
        if (solidBBmin.x < m_bbmin.x) m_bbmin.x = solidBBmin.x;
        if (solidBBmin.y < m_bbmin.y) m_bbmin.y = solidBBmin.y;
        if (solidBBmin.z < m_bbmin.z) m_bbmin.z = solidBBmin.z;
      }
    }
    return result;
  }

  // ************************************************************************
  // draw
  int DSCells::draw(void)
  {
    node *cur = head;

    while(cur)
    {
      cur->solid->vdraw();
      cur = cur->next;
    } 
    return 1;
  }

  // **********************************************************************
  // getBB
  int DSCells::getBB(Vertex &max, Vertex &min)
  {
    max.x = m_bbmax.x;
    max.y = m_bbmax.y;
    max.z = m_bbmax.z;
    min.x = m_bbmin.x;
    min.y = m_bbmin.y;
    min.z = m_bbmin.z;
    return 1;
  }

  // ************************************************************************
  // private functions
  // *************************

  // *************************************
  // testIntersect; returns -1 for not, 0 for common BB face, 1 for intersects
  int DSCells::testIntersect(Vertex &aMax, Vertex &aMin, Vertex &bMax, Vertex &bMin)
  {
    int i = 0;
    Vec3D line1P1, line1P2, line2P1, line2P2;
    int result = aabbIntersection(aMax, aMin, bMax, bMin);
    
    if (result == -1)
    {
      // check to make sure BBs touch somewhere (max to min)

         result = 0;
    }

    return result;
  } 
}

