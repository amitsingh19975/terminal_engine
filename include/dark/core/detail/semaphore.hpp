#ifndef AMT_SEMAPHORE_HPP
#define AMT_SEMAPHORE_HPP

#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>

namespace amt::detail{
    
    struct binary_semaphore{
        constexpr explicit binary_semaphore(int d) 
            : m_counter( d > 0 )
        {}

        inline constexpr void release() noexcept {
            m_counter.fetch_add(1,std::memory_order_release);
        }

        void acquire(){
            int old = 1;
            while( !m_counter.compare_exchange_weak(old,0,std::memory_order_acquire,std::memory_order_relaxed) ){
                old = 1;
                std::this_thread::yield();
            }
        }

    private:
        std::atomic_int m_counter;
    };
} // namespace amt


#endif