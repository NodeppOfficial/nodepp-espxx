/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOfficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_ARDUINO_FS
#define NODEPP_ARDUINO_FS

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace fs {

    inline file_t readable( const string_t& path, const ulong& _size=CHUNK_SIZE ){ return file_t( path, "r", _size ); }
    inline file_t writable( const string_t& path, const ulong& _size=CHUNK_SIZE ){ return file_t( path, "w", _size ); }

    /*─······································································─*/

    inline void read_file( const string_t& path, function_t<void,string_t> cb ){
        if( path.empty() ){ return; } try {
            file_t _file( path, "r" );
            _file.onData( cb ); stream::pipe(_file);
        } catch( except_t ) {}
    }

    inline string_t read_file( const string_t& path ){ string_t s;
        if( path.empty() ){ return s; } try {
            file_t _file( path, "r" );
            return stream::await(_file);
        } catch( except_t ) {} return nullptr;
    }

    /*─······································································─*/

    inline int copy_file( const string_t& src, const string_t& des ){
        if( src.empty() || des.empty() ){ return -1; } try {
            file_t _file_a ( src, "r" );
            file_t _file_b ( des, "w" );
            stream::pipe( _file_a, _file_b ); 
        return 0; } catch( except_t ) {} return -1;
    }

    /*─······································································─*/

    inline int rename_file( const string_t& oname, const string_t& nname ) {
        if( oname.empty() || nname.empty() ){ return -1; }
        return rename( oname.c_str(), nname.c_str() );
    }

    /*─······································································─*/

    inline int move_file( const string_t& oname, const string_t& nname ) {
        if( oname.empty() || nname.empty() ){ return -1;}
        return rename_file( oname, nname );
    }

    /*─······································································─*/

    inline int remove_file( const string_t& path ){
        if( path.empty() ){ return -1; }
        return remove( path.c_str() );
    }

    /*─······································································─*/

    inline bool exists_file( const string_t& path ){
         if ( path.empty() )/*-*/{ return 0; }
        try { file_t( path, "r" ); return 1;
            } catch( except_t ){ } return 0;
    }

    /*─······································································─*/

    int create_file( const string_t& path ){
        if ( path.empty() )/*--*/{ return -1; }
        try{ file_t( path, "w+" ); return  1;
           } catch( except_t ){  } return  0;
    }

    /*─······································································─*/

    inline ulong file_size( const string_t& path ){
        try { file_t file( path, "r" );
              return file.size();
        } catch( except_t ){} return 0;
    }

    /*─······································································─*/

    inline void write_file( const string_t& path, const string_t& data ){
        file_t file( path, "w" ); file.write( data );
    }

    /*─······································································─*/

    inline void append_file( const string_t& path, const string_t& data ){
        file_t file( path, "a" ); file.write( data );
    }

    /*─······································································─*/

    inline bool is_file( const string_t& path ){ return exists_file(path); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif