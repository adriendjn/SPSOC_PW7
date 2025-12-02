#include <defs.h>
#include <taskman/semaphore.h>

#include <implement_me.h>

__global static struct taskman_handler semaphore_handler;


struct wait_data {
    // to be passed as an argument.
    // what kind of data do we need to put here
    // so that the semaphore works correctly?
    struct taskman_semaphore* semaphore;
    int up; //1 =

};

static int impl(struct wait_data* wait_data) {
    // implement the semaphore logic here
    // do not forget to check the header file

    die_if_not(wait_data != NULL);
    struct taskman_semaphore* semaphore = wait_data->semaphore;
    die_if_not(semaphore != NULL);

    if (wait_data->up){
        if (semaphore->count < semaphore->max) {
            semaphore->count++;
            return 1;
        } return 0;
    } else {
        if (semaphore->count > 0){
            semaphore->count--;
            return 1;
        } return 0;
    }
}

static int on_wait(struct taskman_handler* handler, void* stack, void* arg) {
    UNUSED(handler);
    UNUSED(stack);

    return impl((struct wait_data*)arg);
}

static int can_resume(struct taskman_handler* handler, void* stack, void* arg) {
    UNUSED(handler);
    UNUSED(stack);

    return impl((struct wait_data*)arg);
}

static void loop(struct taskman_handler* handler) {
    UNUSED(handler);
}

/* END SOLUTION */

void taskman_semaphore_glinit() {
    semaphore_handler.name = "semaphore";
    semaphore_handler.on_wait = &on_wait;
    semaphore_handler.can_resume = &can_resume;
    semaphore_handler.loop = &loop;

    taskman_register(&semaphore_handler);
}

void taskman_semaphore_init(
    struct taskman_semaphore* semaphore,
    uint32_t initial,
    uint32_t max
) {
    semaphore->count = initial;
    semaphore->max = max;
}

void __no_optimize taskman_semaphore_down(struct taskman_semaphore* semaphore) {

    die_if_not(semaphore != NULL);

    struct wait_data wd;
    wd.semaphore = semaphore;
    wd.up = 0;
    taskman_wait(&semaphore_handler, &wd);
}

void __no_optimize taskman_semaphore_up(struct taskman_semaphore* semaphore) {

    die_if_not(semaphore != NULL);
    struct wait_data wd;
    wd.semaphore = semaphore;
    wd.up = 1;
    taskman_wait(&semaphore_handler, &wd);
}
