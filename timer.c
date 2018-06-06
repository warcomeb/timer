/******************************************************************************
 * Copyright (C) 2017 Marco Giammarini
 *
 * Authors:
 *  Marco Giammarini <m.giammarini@warcomeb.it>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 ******************************************************************************/

#include "timer.h"

static volatile uint32_t Timer_ticks;

static Pit_Config Timer_config = {
    .number    = WARCOMEB_TIMER_NUMBER,
    .frequency = WARCOMEB_TIMER_FREQUENCY,
};

static Timer_callback Timer_callbackArray[WARCOMEB_TIMER_CALLBACK];
static uint8_t Timer_callbackCounter = 0;

void Timer_baseTime (void)
{
    Timer_ticks++;

    for (uint8_t i = 0; i < Timer_callbackCounter; ++i)
    {
        Timer_callbackArray[i]();
    }
}

void Timer_init (void)
{
    Pit_init(OB_PIT0);
    Pit_config(OB_PIT0, Timer_baseTime, &Timer_config);

    Timer_ticks = 0;
    Timer_callbackCounter = 0;

    /* Start timer... */
    Pit_start(OB_PIT0,Timer_config.number);
}

void Timer_delay (uint32_t delay)
{
    uint32_t currTicks = Timer_ticks;

    while ((Timer_ticks - currTicks) < delay);
}

uint32_t Timer_currentTick (void)
{
    return Timer_ticks;
}

void Timer_addCallback (Timer_callback callback)
{
    if (Timer_callbackCounter >= WARCOMEB_TIMER_CALLBACK) return;
    Timer_callbackArray[Timer_callbackCounter++] = callback;
}
