#ifndef NODEPP_DEBUG
#define NODEPP_DEBUG

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class debug_t {     
protected: 

    struct NODE { 
        void * ev = nullptr;
        string_t msg;
    };  ptr_t<NODE> obj;

public:

    virtual ~debug_t() noexcept { 
    //  if ( obj.count() > 2 ){ return; }
	    console::log( obj->msg, "closed" );  
    }
    
    /*─······································································─*/

    debug_t( const string_t& msg ) noexcept : obj(new NODE()) {
        obj->msg = msg; console::log( obj->msg, "open" );
    }
    
    debug_t() noexcept : obj(new NODE()) {
        obj->msg = "something went wrong"; console::log( obj->msg, "open" );
    }
    
    /*─······································································─*/

    template< class... T >
    void log( const T&... args ) const noexcept { console::log( "--", args... ); }

    void error() const noexcept { console::error( obj->msg ); }
    
};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif