#include "../EventSystem/EventQueue.h"
#include "../Misc/Actor.h"
#include <stdio.h>

typedef struct test
{
  int n;
} test;

int main()
{
  test a, b, c;
  a.n = 0;
  b.n = 1;
  c.n = 2;

  printf("a: %p\n", &a);
  printf("a.n: %d\n", a.n);
  printf("b: %p\n", &b);
  printf("b.n: %d\n", b.n);
  printf("c: %p\n", &c);
  printf("c.n: %d\n", c.n);

  set_actor(&a);
  {
    void* ac1 = get_actor();
    printf("ac_1: %p\n", ac1);
    printf("ac_1.n: %d\n", ((test*)ac1)->n);
    set_actor(&b);
    {
      void* ac2 = get_actor();
      printf("ac_2: %p\n", ac2);
      printf("ac_2.n: %d\n", ((test*)ac2)->n);
      set_actor(&c);
      {
        void* ac3 = get_actor();
        printf("ac_3: %p\n", ac3);
        printf("ac_3.n: %d\n", ((test*)ac3)->n);
      }
      release_actor();
      void* ac2_ = get_actor();
      printf("ac_2_: %p\n", ac2_);
      printf("ac_2_.n: %d\n", ((test*)ac2_)->n);
    }
    release_actor();
    void* ac1_ = get_actor();
    printf("ac_1_: %p\n", ac1_);
    printf("ac_1_.n: %d\n", ((test*)ac1_)->n);
  }

  return 0;
}