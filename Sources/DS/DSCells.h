// DSCells.h
/*
    David Shrock
    CS447 - PSU
    2004.07.25
	
    header for DSCells.cpp

    Cell decomposition of DSsolid collection forming a single object that will
    become a new shape containing one set attributes for the new shape.

    Cells must be added in adjoining order, otherwise will be rejected if
    no faces are in common with previous solids.

    Data structure for DSCells is a linear linked list.
*/
#ifndef _DS_CELLS_H
#define _DS_CELLS_H

#include "DSsolid.h"
#include "../Vec3D.h"

namespace ds_cells
{
  struct node          // a cell
  {
    DSsolid *solid;
    node *next;
  };
  class DSCells
  {
    public:
      DSCells();
      DSCells(const DSCells &);
      ~DSCells();

      // add a solid to a node.
      // set solid position relative to 1origin of node group before adding
      // returns 1 for added, -1 for doesn't fit
      int add(DSsolid &);

      // draw Cell structure
      int draw(void);

      int getBB(Vertex &max, Vertex &min);

    private:
      Vertex m_bbmax;        // bounding box mininum of entire cell structure
      Vertex m_bbmin;        // bounding box maximum of entire cells
      node *head;

      // tests intersection of bounding boxes
      // returns -1 for not 0 for common BB face, 1 for intersects
      int testIntersect(Vertex &, Vertex &, Vertex &, Vertex &);
  };
}
#endif
