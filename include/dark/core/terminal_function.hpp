#if !defined(AMT_TERMINAL_ENGINE_TERMINAL_FUNCTION_HPP)
#define AMT_TERMINAL_ENGINE_TERMINAL_FUNCTION_HPP

#include <sstream>
#include <tuple>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>

namespace amt::core::detail{
    
    namespace impl{
        template <typename A, typename... R>
        void crash_impl (std::ostringstream &oss, A const& arg, R... rest){
            oss << arg;
            if constexpr( sizeof...(rest) == 0 ){
                perror(oss.str().data());
                exit(1);
            }else{
                crash_impl(oss, std::forward<R>(rest)...);
            }
        }
    } // impl

    template <typename... Args>
    void crash(Args&&... args){
        std::ostringstream oss;
        impl::crash_impl(oss, std::forward<Args>(args)...);
    }

    inline std::string move_to( int x, int y ){
        std::string ret = "\x1b[" + std::to_string(y) + ";" + std::to_string(x) + "H";
        return ret;
    }

    inline std::string move_up( int y ){
        std::string ret = "\x1b[" + std::to_string(y) + "A";
        return ret;
    }

    inline std::string move_down( int y ){
        std::string ret = "\x1b[" + std::to_string(y) + "B";
        return ret;
    }

    inline std::string move_right( int x ){
        std::string ret = "\x1b[" + std::to_string(x) + "C";
        return ret;
    }

    inline std::string move_left( int x ){
        std::string ret = "\x1b[" + std::to_string(x) + "D";
        return ret;
    }

    std::pair<int,int>  window_size() noexcept {
        struct winsize w;
        ioctl(1,TIOCGWINSZ, &w);
        return {w.ws_col, w.ws_row};
    }

} // namespace amt::core::detail



#endif // AMT_TERMINAL_ENGINE_TERMINAL_FUNCTION_HPP
