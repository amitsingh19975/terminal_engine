#if !defined(AMT_TERMINAL_ENGINE_EVENT_KEYBOARD_HPP)
#define AMT_TERMINAL_ENGINE_EVENT_KEYBOARD_HPP

#include <dark/event/event.hpp>
#include <dark/core/keys.hpp>

namespace amt{
    
    template<EventType_t type>
    struct key_event_t 
        : event_t<type, event_category_bitwise_or(EventCategory_t::Keyboard, EventCategory_t::Input)>{
        
        constexpr Keyboard_t get_key_code() const noexcept { return m_keyCode; }

    protected:
        key_event_t( Keyboard_t k ) : m_keyCode(k) {}
        Keyboard_t m_keyCode;
    };

    struct key_pressed_event_t : key_event_t<EventType_t::KeyPressed> {

        key_pressed_event_t( Keyboard_t kc, int count )
            : key_event_t(kc), m_repeatCount(count){}

        constexpr auto get_repeat_count() const noexcept { return m_repeatCount; }
    private:
        int m_repeatCount;
    };

} // namespace amt


#endif // AMT_TERMINAL_ENGINE_EVENT_KEYBOARD_HPP
