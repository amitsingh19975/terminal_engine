#if !defined(AMT_TERMINAL_ENGINE_POINT_HPP)
#define AMT_TERMINAL_ENGINE_POINT_HPP

#include <tuple>
#include <string>
#include <functional>
#include <dark/core/terminal_function.hpp>

namespace amt::core{
    
    struct point_t{
        
        using size_type = int;
        using base_type = std::pair<size_type, size_type>;
        static constexpr int npos = std::numeric_limits<int>::min();

        point_t() = default;

        point_t( size_type x, size_type y )
            : m_data(x,y)
        {}

        inline constexpr size_type const& x() const noexcept{
            return std::get<0>(m_data);
        }

        inline constexpr size_type& x() noexcept{
            return std::get<0>(m_data);
        }

        inline constexpr size_type const& y() const noexcept{
            return std::get<1>(m_data);
        }

        inline constexpr size_type& y() noexcept{
            return std::get<1>(m_data);
        }

        constexpr bool operator==(point_t const& rhs) const noexcept{
            return m_data == rhs.m_data;
        }

        constexpr bool operator!=(point_t const& rhs) const noexcept{
            return !(*this == rhs);
        }

        std::string str() const noexcept{
            return detail::move_to(x(),y());
        }

        constexpr bool empty() const noexcept{
            return x() == npos && y() == npos;
        }

        friend auto& operator<<(std::ostream& os, point_t const& p){
            return os<<p.str();
        }

    private:
        base_type m_data{npos,npos};
    };

    namespace detail{
        
        struct point_t_hash{
            auto operator()(point_t const& p) const noexcept{
                return std::hash<int>{}(p.x() + p.y());
            }
        };

    } // namespace detail
    
    

} // namespace amt::core



#endif // AMT_TERMINAL_ENGINE_POINT_HPP
