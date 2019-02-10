#include "thread.h"

/*--- Definition: pthread_create ------------------------------------------------*/
int pthread_create( pthread_t *restrict_thread, const pthread_attr_t *restrict_attr, 
void *(*start_routine)(void*), void *restric_arg) {


    //Hold return valule of setjump for main. Set to indicate we are in main thread.
    int made_jmp = 0;

    //Create main thread if one does not exit.
    if (id_counter == 0) {
        thread main_thread;
        made_jmp = setjmp(main_thread.env);

        //If not jumping into main thread. Continue setting up main thread.
        if(!made_jmp) {
            main_thread.id = id_counter;
            id_counter++;
            main_thread.status = READY;
            pool.emplace(main_thread.id, main_thread);
            current_it = pool.find(main_thread.id);
            if(current_it == pool.end())
                throw std::runtime_error("Main thread should have been added to pool on first create call.\n");
            thread_switch();
        }

    }

    //If we are in main thread, create the new thread we want.
    if (made_jmp) {
	printf("Attempting creation of new thread.\n");
	thread new_thread;
        new_thread.stack = new char[STACK_MAX];
        intptr_t *arg = (intptr_t*)&new_thread.stack[STACK_MAX - 4];
        intptr_t *ret = (intptr_t*)&new_thread.stack[STACK_MAX - 8];
        intptr_t bp = (intptr_t)&new_thread.stack[STACK_MAX - 12];
        *arg = (intptr_t)restric_arg;
        *ret = (intptr_t)pthread_exit;
	printf("Stack allocated.\n");
	int i = setjmp(new_thread.env);
        new_thread.env[0].__jmpbuf[3] = ptr_mangle(bp);
        new_thread.env[0].__jmpbuf[4] = ptr_mangle(bp);
        new_thread.env[0].__jmpbuf[5] = ptr_mangle((intptr_t)start_routine);
        new_thread.id = id_counter;
        id_counter++;
        new_thread.status = READY;
        pool.emplace(new_thread.id, new_thread);
        thread_switch();
    }
    
    return 0;
}




