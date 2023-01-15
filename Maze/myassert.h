// define to use custom assert, which prompts debugger to halt execution
//#define I_WOULD_RATHER_SEE_THE_STACK_TRACE_AND_DEBUGGER

#ifndef I_WOULD_RATHER_SEE_THE_STACK_TRACE_AND_DEBUGGER
#include <assert.h>
#else
#define assert(expression) \
{ \
  if(!(expression)) { \
    cout << "Assertion failed: " #expression " at " __FILE__ ", line "<< __LINE__ <<endl; \
    *(int*)0=0; /* attempt to dereference NULL --tells debugger to halt the program! */ \
  } \
}
#endif
