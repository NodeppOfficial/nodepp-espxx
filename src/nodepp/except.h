#ifndef NODEPP_EXCEPT
#define NODEPP_EXCEPT

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class except_t { 
protected: 

    struct NODE { 
        void * ev = nullptr;
        string_t msg;
    };  ptr_t<NODE> obj;

public:

    virtual ~except_t() noexcept { 
    //  if ( obj.count() > 2 ){ return; }
    }

    /*─······································································─*/

    template< class T, class = typename type::enable_if<type::is_class<T>::value,T>::type >
    except_t( const T& except_type ) noexcept : obj(new NODE()) {
        obj->msg = except_type.what();
    }

    /*─······································································─*/

    except_t( const string_t& msg ) noexcept : obj(new NODE()) {
        obj->msg = msg;
    }

    /*─······································································─*/

    except_t() noexcept : obj(new NODE()) {
        obj->msg = "something went wrong";
    }

    /*─······································································─*/

    const char* what() const noexcept { return obj->msg.c_str(); }

    operator char*() const noexcept { return (char*)what(); }
    
    void print() const noexcept { console::error(obj->msg); } 

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif