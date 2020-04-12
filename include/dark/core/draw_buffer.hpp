#if !defined(AMT_TERMINAL_ENGINE_DRAW_BUFFER_HPP)
#define AMT_TERMINAL_ENGINE_DRAW_BUFFER_HPP

#include <vector>
#include <initializer_list>
#include <utility>
#include <dark/core/escape_codes.hpp>
#include <dark/core/attribute.hpp>
#include <dark/core/string_buffer.hpp>
#include <dark/core/point.hpp>

namespace amt::core{
    
    enum class DrawOption_t{
        CLIP_TO_BOADER = 0,
        WRAP_TO_NEXT_LINE = 1,
        NONE
    };

    template<typename CharT>
    struct basic_draw_data_t{
        
        basic_draw_data_t( attr a, CharT pixel , int z = 0, char const* s = nullptr)
            : attributes(std::move(a))
            , pixel(pixel)
            , z_index(z)
            , m_code(s)
        {}
        
        basic_draw_data_t( attr a, CharT pixel , DrawOption_t opt, int z = 0, char const* s = nullptr)
            : attributes(std::move(a))
            , pixel(pixel)
            , opt(opt)
            , z_index(z)
            , m_code(s)
        {}

        attr            attributes;
        CharT           pixel{' '};
        DrawOption_t    opt{DrawOption_t::CLIP_TO_BOADER};
        int             z_index{0};

        // friend auto& operator<<(std::ostream& os, basic_draw_data_t const& d){
        //     return os <<"[ "<<d.pixel<< ", " <<d.z_index<<" ]";
        // }

    private:
        inline constexpr bool updated() const noexcept{
            return m_update;
        }

        inline constexpr void set_update() const noexcept{
            m_update = true;
        }

        inline constexpr void reset_update() const noexcept{
            m_update = false;
        }
    private:
        template<typename>
        friend struct   basic_draw_buffer_t;
        char const*     m_code;
        mutable bool    m_update{true};
    };

    template<typename CharT>
    struct basic_draw_buffer_t{
        using draw_data_t       = basic_draw_data_t<CharT>;
        using base_type         = std::vector< basic_draw_data_t<CharT> >;
        using size_type         = typename base_type::size_type;
        using iterator          = typename base_type::iterator;
        using reference         = typename base_type::reference;
        using const_reference   = typename base_type::const_reference;
        using const_iterator    = typename base_type::const_iterator;

        static draw_data_t space;
        static draw_data_t clear_screen;
        static draw_data_t clear_line;
        static draw_data_t save_pos;
        static draw_data_t restore_pos;
        
        basic_draw_buffer_t()
            : basic_draw_buffer_t(detail::window_size())
        {}
        
        basic_draw_buffer_t(std::pair<int, int> size)
            : basic_draw_buffer_t(size.first, size.second)
        {}
        
        basic_draw_buffer_t(int w, int h)
            : m_data( static_cast<size_t>(w * h), space )
            , m_w(w)
            , m_h(h)
        {}
        
        basic_draw_buffer_t( std::initializer_list<draw_data_t> li)
            : basic_draw_buffer_t(detail::window_size())
        {
            for(auto& el : li){
                set( std::move(el) );
            }
        }

        basic_draw_buffer_t( std::initializer_list< std::pair<draw_data_t, point_t> > li)
            : basic_draw_buffer_t(detail::window_size())
        {
            for(auto& el : li){
                set( std::move(el.first), std::move(el.second) );
            }
        }

        basic_draw_buffer_t( base_type const& d, int w, int h )
            : m_data(d)
            , m_w(w)
            , m_h(h)
        {
            assert( m_data.size() == static_cast<size_type>(w * h) );
        }

        void resize(int w, int h) {
            auto size = static_cast<size_type>( w * h );
            m_data.resize(size, space);
            for(auto& el : m_data){
                el.set_update();
            }
        }

        template<typename... Args>
        inline void emplace(attr a, Args&&... args){
            draw_data_t el(std::move(a), std::forward<Args>(args)...);
            set( std::move(el) );
        }

        template<typename... Args>
        inline void emplace(attr a, Args&&... args, point_t p ){
            draw_data_t el(std::move(a), std::forward<Args>(args)...);
            set( std::move(el), p );
        }

        inline void draw() const{
            std::stringstream buf;

            for(auto y = 0; y < m_h; ++y){
                for(auto x = 0; x < m_w; ++x){
                    point_t p(x + 1,y + 1);
                    auto& el = this->at(x,y);
                    if(el.updated()){
                        auto at     = el.attributes;
                        char pixel  = el.pixel;
                        auto code   = el.m_code;
                        auto pos    = p;
                        if( code != nullptr ){
                            buf << pos << at << code <<reset;
                        }else{
                            buf << pos << at << pixel <<reset;
                        }
                        el.reset_update();
                    }
                }
            }
            if( !buf.eof() ){
                string_buff.eager_write( buf.str() );
            }
        }

        inline constexpr draw_data_t& at( int i, int j ) noexcept{
            return m_data[ static_cast<size_type>(m_w * j + i) ];
        }

        inline constexpr draw_data_t const& at( int i, int j ) const noexcept{
            return m_data[ static_cast<size_type>( m_w * j + i ) ];
        }

        inline constexpr draw_data_t& at( point_t pos ) noexcept{
            return m_data[ static_cast<size_type>( m_w * pos.y() + pos.x() ) ];
        }

        inline constexpr draw_data_t const& at( point_t pos ) const noexcept{
            return m_data[ static_cast<size_type>( m_w * pos.y() + pos.x() ) ];
        }

        inline constexpr auto const& operator[](size_type k) const noexcept{
            return m_data[k];
        }

        inline constexpr auto& operator[](size_type k) noexcept{
            return m_data[k];
        }

        inline constexpr iterator begin() noexcept{
            return m_data.begin();
        }

        inline constexpr iterator end() noexcept{
            return m_data.end();
        }

        inline constexpr const_iterator cbegin() const noexcept{
            return m_data.cbegin();
        }

        inline constexpr const_iterator cend() const noexcept{
            return m_data.cend();
        }

        basic_draw_buffer_t& operator<<(draw_data_t d){
            set( std::move(d) );
            return *this;
        }

        basic_draw_buffer_t& operator<<(std::pair< draw_data_t, point_t > p){
            set( std::move(p.first), std::move(p.second) );
            return *this;
        }

        inline constexpr int width() const noexcept{
            return m_w;
        }

        inline constexpr int height() const noexcept{
            return m_h;
        }

    private:

        inline void set( draw_data_t el ){
            set( std::move(el), m_curr );
        }

        inline void set(draw_data_t el, point_t p){
            if( el.opt == DrawOption_t::CLIP_TO_BOADER ){
                clip_to_border(p);
            }else if ( el.opt == DrawOption_t::WRAP_TO_NEXT_LINE ){
                wrap_to_next_line(p);
            }
            auto& d = at(p);
            if( d.z_index <= el.z_index ){
                d = el;
                d.set_update();
                wrap_to_next_line(m_curr);
            }
        }

        inline constexpr void clip_to_border(point_t& p) const noexcept{
            if( p.x() >= m_w ) p.x() = m_w - 1;
            if( p.y() >= m_h ) p.y() = m_h - 1;
            
            if( p.x() < 0 ) p.x() = 0;
            if( p.y() < 0 ) p.y() = 0;
        }

        inline constexpr void wrap_to_next_line(point_t& p) const noexcept{
            auto k = ( p.x() + 1 ) / m_w;
            p.x() = ( p.x() + 1 ) % m_w;
            p.y() += k;
        }

    private:
        mutable base_type m_data;
        int               m_w{-1};
        int               m_h{-1};
        point_t           m_curr{0,0};
    };
    template<typename CharT>
    typename basic_draw_buffer_t<CharT>::draw_data_t basic_draw_buffer_t<CharT>::space = {
        {},
        ' ',
    };
    template<typename CharT>
    typename basic_draw_buffer_t<CharT>::draw_data_t basic_draw_buffer_t<CharT>::clear_screen = {
        {},
        0,
        0,
        ::amt::core::clear_screen
    };
    template<typename CharT>
    typename basic_draw_buffer_t<CharT>::draw_data_t basic_draw_buffer_t<CharT>::clear_line = {
        {},
        0,
        0,
        ::amt::core::clear_line
    };
    template<typename CharT>
    typename basic_draw_buffer_t<CharT>::draw_data_t basic_draw_buffer_t<CharT>::save_pos = {
        {},
        0,
        0,
        ::amt::core::save_pos
    };
    template<typename CharT>
    typename basic_draw_buffer_t<CharT>::draw_data_t basic_draw_buffer_t<CharT>::restore_pos = {
        {},
        0,
        0,
        ::amt::core::restore_pos
    };

} // namespace amt::core


namespace amt{
    
    using draw_buffer_t = core::basic_draw_buffer_t<char>;
    using draw_data_t = core::basic_draw_data_t<char>;


} // namespace amt


#endif // AMT_TERMINAL_ENGINE_DRAW_BUFFER_HPP
