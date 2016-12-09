#include "lorenzo.h"

#include <iostream>
#include <string.h>

#ifndef NULL
#define NULL 0x0
#endif

class ReferencedObject
{
  public:
    ReferencedObject() : refCount_(1) {}

    void addRef() { refCount_++; } 
    int release() 
    {
      int refCount = --refCount_;
      if (refCount == 0)
      {
        delete this;
      }
      return refCount;
    }

  protected:
    virtual ~ReferencedObject() 
    { 
      std::cout << "ReferencedObject dtor" << std::endl; 
    }


  private:
    int refCount_;
};

#define ARRAY_PAGE_SIZE 3

template <typename T>
class ArrayInverter : public ReferencedObject
{
  public:
  ArrayInverter() : array_(NULL), size_(0), count_(0) 
  {
    realloc();
  }

  void add(T item) 
  {
    if (count_ == size_-1)
    {
      realloc();
    }
    array_[count_] = item;
    count_++;
  }

  void reverse() 
  {
    T* p1 = array_;
    T* p2 = array_ + (count_-1);

    while (p1 < p2) 
    {
      *p1 = *p1 ^ *p2;
      *p2 = *p1 ^ *p2;
      *p1 = *p1 ^ *p2;

      p1++;
      p2--;
    }
  }
  
  void printout() 
  {
    std::cout << "Array: { ";
    for (int i = 0; i<count_; i++)
    {
      std::cout << array_[i] << (i == count_-1 ? " }\n" : ", "); 
    }
  }

  protected:
  virtual ~ArrayInverter() 
  {
    if (array_ != NULL)
    {
      delete[] array_; // think about using free for use of realloc
    }
  }

  void realloc()
  {
    size_ += ARRAY_PAGE_SIZE;
    T* temp = new T[size_];
    memset(temp, 0x0, size_*sizeof(T));
    if (array_ != NULL) {
      memcpy(temp, array_, count_ * sizeof(T));
    }
    T* disposable = array_;
    array_ = temp;
    if (disposable != NULL)
    {
      delete[] disposable;
    }
  }

  private:

    T* array_;
    int size_;
    int count_;
};

/*
void* malloc_aligned(int size, int alignement)
{
  int offset = alignement - 1 + sizeof(void*);
  void* p = (void*)malloc(size+offset);
  void** q = (void**)(((int)(p)+offset) & ~(offset));
  q[-1] = p;
  return q;
}

void free_aligned(void* pointer)
{
  void* p = ((void**)pointer)[-1];
  free(p);
}
*/

#include <string>
#include <sstream>
void compressString(const std::string& instring, std::string& compressedString)
{
  unsigned int len = instring.length();

  const char* pinstring = instring.c_str();
  std::stringstream strstream;


  int count = 0;
  char c = 0x00;
 
  for (unsigned int i=0; i<len; i++)
  {
    if (pinstring[i] == c)
    {
      count++;
    }
    else 
    {
      if (count > 0)
      {
        strstream << c;
        strstream << count;
      }
      c = pinstring[i];
      count = 1;
    }
  }
  strstream << c;
  strstream << count;


  compressedString = strstream.str();

  if (compressedString.length() > len)
  {
    compressedString = instring;
  }
}


// rotate an image
//
// ...
#include <stdlib.h>
void fillBuffer(char** buffer, int& size)
{
  size = 356*288*4;
  char* buf = (char*)malloc(size);
  memset(buf, 0x0, size);
  *buffer = buf;
}

int main(int argc, char* argv[])
{
  /*
  ReferencedObject* refObj = new ReferencedObject();

  refObj->addRef();
  refObj->release();
  refObj->release();
  */

  /*
  ArrayInverter<int>* intArray = new ArrayInverter<int>();
  intArray->add(1);
  intArray->add(2);
  intArray->add(3);
  intArray->add(4);
  intArray->add(5);
  intArray->printout();
  intArray->reverse();
  intArray->printout();
  intArray->release();

  ArrayInverter<char>* charArray = new ArrayInverter<char>();
  charArray->add('a');
  charArray->add('b');
  charArray->add('c');
  charArray->add('d');
  charArray->add('e');
  charArray->printout();
  charArray->reverse();
  charArray->printout();
  charArray->release();
  */


  //std::cout << 6 % 7 << std::endl;
  /*
  int maxsize = 7;
  for (int i=0; i<1000; i++)
  {
    std::cout << i % maxsize << std::endl;
  }
*/

/*
  std::string stringA = "aabbccddeeff";
  std::string stringB;

  compressString(stringA, stringB);
  std::cout << "Input: " << stringA << " - Compressed: " << stringB << std::endl;
*/

  char* buffer = NULL;
  int size = 0;

  fillBuffer(&buffer, size);
  strncpy(buffer, "hello world!", strlen("hello world!"));

  std::cout << buffer << std::endl;


  return 0;
}
