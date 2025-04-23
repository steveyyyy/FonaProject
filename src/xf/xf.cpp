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

#include "xf.h"

XF XF::theXF; /**< Singleton instance of the XF >**/

/**
 * @brief Create an XF object
 * 
 */
XF::XF(/* args */) {

}

/**
 * @brief Destroy the XF object
 * 
 */
XF::~XF() {
    
}

/**
 * @brief Initialize the message queue
 * 
 */
void XF::init() {
    k_msgq_init(&queue, queue_buffer, sizeof(Event*), 10);
    // k_mutex_init(&m);
}

/**
 * @brief Get the XF instance
 * 
 * @return XF* 
 */
XF* XF::getInstance() {
    return &theXF;
}

/**
 * @brief Push event into the queue or intiate timer
 * 
 * @param e 
 */
void XF::pushEvent(Event* e) {
    // Check if Timeout is set
    if (e->getDelay() > 0)
    {
        scheduleTM(e);
    }else
    {
        k_msgq_put(&queue,&e,K_NO_WAIT);
    }
}

/**
 * @brief process event and delete it if needed
 * 
 */
void XF::execute() {
    IReactive* t;
    t = nullptr;

    // Get the oldest Event in the Queue
    
    Event* e = popEvent();
    if (e)
    {
        t = e->getTarget();
        // Check if Target is set
        if (t)
        {
            // Send the event to our target
            t->processEvent(e);
        }
        if (e->getDnd() == false)
        {
            // Delete the event if needed
            k_free(e);
        }
    }
}

/**
 * @brief Function called on a timeout -> push event into queue after timer runs out
 * 
 * @param t 
 */
void XF::onTimeout(struct k_timer* t) {
    Event* e;
    e = static_cast<Event*>(k_timer_user_data_get(t));
    e->setDelay(0);
    XF::getInstance()->pushEvent(e);
    k_free(t);
    e->setT(NULL);
}

/**
 * @brief Stop the set timer
 * 
 * @param t 
 */
void XF::onStop(struct k_timer* t) {
    Event* e;
    e = static_cast<Event*>(k_timer_user_data_get(t));
    if (e->getDnd() == 0)
    {
        k_free(e);
    }
    k_free(t);
    e->setT(NULL);
}

/**
 * @brief Schedule a timeout
 * 
 * @param e 
 */
void XF::scheduleTM(Event* e) {
    struct k_timer* t;
    t = (struct k_timer*) k_malloc(sizeof(struct k_timer));
    e->setT(t);
    k_timer_init(t, &XF::onTimeout, &XF::onStop);
    k_timer_user_data_set(t,e);
    k_timer_start(t,K_MSEC(e->getDelay()), K_MSEC(0));
}

/**
 * @brief Unschedule the timeout
 * 
 * @param e 
 */
void XF::unscheduleTM(Event* e) {
    struct k_timer* t;
    t = e->getT();
    if (t)
    {
        k_timer_stop(t);
    }
}

/**
 * @brief Remove event from queue
 * 
 * @return Event* 
 */
Event* XF::popEvent() {
    Event* e = nullptr;
    k_msgq_get(&queue,&e,K_NO_WAIT);
    return e;
}


