/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOfficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_ARDUINO_WIFI
#define NODEPP_ARDUINO_WIFI

/*────────────────────────────────────────────────────────────────────────────*/

#include <esp_wifi.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class wifi_t {
private:

    using NODE_CLB = function_t<void,ptr_t<const wifi_t>>;

protected:

    struct NODE {
        wifi_config_t wifi_cfg;
        wifi_init_config_t cfg;
        int   state = 1;
    };  ptr_t<NODE> obj;

public:

    event_t<>                    onClose;
    event_t<except_t>            onError;
    event_t<ptr_t<const wifi_t>> onOpen;

    /*─······································································─*/

    virtual ~wifi_t() noexcept {
        if( obj.count() > 1 ){ return; }
        if( obj->state == 0 ){ return; }
        obj->state = 0; onClose.emit();
    }

    wifi_t() noexcept : obj( new NODE() ) {
        obj->cfg = WIFI_INIT_CONFIG_DEFAULT(); esp_wifi_init( &obj->cfg );
        esp_wifi_set_storage( WIFI_STORAGE_RAM );
        esp_wifi_set_mode   ( WIFI_MODE_NULL );
    }

    /*─······································································─*/

    int close() const noexcept { return turn_off(); }

    int turn_on() const noexcept { return esp_wifi_start() == ESP_OK; }

    int turn_off() const noexcept { return esp_wifi_stop() == ESP_OK; }

    /*─······································································─*/

    void listen( const string_t& ssid, const string_t& pass, int channel, NODE_CLB clb=nullptr ) const {
        auto self = type::bind( this );
    process::add([=](){

        self->obj->wifi_cfg.ap.channel        = channel;
        self->obj->wifi_cfg.ap.max_connection = MAX_SOCKET;
        memcpy( self->obj->wifi_cfg.ap.ssid, ssid.get(), min( (uchar)32, (uchar)ssid.size() ));

        if( !pass.empty() ){
            memcpy( self->obj->wifi_cfg.ap.password, pass.get(), min( (uchar)32, (uchar)pass.size() ));
            /*---*/ self->obj->wifi_cfg.ap.authmode = WIFI_AUTH_WPA2_PSK;
        }

        if( self->turn_on() )
          { self->onError.emit( except_t( "can't turn on wifi device" ) ); return -1; }
          
        esp_wifi_set_mode  ( WIFI_MODE_AP );
        esp_wifi_set_config( WIFI_IF_AP, &self->obj->wifi_cfg );

        if( esp_wifi_start() != ESP_OK )
          { self->onError.emit( except_t( "can't start wifi mode" ) ); return -1; }

        clb( self ); self->onOpen.emit( self );

    return -1; });
    }

    /*─······································································─*/

    void connect( const string_t& ssid, const string_t& pass, NODE_CLB clb=nullptr ) const {
        auto self = type::bind( this );
    process::add([=](){

        memcpy( self->obj->wifi_cfg.sta.ssid    , ssid.get(), min( (uchar)32, (uchar)ssid.size() ));
        memcpy( self->obj->wifi_cfg.sta.password, pass.get(), min( (uchar)32, (uchar)pass.size() ));

        if( self->turn_on() )
          { self->onError.emit( except_t( "can't turn on wifi device" ) ); return -1; }

        esp_wifi_set_mode  ( WIFI_MODE_STA );
        esp_wifi_set_config( WIFI_IF_STA, &self->obj->wifi_cfg );

        if( esp_wifi_start() != ESP_OK )
          { self->onError.emit( except_t( "can't start wifi mode" ) ); return -1; }

        if( esp_wifi_connect() != ESP_OK )
          { self->onError.emit( except_t( "can't connect to ssid" ) ); return -1; }

        clb( self ); self->onOpen.emit( self );

    return -1; });
    }

    /*─······································································─*/

    array_t<wifi_ap_record_t> get_devices() const {

        wifi_scan_config_t scan_config;
        scan_config.show_hidden = true;
        scan_config.ssid    = 0;
        scan_config.channel = 0;
        scan_config.bssid   = 0;

        if( turn_on() )
          { return nullptr; }

        if( esp_wifi_scan_start( &scan_config, true ) != ESP_OK )
          { return nullptr; }

        if( esp_wifi_connect() == ESP_OK ) {
            uint16_t len = 0; ptr_t<wifi_ap_record_t> ap_info;
            esp_wifi_scan_get_ap_num(&len); ap_info.resize(len);
            esp_wifi_scan_get_ap_records(&len,&ap_info);
            return ap_info;
        }

        return nullptr;

    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

/*────────────────────────────────────────────────────────────────────────────*/