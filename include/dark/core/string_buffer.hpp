#if !defined(AMT_TERMINAL_ENGINE_BUFFER_HPP)
#define AMT_TERMINAL_ENGINE_BUFFER_HPP

#include <string>
#include <sstream>
#include <type_traits>
#include <memory>
#include <algorithm>
#include <unistd.h>
#include <dark/core/attribute.hpp>

namespace amt::core{

    template<size_t BufferSize = 2048>
    struct string_buffer_t{
        
        static constexpr size_t const buffer_size = BufferSize;

        using char_type = char;
        using size_type = size_t;

        string_buffer_t()
        {}

        bool lazy_write(std::string_view s){
            return write_to_buf(s);
        }

        bool eager_write(std::string_view s){
            return write_impl(s.data(),s.size());
        }
    
        bool flush() {
            return flush_impl();
        }

        friend string_buffer_t& operator<<(string_buffer_t& ds, std::string_view s){
            ds.lazy_write(s);
            return ds;
        }

        template<typename T, std::enable_if_t< !std::is_convertible_v<T,std::string_view>, int > = 0 >
        friend string_buffer_t& operator<<(string_buffer_t& ds, T const& c){
            std::stringstream ss;
            ss << c;
            ds.lazy_write(ss.str());
            return ds;
        }
        
        void clear_screen() noexcept{
            eager_write("\x1b[2J");
        }

        std::string_view buf() const noexcept{
            return std::string_view{m_buff.get(),m_size};
        }

        constexpr bool empty() const noexcept{
            return m_size == 0;
        }

        ~string_buffer_t(){
            flush();
        }
        
        string_buffer_t(string_buffer_t const&) = delete;
        string_buffer_t(string_buffer_t &&) = delete;

    private:

        bool write_to_buf(std::string_view s){
            if( s.size() + m_size >= buffer_size ){
                return flush_impl(s);
            }else{
                std::copy(s.cbegin(), s.cend(), m_buff.get() + m_size);
                m_size += s.size();
                return true;
            }
        }

        bool flush_impl(std::string_view s){
            std::string temp(s);
            s.copy(m_buff.get(),m_size);
            m_size = 0;
            return write_impl(temp.data(), temp.size());
        }

        bool flush_impl(){
            auto res = write_impl(m_buff.get(), m_size);
            m_size = 0;
            return res;
        }

        bool write_impl(char_type const* s, size_type count){
            return write( 1, s, count ) < 0 ? false : true;
        }

    private:
        std::unique_ptr<char>   m_buff{ new char[buffer_size]{0} };
        size_type               m_size{0};
    };

} // namespace amt::core

namespace amt{
    
    inline static core::string_buffer_t<> string_buff;

    namespace core::detail{
        
        void exit_handler(){
            string_buff.flush();
        }

        struct string_buff_init{
            string_buff_init(){
                atexit(exit_handler);
            }
            static string_buff_init init;
        };

        string_buff_init string_buff_init::init;
    } // namespace core::detail
    


} // namespace amt


#endif // AMT_TERMINAL_ENGINE_BUFFER_HPP
