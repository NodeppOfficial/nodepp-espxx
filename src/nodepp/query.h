#ifndef NODEPP_QUERY
#define NODEPP_QUERY

/*────────────────────────────────────────────────────────────────────────────*/

#include "regex.h"
#include "map.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

    using query_t = map_t< string_t, string_t >;

    namespace query {

        query_t parse( string_t data ){ query_t null;
            if ( data.empty() || data[0] != '?' ){ return null; }
                 data.shift(); auto args = string::split( data, '&' );
            for( auto x : args ){ 
                 auto y = regex::match_all( x, "[^=]+" );
                 if ( y.size() != 2 ){ continue; }
                 null[ y[0] ] = y[1];
            }    return null;
        }
        
        /*─······································································─*/
        
        string_t format( const map_t<string_t,string_t>& data ){ 
            array_t<string_t> result; for( auto x:data.data() ) 
                   result.push( x.first + "=" + x.second );
            return string::format("?%s",result.join("&").c_str());
        }

    }

}

/*────────────────────────────────────────────────────────────────────────────*/

#endif