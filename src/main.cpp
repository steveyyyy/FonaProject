/* 
    FonaProject – Retro Rotary Phone Meets 3G
    Copyright (C) 2025  steveyyyy

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
