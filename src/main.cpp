#include <zephyr.h>

#include "factory/factory.h"
#include <logging/log_ctrl.h>

 
int main()
{

    Factory::init();
    Factory::build();
    Factory::start();


    //initalize the logger
    log_init();
    while (true)
    {
        /* code */
        k_sleep(K_MSEC(2));

        //Work through every Event
        Factory::xf()->execute();

            log_process(false);
    }
    return 0;
}
