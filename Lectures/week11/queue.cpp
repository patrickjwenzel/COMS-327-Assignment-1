#include <iostream>

using namespace std;

template <class T>
T mymin(T first, T second)
{
  return first < second ? first : second;
}

//This entire class definition must be available wherever you wish to 
//instantiate it.  Usually these are written entirely in headers.
template <class T>
class queue {
private:
  class queue_node {
  public:
    queue_node(T t) : next(0), prev(0), data(t)
    {
    }
    queue_node(T t, queue_node *prev, queue_node *next) : next(next),
                                                          prev(prev),
                                                          data(t)
    {
    }
    queue_node *next, *prev;
    T data;
  };
  queue_node *head, *tail;
  int data_structure_size;

public:
  queue() : head(0), tail(0), data_structure_size(0)
  {
  }

  ~queue()
  {
    queue_node *q;

    for (q = head; q; q = head) {
      head = head->next;
      delete q;
    }
  }

  void enqueue(T t)
  {
    queue_node *tmp = new queue_node(t, tail, 0);
    if (tail) {
      tail->next = tmp;
    } else {
      head = tmp;
    }
    tail = tmp;
    data_structure_size++;
  }

  T dequeue()
  {
    if (!head) {
      throw "Attempt to dequeue from an empty queue.";
    }

    T t = head->data;
    queue_node *q = head;

    head = head->next;
    if (!head) {
      tail = NULL;
    } else {
      head->prev = NULL;
    }
    delete q;

    data_structure_size--;

    return t;
  }

  T front()
  {
    if (!head) {
      throw "Attempt to front an empty queue.";
    }

    return head->data;
  }

  int size()
  {
    return data_structure_size;
  }
};

int main(int argc, char *argv[])
{
  queue<int> q;
  int i;

  for (i = 0; i < 10; i++) {
    q.enqueue(i);
  }

  try {
    for (i = 0; i < 100; i++) {
      cout << q.dequeue() << " ";
      cout << q.size() << endl;
    }
  }
  catch(char const *s) {
    cout << s << endl;
  }

  q.enqueue(0);

  queue<string> qs;
  qs.enqueue("Hello");
  qs.enqueue("world");

  cout << qs.dequeue() << " ";
  cout << qs.front() << endl;

  /*
  queue<queue<int> > qqi; //Double delete; needs copy constructor
  qqi.enqueue(q);
  */

  cout << mymin(1, 2) << " " << mymin(3.14, 1 .414) << " " << mymin("hello", "world");// << " " << mymin(string("hello"), string("world")) << endl;

  return 0;
}
