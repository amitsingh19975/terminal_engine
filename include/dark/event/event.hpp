#if !defined(AMT_TERMINAL_ENGINE_EVENT_EVENT_HPP)
#define AMT_TERMINAL_ENGINE_EVENT_EVENT_HPP

#include <array>

namespace amt{

    enum class EventType_t{
        None = 0,
        WindowClose, WindowResize,
        KeyPressed,
    };

    enum class EventCategory_t{
        None        = 0,
        App         = 1 << 0,
        Input       = 1 << 1,
        Keyboard    = 1 << 2,
    };

    namespace detail{
        constexpr std::array<char const*, 4> event_type_to_string = {
            "None",
            "WindowClose", "WindowResize",
            "KeyPressed"
        };
    }

    struct event_base_t{
        bool handled = false;

        virtual EventType_t get_event_type() const = 0;
        virtual char const* get_name() const = 0;
        virtual EventCategory_t get_category_flags() const = 0;
        virtual std::string to_string() const { return get_name(); }

        inline constexpr auto is_in_category( EventCategory_t cat ) const{
            return static_cast<int>( get_category_flags() ) & static_cast<int>(cat);
        }

        virtual ~event_base_t() {}
    };

    struct event_dispatcher_t{
        event_dispatcher_t( event_base_t& event ): m_event(event){}

        template<typename T, typename P>
        constexpr auto dispatch( P const& pred ){
            if ( m_event.get_event_type() == T::get_static_type() ){
                m_event.handled = pred(static_cast<T&>(m_event));
                return true;
            }
            return false;
        }

    private:
        event_base_t& m_event;
    };

    template<EventType_t Type, EventCategory_t Category>
    struct event_t: event_base_t{
        constexpr static auto get_static_type() noexcept{ return Type; }
        virtual EventType_t get_event_type() const override{ return get_static_type(); }
        virtual char const* get_name() const override { return detail::event_type_to_string[static_cast<size_t>(Type)];}
        virtual EventCategory_t get_category_flags() const override { return Category; }
    };

    inline auto& operator<<( std::ostream& os, event_base_t const& e){
        return os << e.to_string();
    }

    constexpr EventCategory_t event_category_bitwise_or(EventCategory_t a, EventCategory_t b){
        return static_cast<EventCategory_t>(
            static_cast<int>(a) | static_cast<int>(b)
        );
    }

} // namespace amt


#endif // AMT_TERMINAL_ENGINE_EVENT_EVENT_HPP
