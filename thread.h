#ifndef thread_h
#define thread_h

#include <stdio.h>
#include <pthread.h>
#include <setjmp.h>
#include <map>
#include <exception>


/*---- Struct: thread ---------------------------------------------------------*  
 *  Stores relevant information on each thread which is created.*/
struct thread {
    jmp_buf env;
    char *stack;
    pthread_t id;
    int status;
};

/*--- Namespace: Anonymous---------------------------------------------------------
 *  Functions and objects for the management of threads in this library.
 *  Functionally TCB. Only available on in this translation unit. */
namespace {

    //Map of threads by thread ID.
    std::map<pthread_t, thread> pool;

    //Value for next thread.
    pthread_t id_counter = 0;

    //Iterator for current thread;
    std::map<pthread_t, thread>::iterator current_it;
    
    //Define some constants.
    const int RUNNING = 1;
    const int READY = 0;
    const int EXITED = 2;
    const int STACK_MAX = 32676;


    /*--- Helper function: pthread_switch -----------------------------------------------
     * This shall schedule the next ready thread from the thread pool to continue
     * execution. Threads are cycled through via 50ms round robin. */
    void thread_switch() __attribute__((noreturn));
    void thread_switch() {

        //Switch current thread off from running.
        if(current_it->second.status == RUNNING) 
            current_it->second.status == READY;

        //Find next ready thread.
        do {
            current_it++;
            if(current_it == pool.end()) current_it = pool.begin();
        } while (current_it->second.status != READY);

        //Update status of new thread to running.
        current_it->second.status = RUNNING;

        //Validate that there is only one thread running.
        int running_thrds = 0;
        for (auto& i : pool) if(i.second.status == RUNNING) running_thrds++;
        if (running_thrds > 1) throw std::runtime_error("Switcher detected multiple threads with status=RUNNING\n");

        //Jump to selected thread.
        printf("Switching to [%i] of %i:\n", current_it->second.id, id_counter);
        printf("jmp_buf PC set to %x\n", current_it->second.env[0].__jmpbuf[5]);
        longjmp(current_it->second.env, 1);

        exit(0);
    }

    /*--- Helper Function: pointer mangler --------------------------------------------*/
    static int ptr_mangle(int p) {
    unsigned int ret;
    asm(" movl %1, %%eax;\n"
        " xorl %%gs:0x18, %%eax;"
        " roll $0x9, %%eax;"
        " movl %%eax, %0;"
    : "=r"(ret)
    : "r"(p)
    : "%eax"
    );
    return ret;
    }

}


/*--- Thread API: pthread_create  -------------------------------------------------
 *  This function shall create a new thread in the calling process. Throws excp.*/
int pthread_create(pthread_t *restrict_thread, const pthread_attr_t *restrict_attr, 
    void *(*start_routine)(void*), void *restric_arg);

/*--- Thread API: pthread_self ----------------------------------------------------
 *  This shall return the ID of the calling thread. */
pthread_t pthread_self(void);

/*--- Thread API: pthread_exit -------------------------------------------
 *  This function shall terminate the calling thread and perform cleanup.
 *  This function does not return.*/
void pthread_exit(void *value_ptr) __attribute__((noreturn));


#endif
