/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOfficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_SSL
#define NODEPP_SSL
#define MBEDTLS_ALLOW_PRIVATE_ACCESS

/*────────────────────────────────────────────────────────────────────────────*/

#include "mbedtls/ssl.h"
#include "mbedtls/error.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class ssl_t {
protected:

    using onSNI = function_t<ssl_t*, string_t>;

    struct NODE {

        mbedtls_ssl_context      ssl;
        mbedtls_ssl_config       conf;
        mbedtls_x509_crt         cert;
        mbedtls_pk_context       pkey;
        mbedtls_entropy_context  entropy;
        mbedtls_ctr_drbg_context ctr_drbg;
        
        string_t key_path, crt_path, cha_path;
        bool srv = 0;
        bool stt = 1;

        ptr_t<X509_t> custom_cert;
        ptr_t<onSNI>  sni_func;

        NODE() {
            mbedtls_ssl_init(&ssl);
            mbedtls_ssl_config_init(&conf);
            mbedtls_x509_crt_init(&cert);
            mbedtls_pk_init(&pkey);
            mbedtls_entropy_init(&entropy);
            mbedtls_ctr_drbg_init(&ctr_drbg);
        }

       ~NODE() {
            mbedtls_ssl_free(&ssl);
            mbedtls_ssl_config_free(&conf);
            mbedtls_x509_crt_free(&cert);
            mbedtls_pk_free(&pkey);
            mbedtls_entropy_free(&entropy);
            mbedtls_ctr_drbg_free(&ctr_drbg);
        }

    };  ptr_t<NODE> obj;

    static int bio_send(void *ctx, const unsigned char *buf, size_t len) {
        auto stream = (socket_t*)ctx;
        int ret = stream->__write((char*)buf, len);
        if( ret == -2 ){ return MBEDTLS_ERR_SSL_WANT_WRITE; }
    return ret; }

    static int bio_recv(void *ctx, unsigned char *buf, size_t len) {
        auto stream = (socket_t*)ctx;
        int ret = stream->__read((char*)buf, len);
        if( ret == -2 ){ return MBEDTLS_ERR_SSL_WANT_READ; }
    return ret; }

    static int sni_callback(void *p_sni, mbedtls_ssl_context *ssl, const unsigned char *name, size_t len) {
        onSNI* func = (onSNI*)p_sni;

        string_t servername((char*)name, (ulong)len);
        ssl_t* new_ssl_obj = (*func)(servername);
        
        if( new_ssl_obj != nullptr )
          { return mbedtls_ssl_set_hs_own_cert(ssl, &new_ssl_obj->obj->cert, &new_ssl_obj->obj->pkey); }

    return 0; }

public:

    ssl_t() : obj( new NODE() ) {
        mbedtls_ctr_drbg_seed(&obj->ctr_drbg, mbedtls_entropy_func, &obj->entropy, NULL, 0);
    }

    ssl_t( const string_t& key, const string_t& cert, const string_t& cha="" ) : ssl_t() {
        obj->key_path = key; obj->crt_path = cert; obj->cha_path = cha;
    }

    ssl_t( ssl_t& xtc, int /*unused*/ ) : obj( new NODE() ) { obj->srv = xtc.obj->srv;
        mbedtls_ctr_drbg_seed(&obj->ctr_drbg, mbedtls_entropy_func, &obj->entropy, NULL, 0);
        mbedtls_ssl_setup(&obj->ssl, &xtc.obj->conf);
    }

    int create_server() { obj->srv = 1;

        mbedtls_ssl_config_defaults(&obj->conf, MBEDTLS_SSL_IS_SERVER, 
                                    MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT);
        
        if( !obj->crt_path.empty() )
          { mbedtls_x509_crt_parse_file(&obj->cert, obj->crt_path.get()); }
        if( !obj->key_path.empty() ){
            mbedtls_pk_parse_keyfile(&obj->pkey, obj->key_path.get(), NULL, 
                                     mbedtls_ctr_drbg_random, &obj->ctr_drbg);
        }

        mbedtls_ssl_conf_ca_chain(&obj->conf, obj->cert.next, NULL);
        mbedtls_ssl_conf_own_cert(&obj->conf, &obj->cert, &obj->pkey);
        mbedtls_ssl_conf_rng(&obj->conf, mbedtls_ctr_drbg_random, &obj->ctr_drbg);

        if( obj->sni_func != nullptr )
          { mbedtls_ssl_conf_sni(&obj->conf, sni_callback, obj->sni_func.get()); }

        return 0;
    }

    int create_client() { obj->srv = 0;
        mbedtls_ssl_config_defaults(&obj->conf, MBEDTLS_SSL_IS_CLIENT, 
                                    MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT);
        mbedtls_ssl_conf_authmode(&obj->conf, MBEDTLS_SSL_VERIFY_NONE );
        mbedtls_ssl_conf_rng(&obj->conf, mbedtls_ctr_drbg_random, &obj->ctr_drbg);
        return 0;
    }

    template<class T>
    int _accept(T* stream) {
        mbedtls_ssl_set_bio(&obj->ssl, stream, bio_send, bio_recv, NULL);
        int ret =  mbedtls_ssl_handshake(&obj->ssl);
        if( ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE ){ return -2; }
        return ( ret==0 ) ? 1 : -1;
    }

    template<class T>
    int _connect( T* stream ) {
        mbedtls_ssl_set_bio(&obj->ssl, stream, bio_send, bio_recv, NULL);
        int ret =  mbedtls_ssl_handshake(&obj->ssl);
        if( ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE ){ return -2; }
        return ( ret==0 ) ? 1 : -1;
    }

    template<class T>
    int __read( T* stream, char* bf, ulong sx ) {
        int ret =  mbedtls_ssl_read(&obj->ssl, (unsigned char*)bf, sx);
        if( ret == MBEDTLS_ERR_SSL_WANT_READ ){ return -2; }
        return ret;
    }

    template<class T>
    int __write( T* stream, char* bf, ulong sx ) {
        int ret =  mbedtls_ssl_write(&obj->ssl, (const unsigned char*)bf, sx);
        if( ret == MBEDTLS_ERR_SSL_WANT_WRITE ){ return -2; }
        return ret;
    }

    void free() { if( !obj->stt ){ return; }
        mbedtls_ssl_close_notify(&obj->ssl);
        obj->stt = 0;
    }

    bool is_server() const { return obj->srv; }

    void set_sni_callback( onSNI cb ) { obj->sni_func = type::bind(cb); }

}; }

/*────────────────────────────────────────────────────────────────────────────*/

#endif

/*────────────────────────────────────────────────────────────────────────────*/