#include <pthread.h>
#include <stdio.h>

int main() {
    int scope, detachstate, inheritsched, policy;
    size_t stacksize;
    void* stackptr;

    pthread_attr_t attrs;
    pthread_getattr_default_np(&attrs);

    pthread_attr_getschedpolicy(&attrs, &policy);
    pthread_attr_getinheritsched(&attrs, &inheritsched);
    pthread_attr_getstack(&attrs, &stackptr, &stacksize);
    pthread_attr_getdetachstate(&attrs, &detachstate);
    pthread_attr_getscope(&attrs, &scope);

    if(scope == PTHREAD_SCOPE_SYSTEM) {
        printf("Scope: SYSTEM\n");
    } else if (scope == PTHREAD_SCOPE_PROCESS) {
        printf("Scope: PROCESS\n");
    } else {
        printf("Scope: ERROR\n");
    }

    if (detachstate == PTHREAD_CREATE_DETACHED) {
        printf("detachstate: DETACHED\n");
    } else if(detachstate == PTHREAD_CREATE_JOINABLE) {
        printf("detachstate: JOINABLE\n");
    } else {
        printf("detachstate: ERROR\n");
    }

    printf("Stack address: %p\n", stackptr);
    printf("Stack size: %lu\n", stacksize);

    
    if(inheritsched == PTHREAD_INHERIT_SCHED) {
        printf("Inherit sheduler: INHERIT\n");
    } else if (inheritsched == PTHREAD_EXPLICIT_SCHED) {
        printf("Inherit sheduler: EXPLICIT\n");
    } else {
        printf("Inherit sheduler: ERROR\n");
    }

    
    if(policy == SCHED_FIFO) {
        printf("policy: SCHED_FIFO\n");
    } else if (policy == SCHED_RR) {
        printf("policy: SCHED_RR\n");
    } else if (policy == SCHED_OTHER) {
        printf("policy: SCHED_OTHER\n");
    }
    else {
        printf("policy: ERROR\n");
    }
}
