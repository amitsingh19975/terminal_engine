#if !defined(AMT_TERMINAL_ENGINE_LAYER_HPP)
#define AMT_TERMINAL_ENGINE_LAYER_HPP

#include <string>
#include <dark/core/timestamp.hpp>
#include <dark/event/event.hpp>
#include <dark/core/draw_buffer.hpp>

namespace amt{
    
    using core::timestamp_t;

    struct layer_t{
        layer_t() = default;
        layer_t( std::string_view name )
            : m_debugName(name)
        {}
        virtual ~layer_t() = default;

        virtual void on_update( timestamp_t, draw_buffer_t& ) {}
        virtual void on_event( event_base_t& ) {}

        constexpr std::string_view get_name() const noexcept{ return m_debugName; }
    private:
        std::string             m_debugName{"layer_t"};
    };

} // namespace amt



#endif // AMT_TERMINAL_ENGINE_LAYER_HPP
