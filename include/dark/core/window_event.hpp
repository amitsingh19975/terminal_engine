#if !defined(AMT_TERMINAL_ENGINE_WINDOW_EVENT_HPP)
#define AMT_TERMINAL_ENGINE_WINDOW_EVENT_HPP

#include <dark/event/event.hpp>
#include <type_traits>
#include <functional>
#include <signal.h>

namespace amt::core{
    
    namespace detail{
        
        struct window_fn_t{
            std::function<void(EventType_t)> f;
        } window_fn;

        void window_handler(int sig){
            if( sig == SIGWINCH ){
                window_fn.f(EventType_t::WindowResize);
            }
        }

    } // namespace detail
    

    struct window_event_callback_t{
        
        window_event_callback_t() {}

        template<typename Fn>
        window_event_callback_t( Fn f ){
            register_callback(std::move(f));
        }

        template<typename Fn>
        void register_callback( Fn f ){
            static_assert(std::is_invocable_v<Fn,EventType_t>, 
                "amt::core::window_event_callback_t: invalid callback function, function should accept EventType_t as parameter");
            detail::window_fn.f = std::move(f);
            signal(SIGWINCH, detail::window_handler);
        }
    };

    inline static window_event_callback_t window_event_callback;

} // namespace amt::core



#endif // AMT_TERMINAL_ENGINE_WINDOW_EVENT_HPP
