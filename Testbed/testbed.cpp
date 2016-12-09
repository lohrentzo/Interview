#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <time.h>


// Array x
// Linked List (singly/double) x
// Stack x
// FIFO x
// HashMap x
// Priority Queue
// Heap x
// Tree (Binary [Search]) x

// Binary Search x
// Quicksort
// Merge sort
// Heap sort
// Insertion sort
// Bucket sort


int getHighestBitIndex(int x)
{
  int index = -1;
  for (int i=0; i<sizeof(int)*8; i++)
  {
    int set = (x & (0x1));
    if (set)
    {
      index = i;
    }
    x >>= 1;

  }
  return index;
}

// Linked list
struct Node
{
  Node() : data(-1), next(NULL), prev(NULL) { extra = "_"; }
  int data;
  std::string extra;
  Node* next;
  Node* prev;

  void copyTo(Node** node)
  {
    if (*node == NULL) return;
    (*node)->data = data;
    (*node)->extra = extra;
    (*node)->next = next;
    (*node)->prev = prev;
  }
};

void printList(Node* list)
{
  if (list == NULL)
  {
    printf("NULL list\n");
    return;
  }

  printf("List: ");
  Node* node = list;
  while (node != NULL)
  {
    printf("%p:%d:%s", node, node->data, node->extra.c_str());
    node = node->next;
    if (node != NULL)
    {
      printf(", ");
    }
  }

  printf("\n");
}

Node* addNode(Node** list, int data, const char* extra = NULL)
{
  if (*list == NULL) {
    *list = new Node();
    (*list)->data = data;
    if (extra != NULL) 
      (*list)->extra = extra;
    return *list;
  }
  Node* node = *list;
  Node* end = node;
  while (end->next != NULL)
  {
    end = end->next;
  }

  Node* p = new Node();
  p->data = data;
  if (extra != NULL) 
    p->extra = extra;  
  end->next = p;
  return p;
}

Node* removeNode(Node** list, int data)
{
  if (*list == NULL) return NULL;
  Node* node = *list;

  Node* prev = NULL;
  while (node != NULL && node->data != data)
  {
    prev = node;
    node = node->next;
  }

  Node* next = NULL;
  
  if (node != NULL)
  {
    next = node->next;
    node->next = NULL;
  }
  
  if (prev != NULL)
  {
    prev->next = next;
  }
  else
  {
    *list = node->next;
  }

  if (node != NULL && prev == NULL && next == NULL)
  {
    *list = NULL;
  }

  if (node != NULL) delete node;
  return prev;
}

Node* searchList(Node* list, int data)
{
  if (list == NULL) return NULL;

  if (list->data == data) return list;
  return searchList(list->next, data);
}

void invertListIteration(Node** list)
{
  if (*list == NULL) return;

  Node* end = *list;
  *list = (*list)->next;
  end->next = NULL;

  Node* prev = NULL;
  while ((prev = *list) != NULL)
  {
    *list = prev->next;
    prev->next = end;
    end = prev;
    if (*list == NULL) 
      break;
  }
  *list = prev;
}


class Stack
{
public:
  Stack() : list_(NULL), end_(NULL) {}
  virtual ~Stack() 
  {
    while (list_ != NULL)
    {
      Node* p = list_;
      list_ = list_->next;
      delete p;
    }
  }

  void push(int item) 
  {
    end_ = addNode(&list_, item);
  }
  
  int pop() 
  {
    if (list_ == NULL) return -1;
    int data = end_->data;
    end_ = removeNode(&list_, data);
    //if (end_ != NULL) printList(list_);
    return data;
  }

protected:

  Node* list_;
  Node* end_;
};

void invertListStack(Node** list)
{
  if (*list == NULL) return;

  Stack stack;

  Node* head = *list;
  while (head != NULL)
  {
    stack.push(head->data);
    head = head->next;
  }

  Node* node = *list;
  Node* prev = NULL;
  int data = -1;
  while ((data = stack.pop()) != -1)
  {
    node->data = data;
    if (prev != NULL)
    {
      prev->next = node;
    }
    prev = node;
    node = node->next;
  }
}

class Fifo
{
public:
  Fifo() : list_(NULL), head_(NULL) {}
  virtual ~Fifo() {
    while (head_ != NULL)
    {
      int data= head_->data;
      head_ = head_->next;
      removeNode(&list_, data);
    }
  }

  void enqueue(int data) 
  {
    addNode(&list_, data);
    if (head_ == NULL)
    {
      head_ = list_;
    }
  }

  int dequeue() 
  {
    int data = -1;
    if (head_ != NULL)
    {
        data = head_->data;
        head_ = head_->next;
        removeNode(&list_, data);
    }
    return data;
  }

protected:
  Node* list_;
  Node* head_;
};

typedef Node* LinkedList; 
class HashMap
{
public:
  HashMap(int size) : map_(NULL), size_(-1) 
  {
    map_ = new LinkedList[size];
    memset(map_, 0x0, size*sizeof(LinkedList));
    size_ = size;
  }

  virtual ~HashMap() 
  {
    for (int i=0; i<size_; i++)
    {
      Node* list = map_[i];
      if (list != NULL) 
      {
        Node* node = list;
        Node* next = NULL;
        
        while (node != NULL)
        {
          next = node->next;
          delete node;
          node = next;
        }
      }
    }
    delete[] map_;
  }

  void add(const char* key, int data)
  {
    int index = hash(key);
    Node* node = map_[index];
    addNode(&node, data, key);
    if (map_[index] == NULL)
      map_[index] = node;
  }
  int get(const char* key)
  {
    int index = hash(key);
    Node* node = map_[index];
    Node* next = NULL;
        
    while (node != NULL)
    {
      next = node->next;
      if (node->extra.compare(key) == 0)
      {
        break;
      }
      node = next;
    }
    int data = -1;
    if (node != NULL)
      data = node->data;
    return data;
  }

private:
  int hash(const char* key)
  {
    unsigned int hash = 0;

    int size = strlen(key);
    for (int i=0; i<size; i++)
    {
      hash = (((unsigned int)key[i])^i)*hash + (unsigned int)key[i];
    }
    return hash % size_;
  }

protected:

  LinkedList* map_;
  int size_;
};

class Dictionary
{
public:
  Dictionary() {}
  virtual ~Dictionary() {}

  void insert(int data) {}
  void remove(int data) {}

  int search(int data) { return -1; }

  int max() { return -1; }
  int min() { return -1; }

  int predecessor(int data) { return -1; }
  int successor(int data) { return -1; }

protected:

};

//////////////////////////////////////////////////////////

class BinarySearchTree;
class PriorityQueueElement
{
public:

  PriorityQueueElement(int data, int priority) : data_(data), priority_(priority) {}

  int data_;
  int priority_;
};

class PriorityQueue
{
public:
  
  void insert(int data, int priority);
  PriorityQueueElement* remove(int data);

  PriorityQueueElement* getMax();
  PriorityQueueElement* getMin();

  PriorityQueueElement* removeMax();
  PriorityQueueElement* removeMin();

protected:
  BinarySearchTree* queue_;
};

//////////////////////////////////////////////////////////

class Heap
{
public:

  Heap(int size) : size_(size), count_(0), index_(1)
  {
    array_ = new int[size_];
    array_[0] = 0;
  }
  virtual ~Heap()
  {
    delete[] array_;
  }

  void add(int data) 
  {
    if (index_ == size_) return;
    array_[index_] = data;
    count_++;
    emerge(index_);

    index_++;
  }
  int removemax() { return -1; }
  int removemin() 
  {
    int min = heapmin();
    if (min == -1) return -1;

    array_[1] = array_[count_];
    count_--;
    sink(1);
    return min;

  }
  int heapmin() { return count_ == 0 ? -1 : array_[1]; }
  int heapmax() 
  { 
    int index = getHighestBitIndex(count_);
    index = 1 << index;
    return array_[index]; 
  }

  int height() { 
    // lg(n) --> index of the highest set bit
    return getHighestBitIndex(size_);
  }

  void print()
  {
    for (int i=1; i<=count_; i++)
    {
      int parent = parentIndex(i);
      int left = leftchildIndex(i);
      int right = rightchildIndex(i);

      printf("%i: %i. p: %i, l: %i, r: %i\n", i, array_[i], parent == -1 ? parent : array_[parent], left > count_ ? -1 : array_[left], right > count_ ? -1 : array_[right]);
    }
  }

private:

  void sink(int index) 
  {
    if (index >= count_) return;
    int left = leftchildIndex(index);
    if (left == -1) return;
    int right = rightchildIndex(index);

    int child = -1;
    if (right == -1 || array_[left] < array_[right])
    {
      child = left;
    }
    else if (array_[right] <= array_[left])
    {
      child = right;
    }

    if (array_[child] < array_[index])
    {
      swap(&(array_[child]), &(array_[index]));
      sink(child);
    }

  }
  void emerge(int index) 
  {
    int parent = parentIndex(index);
    if (parent == -1) return; // is root

    if (array_[parent] > array_[index])
    {
      swap(&(array_[index]), &(array_[parent]));
      emerge(parent);
    }
  }
  void swap(int* x, int* y)
  {
    if (x != NULL && y != NULL)
    {
      *x = *x ^ *y;
      *y = *x ^ *y;
      *x = *x ^ *y;
    }
  }

  int parentIndex(int index) 
  {
    if (index == 1) return -1;
    return count_ == 1 ? -1 : index/2; 
  }
  int leftchildIndex(int index) 
  {
    int child = 2*index;
    if (child > count_) return -1;
    return child; 
  }
  int rightchildIndex(int index) 
  {
    int child = (index == 0 ? 0 : 2*index + 1);
    if (child > count_)
      return -1;

    return child;
  }

protected:

  int* array_;
  int size_;
  int count_;
  int index_;
};


class BinaryTree
{

protected:
  int data_;
  BinaryTree* parent;
  BinaryTree* left_;
  BinaryTree* right_;
};


class BinarySearchTree : public BinaryTree
{
  public:
    BinarySearchTree(int data) : data_(data), parent_(NULL), left_(NULL), right_(NULL) {}

    static BinarySearchTree* add(BinarySearchTree* tree, int data)
    {
      BinarySearchTree* node = NULL;
      if (tree == NULL)
      {
        node = new BinarySearchTree(data);
        return node;
      }

      
      if (tree->data_ < data)
      {
        node = add(tree->right_, data);
        if (tree->right_ == NULL)
          tree->right_ = node;
      }
      else 
      {
        node = add(tree->left_, data);
        if (tree->left_ == NULL)
          tree->left_ = node;
      }
      return tree;
    }

    static BinarySearchTree* search(BinarySearchTree* tree, int data)
    {
      if (tree == NULL) return NULL;
      if (tree->data_ == data) return tree;

      BinarySearchTree* result = NULL;

      if (tree->data_ < data)
        return search(tree->right_, data);
      else
        return search(tree->left_, data);
    }
  
    static void print(BinarySearchTree* tree)
    {
      if (tree == NULL) return;
      static int node = 0;
      node++;
      printf("%i: %i\n", node, tree->data_);
      print(tree->left_);
      print(tree->right_);
    }

  public:
    int data_;
    BinarySearchTree* parent_;
    BinarySearchTree* left_;
    BinarySearchTree* right_;

};

class List;
// Graph
class GraphNode
{
  public:
    void visit() { visited_ = true; }

    bool visited_;
    GraphNode** adjacents_;
    int numadjacents_;
};

// DSF
void searchDSF(GraphNode* graph)
{
    if (graph == NULL) return;
    graph->visit();

    for(int i=0; i<graph->numadjacents_; i++)
    {
      GraphNode* adj = graph->adjacents_[i];
      if (!adj->visited_)
      {
        searchDSF(adj);
      }
    }
}

class Queue
{
  public:
    void enqueue(void* item) {}
    void* dequeue() { return NULL; }

  protected:

};

// BSF
void searchBSF(GraphNode* graph)
{
    if (graph == NULL) return;
    graph->visit();
    Queue queue;
    queue.enqueue(graph);

    GraphNode* node = NULL;
    while ((node = (GraphNode*)queue.dequeue()) != NULL)
    {
        for(int i=0; i<node->numadjacents_; i++)
        {
            GraphNode* adj = node->adjacents_[i];
            if (!adj->visited_)
            {
                adj->visit();
                queue.enqueue(adj);
            }
        }

    }
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Quicksort

void quicksort(int** array_, int size)
{
  if (size < 2) return;
  if (*array_ == NULL) return;

  int pivot = -1;
  int* fwrd = *array_;
  int* bkwd = *array_+size-1;

  srand(time(NULL));
  pivot = (*array_)[rand() % (size-1)];

  while (fwrd < bkwd)
  {
    if (*bkwd < pivot || *fwrd > pivot)
    {
      *fwrd = *fwrd ^ *bkwd;
      *bkwd = *fwrd ^ *bkwd;
      *fwrd = *fwrd ^ *bkwd;
    }
    if (*fwrd <= pivot) fwrd++;
    if (*bkwd > pivot) bkwd--;
  }

  int pivotindex = 0;
  for (pivotindex=0; pivotindex<size; pivotindex++)
  {
    if ((*array_)[pivotindex] == pivot)
      break;
  }

  quicksort(array_, pivotindex);
  int* arr = (*array_)+pivotindex+1;
  quicksort(&arr, size-pivotindex-1);
}


// Merge sort
#define MIN(x,y) return ((x)<(Y) ? (x) : (y))

void merge(int* arr, int start, int median, int size, int** dest)
{
 int i1 = start;
 int i2 = median;
 int endindex = size+start-1;

 int* temp = new int[size];

 for (int i=start; i<=endindex; i++)
 {
   if (i1 < median && (i2 > endindex || arr[i1] <= arr[i2]))
   {
     temp[i-start] = arr[i1];
     i1++;
   }
   else 
   {
     temp[i-start] = arr[i2];
     i2++;
   }
 }

 for (int i = 0; i<size; i++)
 {
   (*dest)[start+i] = temp[i];
   printf("%i ", (*dest)[i]);
 }
 printf("\n");
 delete[] temp;
}


void partitionThenMerge(int* arr, int start, int size, int** dest)
{
  if (size < 2) return;
  int median = start+size/2;

  int end = start+size-1;
  partitionThenMerge(arr, start, median-start, dest);
  partitionThenMerge(arr, median, size-median+start, dest);
  merge(arr, start, median, size, dest);
}


void mergesort(int** array_, int size)
{
  if (*array_ == NULL) return;

  partitionThenMerge(*array_, 0, size, array_);

}

// Heap sort x

// Insertion sort 
void insertionsort(int** array_, int size)
{
  int* temp = new int[size];

  for (int i=0; i<size; i++)
  {
    int item = (*array_)[i];
    int index = 0;
    for (index=0; index<i; index++)
    {
      if (i == 0) {
        temp[0] = item;
        break;
      }
      if (item < temp[index])
      {
        for (int j=i; j>index; j--)
        {
          temp[j] = temp[j-1];
        }
        break;
      }
    }
    temp[index] = item;    
  }
  for (int i=0; i<size; i++)
  {
    (*array_)[i] = temp[i];
  }
}


// Bucket sort x

// Count ways: steps
int countways(int steps)
{
  if (steps<0) return 0;
  if (steps == 0) return 1;
  return countways(steps-1) + countways(steps-2) + countways(steps-3);
}

unsigned int countwaysDCP(int steps, unsigned int** map)
{
  if (*map == NULL)
  {
      *map = new unsigned int[steps*steps];
      memset(*map, 0x0, steps*steps*sizeof(unsigned int));
  }
  if (steps < 0) return 0;
  if (steps == 0) return 1;
  unsigned int partial = (*map)[steps];
  if (partial != 0)
      return partial;
  partial = countwaysDCP(steps-1, map) + countwaysDCP(steps-2, map) + countwaysDCP(steps-3, map);
  (*map)[steps] = partial;
  return partial;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int sum(int a, int b)
{
  if (b == 0) return a;
  int sumnocarry = a^b;
  int carry = (a&b) << 1;
  return sum(sumnocarry, carry);
}


int main(int argc, char* argv[])
{
  /*
  const char* keys[] = { "cabac", "bacca", "lorenzo", "abba", "aaaaaaa", "bbbb" };

  int keynum = sizeof(keys)/sizeof(*keys);
  for (int k=0; k<keynum; k++)
  {
    const char* key = keys[k];
    std::cout << "key: " << key;
    char c = 0x00;
    unsigned short total = 0;
    unsigned int hash = -1;
    for (int i=0; i<strlen(key); i++)
    {
      c = key[i];
      total = ((short)c ^ (short)i)*total + (short)c;
    }
    hash = total % 99;
    std::cout << " - total: " << total;
    std::cout << " - hash: " << hash << std::endl;
  }
  */

  /*
  int i1 = getHighestBitIndex(10);
  int i2 = getHighestBitIndex(15);
  int i3 = getHighestBitIndex(8);

  int i4 = 1 << i1;
  */

    int a = 73;
    int b = 199;
    printf("%i + %i = %i\n", a, b, sum(a,b));


    int steps = 20;
    unsigned int* map = NULL;
    printf("Count Ways: %u\n", countwaysDCP(steps, &map));


  printf("Array\n");

  int arraySize = 11;
  int* Array = new int[arraySize];
  
  srand (time(NULL));
  for (int i=0; i<arraySize; i++)
  {
    Array[i] = rand() ;//% (arraySize);
    printf("%i: %i\n", i, Array[i]);
  }
  

  printf("Sorting\n");

  //quicksort(&Array, arraySize);
  //mergesort(&Array, arraySize);
  insertionsort(&Array, arraySize);

  for (int i=0; i<arraySize; i++)
  {
    printf("%i: %i\n", i, Array[i]);
  }

  printf("Linked List\n");
  // create a linked list
  Node* list = NULL;
  for (int i=0; i<arraySize; i++)
  {
    addNode(&list, Array[i]);
  }

  printList(list);

  {
    printf("Linked List::search\n");
    srand (time(NULL));
    int index = rand() % (arraySize*10);
    int data = Array[index];
    Node* node = searchList(list, data);
    printf("index: %i, data: %i --> Node: %p:%i\n", index, data, node, (node == NULL ? -1 : node->data));
  }
  
  printf("Linked List::invert::iteration\n");
  invertListIteration(&list);
  printList(list);

  printf("Linked List::invert::stack\n");
  invertListStack(&list);
  printList(list);

  printf("Fifo\n");
  Fifo* fifo = new Fifo();

  Node* node = list;
  while (node != NULL)
  {
    fifo->enqueue(node->data);
    node = node->next;
  }

  printf("Fifo");
  int data = -1;
  bool first = true;
  while ((data = fifo->dequeue()) != -1)
  {
    printf("%c %i", first ? ':' : ',', data);
    first = false;
  }
  printf("\n");
  delete fifo;

  {
    printf("Hashmap\n");
    const char* keys[] = { "cabac", "bacca", "lorenzo", "abba", "aaaaaaa", "bbbb" };
    int numkeys = sizeof(keys)/sizeof(*keys);

    HashMap map(100);

    srand (time(NULL));
    for (int i=0; i<numkeys; i++)
    {
      const char* key = keys[i];
      map.add(key, Array[i]);
    }

    for (int i=0; i<numkeys; i++)
    {
      const char* key = keys[i];
      int data = map.get(key);
      printf("key: %s - data: %d\n", key, data);
    }
  }

  {
    printf("Heap\n");
    Heap heap(arraySize);


    for (int i=0; i<arraySize; i++)
    {
      heap.add(Array[i]);
    }

    heap.print();

    int data = -1;
    while ((data = heap.removemin()) != -1)
    {
      printf("%i\n", data);
    }
  }

  {
    printf("BinarySearchTree\n");
    BinarySearchTree* tree = NULL;

    for (int i=0; i<arraySize; i++)
    {
      tree = BinarySearchTree::add(tree, Array[i]);
    }
    BinarySearchTree::print(tree);
  }

  printf("The end\n");
  delete[] Array;
  getchar();
  return 0;
}
