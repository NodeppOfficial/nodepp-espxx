/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOfficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace fs {

    file_t readable( const string_t& path, const ulong& _size=CHUNK_SIZE ){ return file_t( path, "r", _size ); }
    file_t writable( const string_t& path, const ulong& _size=CHUNK_SIZE ){ return file_t( path, "w", _size ); }

    /*─······································································─*/

    void read_file( const string_t& path, function_t<void,string_t> cb ){
        if( path.empty() ){ return; } file_t _file( path, "r" );
        _file.onData( cb ); stream::pipe(_file);
    }

    string_t read_file( const string_t& path ){ string_t s;
        if( path.empty() ){ return s; }
        file_t _file( path, "r" );
        return stream::await(_file);
    }

    /*─······································································─*/

    int copy_file( const string_t& src, const string_t& des ){
        if( src.empty() || des.empty() ){ return -1; } try {
            file_t _file_a ( src, "r" );
            file_t _file_b ( des, "w" );
            stream::pipe( _file_a, _file_b ); return  0;
        } catch(...) {} return -1;
    }

    /*─······································································─*/

    int rename_file( const string_t& oname, const string_t& nname ) {
        if( oname.empty() || nname.empty() ){ return -1; }
        return rename( oname.c_str(), nname.c_str() );
    }

    /*─······································································─*/

    int move_file( const string_t& oname, const string_t& nname ) {
        return rename_file( oname, nname );
    }

    /*─······································································─*/

    int remove_file( const string_t& path ){
        if( path.empty() ){ return -1; }
        return remove( path.c_str() );
    }

    /*─······································································─*/

    bool exists_file( const string_t& path ){
         if ( path.empty() )     { return 0; }
        try { file_t( path, "r" ); return 1;
            } catch(...){} return 0;
    }

    /*─······································································─*/

    int create_file( const string_t& path ){
        if ( path.empty() )      { return -1; }
        try{ file_t( path, "w+" ); return  1;
           } catch(...){} return 0;
    }

    /*─······································································─*/

    ulong file_size( const string_t& path ){
        try { file_t file( path, "r" );
              return file.size();
        } catch(...){} return 0;
    }

    /*─······································································─*/

    void write_file( const string_t& path, const string_t& data ){
        file_t file( path, "w" ); file.write( data );
    }

    /*─······································································─*/

    void append_file( const string_t& path, const string_t& data ){
        file_t file( path, "a" ); file.write( data );
    }

    /*─······································································─*/

    bool is_file( const string_t& path ){ return exists_file(path); }


}}

/*────────────────────────────────────────────────────────────────────────────*/
