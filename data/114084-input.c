/* PR middle-end/114084 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -pedantic-errors" } */

typedef unsigned _BitInt(31) T;
T a, b;

void foo(void) { b = (T)((a | (T)-1) >> 1 | (a | (T)5) << 4); }