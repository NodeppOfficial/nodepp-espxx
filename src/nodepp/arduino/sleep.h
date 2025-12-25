/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOfficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_ARDUINO_SLEEP
#define NODEPP_ARDUINO_SLEEP

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {

    inline ulong seconds(){ return ::esp_timer_get_time() / 1000000; }

    inline ulong  millis(){ return ::esp_timer_get_time() / 1000; }

    inline ulong  micros(){ return ::esp_timer_get_time(); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {

    inline ulong set_timeout( int time=0 ) { 
        if( time == -1 ) /*------------*/ { return 0; }
    thread_local static ulong stamp; ulong out=stamp;
        if( stamp > time || stamp == 0 ){ stamp=time; }
    return out==0 ? 0 : out; }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {

    inline void delay( ulong time ){ ::ets_delay_us( time * 1000 ); }

    inline ulong now(){ return ::esp_timer_get_time() / 1000; }

    inline void  yield(){ delay( TIMEOUT ); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif