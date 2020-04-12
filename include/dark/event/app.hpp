#if !defined(AMT_TERMINAL_ENGINE_EVENT_APP_EVENT_HPP)
#define AMT_TERMINAL_ENGINE_EVENT_APP_EVENT_HPP

#include <dark/event/event.hpp>

namespace amt{
    
    struct window_resize_event_t
        : event_t< EventType_t::WindowResize, EventCategory_t::App >{

        window_resize_event_t( int width, int height )
            : m_width(width), m_height(height){}
        
        constexpr auto width() const noexcept { return m_width; }
        constexpr auto height() const noexcept { return m_height; }

        std::string to_string() const override{
			std::stringstream ss;
			ss << "window_resize_event: " << m_width << ", " << m_height;
			return ss.str();
		}

    private:
        int m_width;
        int m_height;
    };

    struct window_close_event_t
        : event_t< EventType_t::WindowClose, EventCategory_t::App >{
        window_close_event_t() = default;
    };

} // namespace amt


#endif // AMT_TERMINAL_ENGINE_EVENT_APP_EVENT_HPP
