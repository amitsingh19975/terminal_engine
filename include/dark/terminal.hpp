#if !defined(AMT_TERMINAL_ENGINE_TERMINAL_HPP)
#define AMT_TERMINAL_ENGINE_TERMINAL_HPP

#include <dark/core/draw_buffer.hpp>
#include <dark/core/terminal_window.hpp>
#include <memory>
#include <dark/layer_stack.hpp>
#include <chrono>

namespace amt{
    
    template<typename CharT>
    struct basic_terminal_t{
        
        using base_type = core::terminal_window;

        basic_terminal_t()
            : m_win( new base_type() )
        {
            m_win->set_event_callback( std::bind( &basic_terminal_t::on_event, this, std::placeholders::_1 ) );
        }

        virtual ~basic_terminal_t() = default;

        inline void on_event(event_base_t& e){
            event_dispatcher_t dis(e);

            dis.dispatch<window_resize_event_t>([&](window_resize_event_t& e){
                if( e.width() == 0 || e.height() == 0){
                    m_minimized = true;
                }else{
                    m_buff.resize(e.width(), e.height());
                }
                return false;
            });

            dis.dispatch<key_pressed_event_t>([&](key_pressed_event_t& e){
                on_window_close(e);
                return false;
            });

            std::for_each(m_layer_stack.begin(), m_layer_stack.end(), [&e](auto& l){
                if( e.handled ) return;
                l->on_event(e);
            });
        }

        void run() {
            while(m_running){
                float time = get_tick();
                timestamp_t ts = time - m_last_frame;
                m_last_frame = time;

                if( !m_minimized ){
                    for( auto& l : m_layer_stack ){
                        l->on_update(ts, m_buff);
                    }
                }
                m_buff.draw();
            }
        }

        inline void push_layer( std::shared_ptr<layer_t> l){
            m_layer_stack.push_layer(l);
        }

        inline void push_overlay( std::shared_ptr<layer_t> ol){
            m_layer_stack.push_overlay(ol);
        }

        inline constexpr int const& width() const noexcept{
            return m_win->width();
        }

        inline constexpr int const& height() const noexcept{
            return m_win->height();
        }

        inline constexpr void stop() noexcept{
            m_running = false;
        }

        virtual void on_window_close(key_pressed_event_t& e) noexcept{
            if( e.get_key_code() == amt::Keyboard_t::KEY_ESC ){
                stop();
            }
            // m_win.reset();
            // m_buff.~draw_buffer();
            // m_layer_stack.~layer_stack_t();
            // exit(0);
        }

    private:
        
        float get_tick() const{
            auto t = std::chrono::high_resolution_clock::now().time_since_epoch();
            auto ep = std::chrono::duration_cast<std::chrono::milliseconds>(t);
            return ep.count();
        }


    private:
        amt::core::basic_draw_buffer_t<CharT>   m_buff{};
        std::unique_ptr<base_type>              m_win;
        layer_stack_t                           m_layer_stack;
        bool                                    m_minimized{false};
        bool                                    m_running{true};
        float                                   m_last_frame{0.f};
    };


    using terminal_t = basic_terminal_t<char>;

} // namespace amt


#endif // AMT_TERMINAL_ENGINE_TERMINAL_HPP
