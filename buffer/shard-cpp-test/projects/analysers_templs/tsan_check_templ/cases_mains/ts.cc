// https://www.mail-archive.com/gcc-patches@gcc.gnu.org/msg99353.html

#include <iostream>

#include <pthread.h>

int v;

int
foo (int x)
{
  if (x < 99)
    throw x;
  v++;
  return x;
}

void *
tf (void *)
{
  for (int i = 0; i < 100; i++)
    try { foo (i); } catch (int) {
      std::cout  << "catch" << std::endl;  // looks work in TSan
    }
  return NULL;
}

int
main ()
{
  pthread_t th;
  if (pthread_create (&th, NULL, tf, NULL))
    return 0;
  v++;
  pthread_join (th, NULL);
  return 0;
}