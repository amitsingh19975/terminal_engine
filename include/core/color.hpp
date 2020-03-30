#if !defined(AMT_TERMINAL_ENGINE_COLOR_HPP)
#define AMT_TERMINAL_ENGINE_COLOR_HPP

#include <string>
#include <sstream>
#include <variant>
#include "core/esacpe_codes.hpp"

namespace amt::core{


    namespace detail{
        
        struct rgb{
            using color_type = unsigned char;

            constexpr rgb() = default;
            constexpr rgb( color_type r, color_type g = 0, color_type b = 0 ) noexcept
                : r(r), g(g), b(b)
            {}

            std::string str() const {
                std::stringstream ss;
                ss  <<"2;"
                    <<number_to_str[r]<<";"
                    <<number_to_str[g]<<";"
                    <<number_to_str[b]<<"m";
                return ss.str();
            }

            std::string rstr() const {
                std::stringstream ss;
                ss  <<"2;"
                    <<number_to_str[r]<<";"
                    <<number_to_str[g]<<";"
                    <<number_to_str[b];
                return ss.str();
            }


            color_type r{};
            color_type g{};
            color_type b{};
        };

        struct bit8{
            using color_type = unsigned char;

            constexpr bit8() = default;
            constexpr bit8( color_type color ) noexcept
                : col(color)
            {}

            std::string str() const {
                std::stringstream ss;
                ss  <<"5;"
                    <<number_to_str[col]<<"m";
                return ss.str();
            }

            std::string rstr() const {
                std::stringstream ss;
                ss  <<"5;"
                    <<number_to_str[col];
                return ss.str();
            }

            color_type col{};
        };

    } // namespace detail
    

    struct f_rgb : detail::rgb{
        using parent_type = detail::rgb;
        using color_type = typename parent_type::color_type;

        f_rgb() = default;
        f_rgb(color_type r, color_type g = 0, color_type b = 0)
            : parent_type(r,g,b)
        {}

        std::string str() const noexcept{
            return "\x1b[38;" + parent_type::str();
        }

        std::string rstr() const noexcept{
            return "38;" + parent_type::rstr();
        }

    private:
        std::string m_code;
    };

    struct f_bit8 : detail::bit8{
        using parent_type = detail::bit8;
        using color_type = typename parent_type::color_type;

        f_bit8() = default;
        f_bit8(color_type col)
            : parent_type(col)
        {}

        std::string str() const noexcept{
            return "\x1b[38;" + parent_type::str();
        }

        std::string rstr() const noexcept{
            return "38;" + parent_type::rstr();
        }
    };
    

    struct b_rgb : detail::rgb{
        using parent_type = detail::rgb;
        using color_type = typename parent_type::color_type;

        b_rgb() = default;
        b_rgb(color_type r, color_type g = 0, color_type b = 0)
            : parent_type(r,g,b)
        {}

        std::string str() const noexcept{
            return "\x1b[48;" + parent_type::str();
        }

        std::string rstr() const noexcept{
            return "48;" + parent_type::rstr();
        }
    };

    struct b_bit8 : detail::bit8{
        using parent_type = detail::bit8;
        using color_type = typename parent_type::color_type;

        b_bit8() = default;
        b_bit8(color_type col)
            : parent_type(col)
        {}

        std::string str() const noexcept{
            return "\x1b[48;" + parent_type::str();
        }
        
        std::string rstr() const noexcept{
            return "48;" + parent_type::rstr();
        }
    };

    struct color_null{
        std::string str() const noexcept{
            return std::string( escape_seq ) + number_to_str[ 
                    static_cast<unsigned char>( escape_codes::DEFAULT_FONT_COLOR )
                ] + "m";
        }
        
        std::string rstr() const noexcept{
            return number_to_str[ 
                    static_cast<unsigned char>( escape_codes::DEFAULT_FONT_COLOR )
                ];
        }
    };
        
    struct color{
        
        constexpr color() noexcept 
            : m_col(color_null{})
        {}

        color(f_rgb col) noexcept
            : m_col(col)
        {}

        color(f_bit8 col) noexcept
            : m_col(col)
        {}
        
        color(b_rgb col) noexcept
            : m_col(col)
        {}

        color(b_bit8 col) noexcept
            : m_col(col)
        {}

        std::string str() const noexcept{
            return std::visit([](auto&& arg) -> std::string{
                return arg.str();
            }, m_col);
        }

        std::string rstr() const noexcept{
            return std::visit([](auto&& arg) -> std::string{
                return arg.rstr();
            }, m_col);
        }

        template<typename T>
        T& get_color(){
            return std::get<T>(m_col);
        }

        constexpr bool empty() const noexcept{
            return m_col.index() == 4;
        }
        
    private:
        std::variant<f_rgb, f_bit8, b_rgb, b_bit8, color_null> m_col;
    };



} // namespace amt::core

namespace amt{
    
    using amt::core::f_bit8;
    using amt::core::b_bit8;
    using amt::core::f_rgb;
    using amt::core::b_rgb;
    using amt::core::color;

} // namespace amt

#endif // AMT_TERMINAL_ENGINE_COLOR_HPP
