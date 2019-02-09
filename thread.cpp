#include "thread.h"

/*--- Definition: pthread_create ------------------------------------------------*/
int pthread_create( pthread_t *restrict_thread, const pthread_attr_t *restrict_attr, 
void *(*start_routine)(void*), void *restric_arg) {

    //Variable to hold current env state.
    thread new_thread;

    //Hold return valule of setjump for main. Set to indicate we are in main thread.
    int made_jmp = (int)false;

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
        
        
    }
    
    return 0;
}




