#if !defined(AMT_TERMINAL_ENGINE_TERMINAL_WINDOW_HPP)
#define AMT_TERMINAL_ENGINE_TERMINAL_WINDOW_HPP

#include <dark/core/terminal_function.hpp>
#include <dark/core/keyboard_event.hpp>
#include <dark/core/window_event.hpp>
#include <string>
#include <sstream>
#include <dark/core/string_buffer.hpp>
#include <dark/event/keyboard.hpp>
#include <dark/event/app.hpp>
#include <dark/core/raw_mode.hpp>

namespace amt::core{
    
    struct window_props{
        std::string title{"Untitled"};
        int width;
        int height;
        uint32_t baud_rate{9600};
        bool is_winsize_set{false};

        window_props() {
            auto [w,h] = detail::window_size();
            width = w;
            height = h;
        }

        window_props(std::string_view title, uint32_t baud_rate = 9600)
            : title(title)
            , baud_rate(baud_rate)
        {
            auto [w,h] = detail::window_size();
            width = w;
            height = h;
        }

        window_props(std::string_view title, int w, int h, uint32_t baud_rate = 9600)
            : title(title)
            , width(w)
            , height(h)
            , baud_rate(baud_rate)
            , is_winsize_set(true)
        {}
    };

    struct terminal_window{
        
        using event_callback_t = std::function<void(event_base_t&)>;

        terminal_window( window_props const& p = window_props{} ){
            m_data.height       = p.height;
            m_data.width        = p.width;
            m_data.title        = p.title;
            m_data.baud_rate    = p.baud_rate;
            
            if(p.is_winsize_set){
                resize();
            }

            raw_mode.enable(m_data.baud_rate);
        }

        inline constexpr int const& width() const noexcept{
            return m_data.width;
        }

        inline constexpr int const& height() const noexcept{
            return m_data.height;
        }

        inline void set_event_callback(event_callback_t callback) noexcept { 
            m_data.event_callback = std::move(callback); 

            keyboard_event.register_callback([&](EventType_t e, Keyboard_t k){
                if ( e == EventType_t::KeyPressed ){
                    auto t = key_pressed_event_t(k,1);
                    m_data.event_callback(t);
                }
            });

            window_event_callback.register_callback([&](EventType_t e){
                if( e == EventType_t::WindowResize ){
                    auto [w,h] = detail::window_size();
                    auto t = window_resize_event_t(w,h);
                    m_data.event_callback(t);
                }
            });
        }

        ~terminal_window(){
            raw_mode.disable();
        }

    private:

        inline void resize() const{
            std::stringstream ss;
            ss <<"\x1b[8;"<<m_data.width<<';'<<m_data.height<<'t';
            string_buff.eager_write(ss.str());
        }

    private:
        struct window_data{
            event_callback_t    event_callback;
            std::string         title;
            int                 width;
            int                 height;
            uint32_t            baud_rate;
        };
    private:
        window_data m_data;
    };

} // namespace amt::core


#endif // AMT_TERMINAL_ENGINE_TERMINAL_WINDOW_HPP
