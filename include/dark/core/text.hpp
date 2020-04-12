#if !defined(AMT_TERMINAL_ENGINE_TEXT_HPP)
#define AMT_TERMINAL_ENGINE_TEXT_HPP

#include <string>
#include <dark/core/attribute.hpp>
#include <dark/core/draw_buffer.hpp>
#include <dark/core/point.hpp>

namespace amt::core{


    template<typename CharT>
    struct basic_text{
        
        basic_text(std::basic_string_view<CharT> s, attr a, DrawOption_t o, int z = 0)
            : m_data(s)
            , m_attr(std::move(a))
            , m_opt(o)
            , m_z(z)
        {}

        basic_text(std::basic_string_view<CharT> s, attr a, point_t p, DrawOption_t o, int z = 0)
            : m_data(s)
            , m_attr(std::move(a))
            , m_point(std::move(p))
            , m_opt(o)
            , m_z(z)
        {}

        friend auto& operator<<(basic_draw_buffer_t<CharT>& d, basic_text<CharT> const& txt) {
            if( !txt.m_point.empty() ){
                auto curr = txt.m_point;
                for(auto const& c : txt.m_data){
                    d << std::make_pair( basic_draw_data_t<CharT>{ txt.m_attr , c, txt.m_opt, txt.m_z }, curr );
                    curr.x() += 1;
                }
            }else{
                for(auto const& c : txt.m_data){
                    d << basic_draw_data_t<CharT>{ txt.m_attr , c, txt.m_opt, txt.m_z };
                }
            }
            return d;
        }

    private:
        std::basic_string<CharT>    m_data;
        attr                        m_attr;
        point_t                     m_point;
        DrawOption_t                m_opt;
        int                         m_z;
    };


} // namespace amt::core




#endif // AMT_TERMINAL_ENGINE_TEXT_HPP
