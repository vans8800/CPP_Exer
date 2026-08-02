
#include <stdio.h>

class shape {
public:
  virtual ~shape() {}
};

class circle : public shape {
public:
  circle() { puts("circle()"); }
  ~circle() { puts("~circle()"); }
};

class triangle : public shape {
public:
  triangle() { puts("triangle()"); }
  ~triangle() { puts("~triangle()"); }
};

class result {
public:
  result() { puts("result()"); }
  ~result() { puts("~result()"); }
};

result
process_shape(const shape& shape1,
              const shape& shape2)
{
  puts("process_shape()");
  return result();
}

int main()
{
  puts("main()");
  //process_shape(circle(), triangle());
  // 将返回值绑定到右值引用上，延长了临时对象（result）的生命周期
  result&& r = process_shape(circle(), triangle());
  puts("something else");
}