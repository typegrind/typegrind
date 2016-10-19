
template<typename T>
class ClName {
  T* ptr1;
  double* ptr2;
public:
  ClName()
  : ptr1(new T[100])
  {
    ptr2 = new double(3.3);
  }

  virtual ~ClName()
  {
    delete[] ptr1;
    delete ptr2;
  }

};

template<>
class ClName<char> {
public:
  char* cc;

  ClName()
  : cc(new char[1337]){}

  ~ClName()
  {
    delete[] cc;
  }
};

int main(void) {
  int* pT = new int(53);
  delete pT;

  pT = reinterpret_cast<int*>(::operator new[](200));

  ClName<short> cl1;
  { ClName<int> cl2; }

  ClName<char> cl_spec;

  ::operator delete[](pT);

  return 0;
}

