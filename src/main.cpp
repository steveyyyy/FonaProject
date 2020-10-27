#include <zephyr.h>

#include "factory/factory.h"

void main(void)
{

    Factory::init();
    Factory::build();
    Factory::start();

    while (true)
    {
        /* code */
        k_sleep(K_MSEC(2));

        // Work through every Event
        Factory::xf()->execute();

    }
}
