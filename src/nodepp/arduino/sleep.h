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

    inline ulong& get_timeout( bool reset=false ) {
    thread_local static ulong stamp=0; 
        if( reset ) { stamp=(ulong)-1; }
    return stamp; }

    inline void clear_timeout() { get_timeout(true); }

    inline ulong set_timeout( int time=0 ) { 
        if( time < 0 ){ /*--------------*/ return 1; }
        auto stamp=&get_timeout(); ulong out=*stamp;
        if( *stamp>(ulong)time ){ *stamp=(ulong)time; }
    return out==0 ? 1 : out; }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {

    inline void delay( ulong time ){ ::ets_delay_us( time * 1000 ); }

    inline ulong now(){ return ::esp_timer_get_time() / 1000; }

    inline void  yield(){ delay( TIMEOUT ); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif