A user library for thread implementation.
Written by: ANDREW LAUX 5475900

To compile simply run command: `make`

#Method:

###Thread creation
I created an anonymous namespace in thread.h which contains helper functions and data for managing the creation and execution of threads. When `pthread_create(...)` gets called if no threads exits it creates a thread for main and continues on that thread for the remainder of the program. Once the main thread with id [0] has been created or if it already existed then a stack is created for the new thread which will execute the function pointed to by `start_routine`.

I used the following method for creating a virtual stack on the heap:

```
        thread new_thread;
        new_thread.stack = new char[STACK_MAX];
        intptr_t *args = (intptr_t*)&new_thread.stack[STACK_MAX - 4];
        intptr_t *ret = (intptr_t*)&new_thread.stack[STACK_MAX - 8];
        intptr_t sp = (intptr_t)&new_thread.stack[STACK_MAX - 8];
        *args = (intptr_t)restric_arg;
        *ret = (intptr_t)pthread_exit;
        new_thread.env[0].__jmpbuf[4] = ptr_mangle(sp);
        new_thread.env[0].__jmpbuf[5] = ptr_mangle((intptr_t)start_routine);
        new_thread.id = id_counter;
        *restrict_thread = id_counter;
        id_counter++;
        new_thread.status = READY;
        pool.emplace(new_thread.id, new_thread);
```

Since the calling convetions for x86 require that a function first `push $ebp` then `move $ebp, $esp` I set the jmpbuf stack pointer to the return address I pushed on to the virtual stack. That way the stack looks like

`max: <args>`
`max-4: <return addr>`
`max-8: <jmpbuf BP>` <- ebp
`locals` <- $esp 

### Scheduling

I used a function called `thread_switch()` that looks for the next thread object with status `READY`. Then sets a new alarm and longjumps to that thread. The program enters into this function from three different points.

1) The SIGALRM is caught by the alarm handler function. The handler then saves the env to the thread's env then calls `thread_switch()`.
2) The thread completes and returns to pthread_exit. The thread's status is updated to `EXITED` and the virtual stack is freed before the function calls `thread_switch()`.
3) When the main thread (id = 0) is created there is a hardcoded call to `thread_swtich()`.

The `SIGALRM` is set by `ualarm` to be sent every 50ms.


### Problems
I had the issue with `basic_4.c` that was discussed on piazza. The program hangs up and becomes unresponsive about 50% percent of the times I try to run it. By tracing through some debug statements I found that the program was locked in in the `bbq_party` while loop. An alarm should have been triggered and sent to the handler where main would resume an `is_welcome` could be updated but that signal was not being caught.

Things i tried to resolve this:
- making sa_mask empty before the sigaction was registered.
- using memset to zero out unintilized variables in the SA to insure that possible garbage values don't cause problems.
- setting ualarm to fire off mutliple times on an interval instead of once.
- creating the sig action once instead of multiple times at each thread switch.