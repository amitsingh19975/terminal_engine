#if !defined(AMT_TERMINAL_ENGINE_DRAW_BUFFER_HPP)
#define AMT_TERMINAL_ENGINE_DRAW_BUFFER_HPP

#include <vector>
#include <initializer_list>
#include <utility>
#include "core/esacpe_codes.hpp"
#include "core/attribute.hpp"
#include "core/string_buffer.hpp"

namespace amt::core{
    
    struct draw_buffer{
        using pair_type = std::pair< attr, std::string >;
        using base_type = std::vector< pair_type >;
        using iterator  = typename base_type::iterator;
        using reference  = typename base_type::reference;
        using const_reference  = typename base_type::const_reference;
        using const_iterator  = typename base_type::const_iterator;

        static pair_type space;
        
        draw_buffer() = default;
        
        draw_buffer( std::initializer_list<pair_type> li)
            : m_data(std::move(li))
        {
        }

        draw_buffer( base_type const& d )
            : m_data(d)
        {}

        void draw() const{
            std::stringstream buf;
            for( auto const& el : m_data ){
                auto at = el.first;
                auto str = el.second;
                buf << at << str << reset;
            }
            string_buff << buf.str();
        }

        constexpr iterator begin() noexcept{
            return m_data.begin();
        }

        constexpr iterator end() noexcept{
            return m_data.end();
        }

        constexpr const_iterator cbegin() const noexcept{
            return m_data.cbegin();
        }

        constexpr const_iterator cend() const noexcept{
            return m_data.cend();
        }

        void push_back( pair_type p ){
            m_data.push_back( std::move(p) );
        }

    private:
        base_type m_data;
    };

    draw_buffer::pair_type draw_buffer::space = std::make_pair( attr{}, std::string(" ") );

} // namespace amt::core


#endif // AMT_TERMINAL_ENGINE_DRAW_BUFFER_HPP
