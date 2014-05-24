// EntityList.h
/*
  David Shrock
	CS447 - PSU
	2004.07.22

  header for EntityList.cpp

  List of Entity objects.  Stored by id which is returned to caller to identify
  later when wanting to remove.
*/

#ifndef _ENTITY_LIST_H
#define _ENTITY_LIST_H

namespace d_entity_list
{
  struct node 
  {
    int id;                     // id of entity 
    Entity *obj;                // ptr to real Entity
    node *next;
  };
  class EntityList
  {
    public:
      EntityList();
      EntityList(const EntityList &);
      ~EntityList();
      EntityList & operator =(EntityList &);

      int insert(const Entity &obj);    // returns id number of entity
      int remove(int id);         // returns 1 for removed, -1 for id not found

      int render(void);             // renders entire list

    private:
      node *head;

  };
}
