#if !defined(AMT_TERMINAL_ENGINE_ATTRIBUTE_HPP)
#define AMT_TERMINAL_ENGINE_ATTRIBUTE_HPP

#include <ostream>
#include "core/escape_codes.hpp"
#include "core/color.hpp"

namespace amt::core{
    
    namespace detail{
        
        template<typename T>
        struct always_false : std::false_type{};

    } // namespace detail
    

    struct attr{

        attr() = default;

        template<typename T, typename... Args>
        attr(T const& a0, Args&&... args)
            : attr(std::forward<Args>(args)...)
        {
            if constexpr( 
                std::is_same_v<T,f_bit8> ||
                std::is_same_v<T,b_bit8> ||
                std::is_same_v<T,f_rgb>  ||
                std::is_same_v<T,b_rgb>  ||
                std::is_same_v<T,color>
            ){
                m_col.push_back(a0);
            }else if constexpr( std::is_same_v<T, escape_codes> ){
                m_codes.push_back(a0);
            }else{
                static_assert(detail::always_false<T>::value,"amt::core::attr() : invalid type");
            }
        }

        friend std::ostream& operator<<(std::ostream& os, attr const& a){
            return os<<a.str();
        }   
        
        template<typename T>
        attr& operator<<(T const& val){
            if constexpr( 
                std::is_same_v<T,f_bit8> ||
                std::is_same_v<T,b_bit8> ||
                std::is_same_v<T,f_rgb>  ||
                std::is_same_v<T,b_rgb>  ||
                std::is_same_v<T,color>
            ){
                m_col.push_back(val);
            }else if constexpr( std::is_same_v<T, escape_codes> ){
                m_codes.push_back(val);
            }else{
                static_assert(detail::always_false<T>::value,"amt::core::attr::operator<<() : invalid type");
            }
            m_cached_str.clear();
            return *this;
        }

        std::string const& str() const{
            if( m_cached_str.empty() ){

                m_cached_str = "\x1b[";
                
                for(auto const& col : m_col){
                    m_cached_str += col.rstr() + ";";
                }

                for(auto const& code : m_codes){
                    auto idx = static_cast<unsigned char>(code);
                    m_cached_str += number_to_str[idx];
                    m_cached_str.push_back(';');
                }
                m_cached_str.back() = 'm';
            }
            
            return m_cached_str;
        }

        constexpr bool empty() const noexcept{
            return m_col.empty() && m_codes.empty();
        }

    private:
        std::vector<color>          m_col;
        std::vector<escape_codes>   m_codes;
        mutable std::string         m_cached_str;
    };

} // namespace amt::core

namespace amt{
    
    using amt::core::attr;

} // namespace amt


#endif // AMT_TERMINAL_ENGINE_ATTRIBUTE_HPP
