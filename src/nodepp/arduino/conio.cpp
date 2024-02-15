#pragma once

#define C_BLACK   0x00
#define C_WHITE   0x01
#define C_GREEN   0x02
#define C_RED     0x03
#define C_BLUE    0x04
#define C_CYAN    0x05
#define C_YELLOW  0x06
#define C_MAGENTA 0x07
#define C_BOLD    0x10

namespace nodepp { namespace conio {

    /*─······································································─*/

    int perr( const string_t& args ){ return Serial.write( args.c_str() ); }
    int pout( const string_t& args ){ return Serial.write( args.c_str() ); }

    template< class V, class... T >
    int scan( const V& argc, const T&... args ){ while(!Serial.available() ){}
        return string::parse( Serial.readString().c_str(), (char*)argc, args... );
    }

    /*─······································································─*/

    template< class... T >
    int log( const T&... args ){
        int last = sizeof...( args ), size = 0;
        string::map([&]( string_t arg ){ 
            size += pout( arg + ( --last<1 ? "" : " " ) ); 
        },  args... ); 
            size += pout("\033[0m"); 
        return size;
    }

    /*─······································································─*/

    int gotoxy( int x, int y ){ return pout(string::format("\033[%d;%dH",x,y)); }
    int undescore(){ return pout("\033[4m"); }
    int inverse(){ return pout("\033[7m"); }
    int reset(){ return pout("\033[0m"); }
    int clear(){ return pout("\033c"); }

    /*─······································································─*/

    int background( int color ){ 
        if( color & 0x10 ){ pout("\033[1m"); color &= 0x0f; }
        switch( color ) {
            case C_BLACK:   return pout("\033[40m"); break;
            case C_WHITE:   return pout("\033[47m"); break;
            case C_GREEN:   return pout("\033[42m"); break;
            case C_RED:     return pout("\033[41m"); break;
            case C_BLUE:    return pout("\033[44m"); break;
            case C_CYAN:    return pout("\033[46m"); break;
            case C_YELLOW:  return pout("\033[43m"); break;
            case C_MAGENTA: return pout("\033[45m"); break;
        }   return -1;
    }

    int foreground( int color ){
        if( color & 0x10 ){ pout("\033[1m"); color &= 0x0f; }
        switch( color ) {
            case C_BLACK:   return pout("\033[30m"); break;
            case C_WHITE:   return pout("\033[37m"); break;
            case C_GREEN:   return pout("\033[32m"); break;
            case C_RED:     return pout("\033[31m"); break;
            case C_BLUE:    return pout("\033[34m"); break;
            case C_CYAN:    return pout("\033[36m"); break;
            case C_YELLOW:  return pout("\033[33m"); break;
            case C_MAGENTA: return pout("\033[35m"); break;
        }   return -1;
    }

    /*─······································································─*/

    int error( const char* msg ){ foreground( C_RED    | C_BOLD ); return log( msg ); }
    int  info( const char* msg ){ foreground( C_CYAN   | C_BOLD ); return log( msg ); }
    int  done( const char* msg ){ foreground( C_GREEN  | C_BOLD ); return log( msg ); }
    int  warn( const char* msg ){ foreground( C_YELLOW | C_BOLD ); return log( msg ); }

}}