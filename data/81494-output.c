__attribute__((target_version("dotprod+lse"))) int foo(void) { return 1; }

__attribute__((target_version("default"))) int foo(void) { return 0; }

#ifdef HAS_CALLER
int caller(void) { return foo(); }
#endif