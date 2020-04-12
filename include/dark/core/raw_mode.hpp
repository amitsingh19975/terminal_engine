#if !defined(AMT_TERMINAL_ENGINE_RAW_MODE_HPP)
#define AMT_TERMINAL_ENGINE_RAW_MODE_HPP

#include <termios.h>
#include <unistd.h>
#include "terminal_function.hpp"
#include "keyboard_event.hpp"

namespace amt::core{
    
    struct raw_mode_t{
        raw_mode_t() = default;

        ~raw_mode_t(){
            disable();
        }

        inline void disable() noexcept{
            disable_impl();
        }

        inline void enable(uint32_t baud_rate = 9600) noexcept{
            enable_impl(baud_rate);
        }

    private:

        constexpr void check_and_set_baud_rate(uint32_t br, struct termios& options) const noexcept{
            switch(br){
                case 4800:   cfsetospeed(&options, B4800);   break;
                case 9600:   cfsetospeed(&options, B9600);   break;
                case 19200:  cfsetospeed(&options, B19200);  break;
                case 38400:  cfsetospeed(&options, B38400);  break;
                case 115200: cfsetospeed(&options, B115200); break;
                default:
                    cfsetospeed(&options, B9600);
            }
        }

        raw_mode_t(raw_mode_t const&) = default;
        raw_mode_t(raw_mode_t&&) = default;

        void static disable_impl(){
            keyboard_event.stop();
            if( s_instance != nullptr ){
                if( s_instance->m_init ){
                    auto& org = s_instance->m_org_state;
                    if( tcsetattr(0,TCSAFLUSH, &org) == - 1 ){
                        detail::crash("raw_mode_t::disable_impl(): ");
                    }
                }
            }else{
                detail::crash("raw_mode_t::disable_impl(): s_instance is nullptr");
            }
        }

        void enable_impl(uint32_t baud_rate){
            using value_type = unsigned long;

            tcgetattr(0,&m_org_state);
            m_init = true;
            auto raw = m_org_state;
            atexit(disable_impl);

            raw.c_iflag &= ~(   value_type(BRKINT) | value_type(ICRNL) | value_type(INPCK) | value_type(ISTRIP) | value_type(IXON) );
            raw.c_oflag &= ~    value_type(OPOST);
            raw.c_cflag |=      value_type(CS8);
            raw.c_lflag &= ~(   value_type(ECHO) | value_type(ICANON) | value_type(IEXTEN) | value_type(ISIG));
            raw.c_cc[VMIN] = 0;
            raw.c_cc[VTIME] = 1;
            
            check_and_set_baud_rate(baud_rate,raw);

            if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
                detail::crash("raw_mode_t::disable_impl(): unable to set raw mode");
            }
            keyboard_event.run();
        }

    private:
        struct termios      m_org_state;
        bool                m_init{false};
        static raw_mode_t*  s_instance;
    };

    inline static raw_mode_t raw_mode{};
    raw_mode_t* raw_mode_t::s_instance = &raw_mode;
} // namespace amt::core

#endif // AMT_TERMINAL_ENGINE_RAW_MODE_HPP
