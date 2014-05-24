// EntityList.h
/*
  David Shrock
	CS447 - PSU
	2004.07.22

  implementation of EntityList.

  A linear linked list holding Entity objects.
*/
#include "EntityList.h"

namespace d_entity_list
{
  // constructor
  EntityList::EntityList()
  {
    head = NULL;
  }

  // ***********************************************
  // copy constructor
  EntityList::EntityList(const EntityList &source)
  {
    node *cur = head;
    node *orig;
    if (source.head)
    {
      orig = source.head;
      head = new node;
      head->id = source.head->id;
      head->obj = source.head->obj;
      head->next = cur;
      cur = head;
      while (orig->next)
      {
        cur->next = new node;
        cur = cur->next;
        orig = orig->next;
        cur->id = orig->id;
        cur->obj = source.head->obj;
      }
      cur->next = NULL;
    }
  }

  // *******************************************
  // destructor
  EntityList::~EntityList()
  {
    node *cur;
    while (head)
    {
      cur = head->next;
      head = cur;
    }
  }

  // ************************************************
  // assignment operator
  EntityList & EntityList::operator =(const EntityList &source)
  {
    node *cur, *orig;
    if (this != &source)
    {
      if (source.head)
      {
        orig = source.head;
        head = new node;
        head->id = source.head->id;
        head->obj = source.head->obj;
        head->next = cur;
        cur = head;
        while (orig->next)
        {
          cur->next = new node;
          cur = cur->next;
          orig = orig->next;
          cur->id = orig->id;
          cur->obj = orig->obj;
        } 
        cur->next = NULL;
      }
    }
    return (*this);
  }

  // **********************************************************************
  // returns id number of entity
  // if objects were removed then, may have missing id numbers, so insert
  // into first available id slot.
  int EntityList::insert(const Entity &obj) 
  {
    int id = 0;                      // current id looking at
    node *cur = head, *prev = NULL;

    // check to see if list is empty
    if (!head)
    {
      head = new node;
      head->id = id;
      head->obj = &obj;
      head->next = cur;
    }
    else if (id < head->id)
    {
      // new one goes first
      cur = head->next;
      head = new node;
      head->id = id;
      head->obj = &obj;
      head->next = cur;
    }
    else
    {
      // traverse to find 1st empty id
      while(cur)
      {

      }
    }
  }
}
      
