#ifndef NODEPP_WIFI
#define NODEPP_WIFI

/*────────────────────────────────────────────────────────────────────────────*/

#if _KERNEL_ == NODEPP_KERNEL_ARDUINO
    #include "dns.h"
    #include "event.h"
    #include "arduino/wifi.h"
#else
    #error "This OS Does not support wifi.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif