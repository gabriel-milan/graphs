/*
 * Adapted from
 * https://www.geeksforgeeks.org/binary-heap/
 */
#include <iostream>
#include <climits>
using namespace std;

struct MinHeapNode
{
  int v;
  float dist;
};

struct MinHeapNode *newMinHeapNode(int v, float dist)
{
  struct MinHeapNode *minHeapNode =
      (struct MinHeapNode *)
          malloc(sizeof(struct MinHeapNode));
  minHeapNode->v = v;
  minHeapNode->dist = dist;
  return minHeapNode;
}

void swapNodes(struct MinHeapNode **x, struct MinHeapNode **y)
{
  struct MinHeapNode *t = *x;
  *x = *y;
  *y = t;
}

class MinHeap
{
public:
  MinHeapNode **harr;
  int *pos;
  int capacity;
  int heap_size;

  MinHeap(int capacity)
  {
    this->heap_size = 0;
    this->capacity = capacity;
    this->pos = (int *)malloc(capacity * sizeof(int));
    this->harr = (struct MinHeapNode **)malloc(capacity * sizeof(struct MinHeapNode *));
  }

  void minHeapify(int idx)
  {
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < this->heap_size && this->harr[left]->dist < this->harr[smallest]->dist)
      smallest = left;

    if (right < this->heap_size && this->harr[right]->dist < this->harr[smallest]->dist)
      smallest = right;

    if (smallest != idx)
    {
      MinHeapNode *smallestNode = this->harr[smallest];
      MinHeapNode *idxNode = this->harr[idx];
      this->pos[smallestNode->v] = idx;
      this->pos[idxNode->v] = smallest;
      swapNodes(&this->harr[smallest], &this->harr[idx]);
      this->minHeapify(smallest);
    }
  }

  bool isEmpty()
  {
    return this->heap_size == 0;
  }

  struct MinHeapNode *extractMin()
  {
    if (this->isEmpty())
      return NULL;
    struct MinHeapNode *root = this->harr[0];
    struct MinHeapNode *lastNode = this->harr[this->heap_size - 1];
    this->harr[0] = lastNode;
    this->pos[root->v] = this->heap_size - 1;
    this->pos[lastNode->v] = 0;
    this->heap_size--;
    this->minHeapify(0);
    return root;
  }

  void decreaseKey(int v, float dist)
  {
    int i = this->pos[v];
    this->harr[i]->dist = dist;
    while (i && this->harr[i]->dist < this->harr[(i - 1) / 2]->dist)
    {
      this->pos[this->harr[i]->v] = (i - 1) / 2;
      this->pos[this->harr[(i - 1) / 2]->v] = i;
      swapNodes(&this->harr[i], &this->harr[(i - 1) / 2]);
      i = (i - 1) / 2;
    }
  }

  bool isInMinHeap(int v)
  {
    if (this->pos[v] < this->heap_size)
      return true;
    return false;
  }
};