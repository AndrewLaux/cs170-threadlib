#include "thread.h"

/*--- Definition: pthread_create ------------------------------------------------*/
int pthread_create( pthread_t *restrict_thread, const pthread_attr_t *restrict_attr, 
void *(*start_routine)(void*), void *restric_arg) {

	//Pause alarm for this function.
	int rem_time = ualarm(0,0);

    //Hold return valule of setjump for main. Set to indicate we are in main thread.
    int made_jmp = 0;

    //Create main thread if one does not exit.
    if (id_counter == 0) {
        thread main_thread;
        made_jmp = setjmp(main_thread.env);
	if(made_jmp) rem_time = ualarm(0,0);

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
    if (1) {
        thread new_thread;
        new_thread.stack = new char[STACK_MAX];
        intptr_t *args = (intptr_t*)&new_thread.stack[STACK_MAX - 4];
        intptr_t *ret = (intptr_t*)&new_thread.stack[STACK_MAX - 8];
        intptr_t sp = (intptr_t)&new_thread.stack[STACK_MAX - 8];
        *args = (intptr_t)restric_arg;
        *ret = (intptr_t)pthread_exit;
        new_thread.env[0].__jmpbuf[3] = ptr_mangle(0xffff0000);
        new_thread.env[0].__jmpbuf[4] = ptr_mangle(sp);
        new_thread.env[0].__jmpbuf[5] = ptr_mangle((intptr_t)start_routine);
        new_thread.id = id_counter;
        *restrict_thread = id_counter;
        id_counter++;
        new_thread.status = READY;
        pool.emplace(new_thread.id, new_thread);
    }

    //Restor paused alarm.
    ualarm(rem_time, 0);
    return 0;
}

/*--- Definition: pthread_exit ------------------------------------------------*/
void pthread_exit(void *value_ptr) {
    delete[] current_it->second.stack;
    current_it->second.status = EXITED;
    thread_switch();
    exit(0);
}

/*--- Definition: pthread_self ------------------------------------------------*/
pthread_t pthread_self(void) {
    return current_it->second.id;
}
