#if !defined(AMT_TERMINAL_ENGINE_KEYBOARD_EVENT_HPP)
#define AMT_TERMINAL_ENGINE_KEYBOARD_EVENT_HPP

#include <queue>
#include "jthread.hpp"
#include <algorithm>
#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include "keys.hpp"
#include "terminal_function.hpp"
#include <iostream>
#include <dark/event/event.hpp>

namespace amt::core{

    struct keyboard_event_t{
        
        using char_type = unsigned char;
        using size_type = ptrdiff_t;
        using queue_t   = std::queue<Keyboard_t>;

        keyboard_event_t()
        {
            m_pollfd.events = POLL_IN;
            m_pollfd.fd = 0 ;
        }
        
        template<typename Fn>
        keyboard_event_t(Fn f)
        {
            m_pollfd.events = POLL_IN;
            m_pollfd.fd = 0 ;
            m_callback = std::move(f);
        }

        ~keyboard_event_t(){
            stop();
        }
        
        keyboard_event_t(keyboard_event_t const&) = delete;
        keyboard_event_t(keyboard_event_t &&) = delete;

        constexpr Keyboard_t peek() const noexcept{
            if( m_keys.empty() ){
                return Keyboard_t::NONE;
            }
            return m_keys.front();
        }

        constexpr Keyboard_t pop() noexcept{
            if( m_keys.empty() ){
                return Keyboard_t::NONE;
            }
            auto k = m_keys.front();
            m_keys.pop();
            return k;
        }

        inline constexpr auto size() const noexcept{
            return m_keys.size();
        }

        inline constexpr auto empty() const noexcept{
            return m_keys.empty();
        }

        void clear() noexcept{
            queue_t empty;
            std::swap(m_keys,empty);
        }

        template<typename Fn>
        void register_callback(Fn f){
            m_callback = std::move(f);
            m_callback_set = true;
        }

    private:

        friend struct raw_mode_t;

        void run(){
            m_thread = jthread([this](stop_token s){
                this->read_input(s);
            });
        }

        void stop(){
            m_thread.request_stop();
        }

        constexpr void push(Keyboard_t k){
            m_keys.push(k);
        }

        inline void read_input(stop_token st) noexcept{
            while(!st.stop_requested()){
                read_input_impl();
            }
        };

        inline void parse(char_type c){
            if( c == static_cast<char_type>( Keyboard_t::KEY_ESC ) ){
                parse_seq();
            }else{
                push(static_cast<Keyboard_t>(c));
            }

            if( m_callback_set && !empty() ){
                m_callback(EventType_t::KeyPressed, pop());
            }
        }

        inline void parse_seq(){
            char_type seq[3]{0};
            if( read(0, &seq[0], 1) <= 0 ) {
                push(Keyboard_t::KEY_ESC);
                return;
            }

            if( read(0, &seq[1], 1) <= 0) { 
                push(Keyboard_t::KEY_ESC);
                return;
            }

            if( seq[0] == static_cast<char_type>( Keyboard_t::KEY_OPEN_BRACKET ) ){
                switch (seq[1])
                {
                case 'A':
                    push(Keyboard_t::KEY_UP);
                    return;
                
                case 'B':
                    push(Keyboard_t::KEY_DOWN);
                    return;
                
                case 'C':
                    push(Keyboard_t::KEY_RIGHT);
                    return;
                
                case 'D':
                    push(Keyboard_t::KEY_LEFT);
                    return;
                default:
                    return;
                }
            }
        }

        inline void read_input_impl() noexcept{
            char_type c{0};
            int ret_val = poll(&m_pollfd,1,0);
            
            if( ret_val < 0 ){
                detail::crash("keyboard_event_t::read_input_impl() : unable to read");
            }else if ( ret_val ){
                if( m_pollfd.revents == POLL_IN ){
                    read(0, &c, 1);
                    parse(c);
                }
            }
        }

        inline constexpr bool is_ctrl(char_type c) const noexcept{
            return c & 0x1f;
        }

    private:
        queue_t                                             m_keys;
        jthread                                             m_thread;
        struct pollfd                                       m_pollfd;
        std::function<void(EventType_t,amt::Keyboard_t)>    m_callback;
        bool                                                m_callback_set{false};
    };

    inline static keyboard_event_t keyboard_event{};

} // namespace amt::core


#endif // AMT_TERMINAL_ENGINE_KEYBOARD_EVENT_HPP
