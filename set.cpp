#include <algorithm>
#include <array>
#include "set.h"
#include <iostream>
#include <iomanip>
#include <conio.h>
using namespace std;
template <class Item>
set<Item>::set()
{
  for (int i = 0; i < MAXIMUM + 1; i ++)
  {
    data[i] = 0;
  }
  for (int i = 0; i < MAXIMUM +2; i++)
  {
    subset[i] = nullptr;
  }
  data_count = 0;
  child_count = 0;
}
template<class Item>
set<Item>::set(const set& source)
{
  child_count = source.child_count;
  data_count = source.data_count;
  for (int i = 0; i < data_count; i++)
    {
      data[i] = source.data[i];
    }
  for (int i = 0; i < child_count; i++)
    {
      subset[i] = new set(*source.subset[i]);
    } 
}
template <class Item>
void set<Item>::operator=(const set &source)
{ 
  child_count = source.child_count;
  data_count = source.data_count;
  for (int i = 0; i < data_count; i++)
    {
      data[i] = source.data[i];
    }
  for (int i = 0; i < child_count; i++)
    {
      subset[i] = new set(*source.subset[i]);
    } 
}
template <class Item>
void set<Item>::clear()
{
  for (int i = 0; i < child_count; i++)
  {
    subset[i]->clear();
    subset[i] = nullptr;
  }
  data_count = 0;
  child_count = 0;
} 
template <class Item>
bool set<Item>::insert(const Item& entry)
    {
      if (empty())
      {
        new set();
        data[0] = entry;
        data_count++;
      }
      else
      {
        if (!loose_insert(entry))
        {
          return false; 
        }
        if (data_count > MAXIMUM)
        {
          set *temp = new set();
          for (int i = 0; i < MAXIMUM+1; i++)
          {
            temp->data[i] = data[i];
            temp->data_count++;
          }
          for (int i = 0; i < child_count; i++)
          {
            temp->subset[i] = subset[i];
            temp->child_count++;
          }
          for(int i = 0; i < child_count; i++){
                subset[i] = nullptr;
            }
            for(int i = 0; i < data_count; i++){
                data[i] = 0;
            }
            data_count = 0;
            child_count = 0;
          subset[0] = temp;
          child_count++;
          fix_excess(0);
        }
      }
      return false;
    }
template <class Item>
bool set<Item>::loose_insert(const Item& entry)
    {
        bool found = false; 
        int i = 0;
        while (!found)
        {
          if ((data[i] < entry) && (i < data_count))
          {
              i++;
          }
          else if ((entry < data[i]) && !(i > data_count))
          {
              found = true;
          }
          else
          {
              i = data_count;
              found = true;   
          }
        }
        if (data[i] == entry)
        {
          return false;
        }
        else if (is_leaf())
        {
          if (i < data_count)
          {
            for (int j = data_count; j > i; j--)
            {
              data[j] = data[j-1];
            } 
            data[i] = entry;
            data_count++;
          }
          else
          {
            data[i] = entry;
            data_count++;
          }
          return true;
        }
        else
        {
          subset[i]->loose_insert(entry);
          if (subset[i]->data_count > MAXIMUM)
          {
            fix_excess(i);
          }
          return true;
        }
    }
template <class Item>
void set<Item>::fix_excess(size_t i)
{
  bool done = false;
  for (int x = data_count; x > i; x--)
  {
    data[x] = data[x-1];
  }
  data[i] = subset[i]->data[MINIMUM];
  data_count++;
  for (int x = child_count; x > i; x--)
  {
    subset[x] = subset[x-1];
  }
  subset[i] = new set();
  child_count++;
  for (int x = 0; x <= MINIMUM-1; x++)
  {
    subset[i]->data[x] = subset[i+1]->data[x];
    subset[i]->data_count++;
  }
  int it = (MAXIMUM - MINIMUM) + 1;
  while (it != 0)
  {
    for (int x = 0; x < subset[i+1]->data_count; x++)
    {
      subset[i+1]->data[x] = subset[i+1]->data[x+1];
    }
    subset[i+1]->data[it] = 0;
    subset[i+1]->data_count--;
    it--;
  }
}
template <class Item>
void set<Item>::print(int indent) const
{  
  const int EXTRA_INDENTATION = 4;
  cout << setw(indent) << ""; // Print the indentation.
  for (size_t i = 0; i < data_count; ++i)
  cout << data[i] << " ";
  cout << endl;
  for (size_t i = 0; i < child_count; ++i)
  subset[i]->print(indent+EXTRA_INDENTATION);
}
template <class Item>
size_t set<Item>::count(const Item& target) const
{
bool found = false; 
int i = 0;
while (!found)
{
  if ((data[i] < target) && (i < data_count))
  {
    i++;
  }
  else if ((target < data[i]) && !(i > data_count))
  {
    found = true;
  }
  else
  {
   i = data_count;
    found = true;   
  }
}
if (data[i] == target)
{
  return 1;
}
else if (is_leaf())
{
  return 0;
}
else
{
  return subset[i]->count(target);
}
}
template <class Item>
size_t set<Item>::erase(const Item& target)
{
  if (!loose_erase(target))
  {
    return false;
  }
  if ((data_count == 0) && (child_count == 1))
  {
    data_count = subset[0]->data_count;
    child_count = subset[0]->child_count;
    for (int i = 0; i < data_count; i++)
    {
      data[i] = subset[0]->data[i];
    }
    for (int i = 0; i < child_count; i++)
    {
      subset[i] = subset[0]->subset[i];
    }
  }
  return true;
}
template <class Item>
bool set<Item>::loose_erase(const Item& target)
{
bool found = false; 
int i = 0;
while (!found)
{
  if ((data[i] < target) && (i < data_count))
  {
    i++;
  }
  else if ((data[i] > target) && (i < data_count))
  {
    found = true;
  }
  else if (data[i] = target)
  {
    found = true;
  } 
  else
  {
    i = data_count;
    found = true;
  } 
}
if ((is_leaf()) && (data[i] != target))
{
  return false;
}
else if ((is_leaf()) && !(data[i] > target))
{
  for (int h = i; h < data_count; h++)
  {
    data[h] = data[h+1];
  }
  data_count--;
  return true;
}
else if ((!is_leaf()) && (data[i] != target))
{
  subset[i]->loose_erase(target);
  if (subset[i]->data_count < MINIMUM)
  {
    fix_shortage(i);
  }
  return true;
}
else
{
  subset[i]->remove_biggest(data[i]);
  if (subset[i]->data_count < MINIMUM)
  {
    fix_shortage(i);
  }
  return true;
}
}
template <class Item>
void set<Item>::remove_biggest(Item& removed_entry)
{
  if (is_leaf())
  {
    removed_entry = data[data_count-1];
    data_count--;
  }
  else
  {
    subset[child_count-1]->remove_biggest(removed_entry);
    if (subset[child_count]->data_count < MINIMUM)
    {
      fix_shortage(child_count-1);
    }
  }
}
template <class Item>
void set<Item>::fix_shortage(size_t i)
{
  //case1
  if ( (i > 0) && (subset[i-1]->data_count > MINIMUM))
  {
    //a
    for (int a = subset[i]->data_count; a > 0; a--)
    {
      subset[i]->data[a] = subset[i]->data[a-1];
    }
    subset[i]->data[0] = data[i-1];
    subset[i]->data_count++;
    //b
    data[i-1] = subset[i-1]->data[subset[i-1]->data_count-1];
    subset[i-1]->data_count--; 
    //c
    if (!subset[i-1]->is_leaf())
    {
      for (int a = subset[i]->child_count-1; a > 0; a--)
      {
        subset[i]->subset[a] = subset[i]->subset[a-1];
      }
      subset[i]->subset[0] = subset[i-1]->subset[subset[i-1]->child_count-1];
      subset[i]->child_count++;
      subset[i-1]->child_count--; 
    }
  }
  //case2
  else if ((i<data_count) && (subset[i+1]->data_count > MINIMUM))
  {
    subset[i]->data[subset[i]->data_count] = data[i];
    data[i] = subset[i+1]->data[0];
    for (int b = 0; b < subset[i+1]->data_count-1; b++)
    {
      subset[i+1]->data[b] = subset[i+1]->data[b-1];
    } 
    subset[i]->data_count++;
    subset[i+1]->data_count--;

    if (!subset[i+1]->is_leaf())
    {
      subset[i]->subset[subset[i]->child_count] = subset[i+1]->subset[0];
      subset[i]->child_count++;
      for (int b = 0; b < subset[i+1]->child_count - 2; b++)
      {
        subset[i]->subset[b] = subset[i]->subset[b+1];
      }
      subset[i+1]->child_count--;
    }
  }
  //case3
  else if ((i > 0) && (subset[i-1]->data_count == MINIMUM))
  {
    //a
    subset[i-1]->data[subset[i-1]->data_count] = data[i-1];
    for (int c = i-1; c < data_count; c++)
    {
        data[c] = data[c+1]; 
    }
    data_count--;
    subset[i-1]->data_count++;
    for (int c = 0; c < subset[i]->data_count; c++)
    {
      subset[i-1]->data[subset[i-1]->data_count] = subset[i]->data[c];
      subset[i-1]->data_count++;
    }
    subset[i]->data_count = 0;
    if (!subset[i-1]->is_leaf())
    {
      int y = 0;
      for (int c = subset[i-1]->child_count; c < subset[i-1]->data_count; c++)
      {
        subset[i-1]->subset[c] = subset[i]->subset[y];
        subset[i-1]->child_count++;
        y++;
      }
        subset[i]->child_count = 0;
    }
    //c
    for (int c = i; c < child_count; c++)
    {
      subset[c] = subset[c-1];
    }
    child_count--;
  }
  //case4
  else if ((i < child_count) && (subset[i+1]->data_count == MINIMUM))
  {
    //a
    subset[i]->data[subset[i]->data_count] = data[i];
    subset[i]->data_count++;
    for(int d = i; d < data_count; d++)
    {
      data[d] = data[d+1];
    }
    data_count--;
    //b
    for(int d = 0; d <subset[i+1]->data_count; d++)
    {
      subset[i]->data[subset[i]->data_count] = subset[i+1]->data[d];
      subset[i]->data_count++;
    }
    subset[i+1]->data_count = 0;
    if (!subset[i+1]->is_leaf())
    {
      int x = 0;
      for (int d = subset[i]->child_count; d < subset[i]->data_count+1; d++)
      {
      subset[i]->subset[d] = subset[i+1]->subset[x];
      subset[i]->child_count++;
      x++;
      }
      subset[i+1]->child_count = 0;
    }
    //c   
    for (int d = i + 1; d < child_count; d++)
    {
      subset[d] = subset[d+1];
    }
    child_count--;
  }
}
int main()
{
  set<int> tree;
  cout << "-----------" << endl;
  cout << "Insert 5" << endl;
  tree.insert(5);
  tree.print(2);

  cout << "-----------" << endl;
  cout << "Insert 3" << endl;
  tree.insert(3);
  tree.print(2); 

  cout << "-----------" << endl;
  cout << "Insert 7" << endl;
  tree.insert(7);
  tree.print(2);

  cout << "-----------" << endl;
  cout << "Erase 5" << endl;
  tree.erase(5);
  tree.print(2);

  cout << "-----------" << endl;
  cout << "Insert 5" << endl;
  tree.insert(5);
  tree.print(2);

  cout << "-----------" << endl;
  cout << "Insert 1" << endl;
  tree.insert(1);
  tree.print(2);

  cout << "-----------" << endl;
  cout << "Insert 9" << endl;
  tree.insert(9);
  tree.print(2);  

  cout << "-----------" << endl;
  cout << "Insert 8" << endl;
  tree.insert(8);
  tree.print(2);

  cout << "-----------" << endl;
  cout << "Insert 6" << endl;
  tree.insert(6);
  tree.print(2);

  cout << "-----------" << endl;
  cout << "Insert 10" << endl;
  tree.insert(10);
  tree.print(2);

  cout << "-----------" << endl;
  cout << "Erase 10" << endl;
  tree.erase(10);
  tree.print(2);

  cout << "-----------" << endl;
  cout << "Erase 9" << endl;
  tree.erase(9);
  tree.print(2);

  cout << "-----------" << endl;
  cout << "Erase 6" << endl;
  tree.erase(6);
  tree.print(2);

}   