#if !defined(AMT_TERMINAL_ENGINE_TIMESTAMP_HPP)
#define AMT_TERMINAL_ENGINE_TIMESTAMP_HPP

namespace amt::core{
    
    struct timestamp_t{
        timestamp_t( float time = .0f ): m_time(time){}
        constexpr operator float() const noexcept { return m_time; }
        constexpr float sec() const noexcept { return m_time; }
        constexpr float min() const noexcept { return m_time * 1000.0f; }
    private:
        float m_time;
    };

} // namespace amt::core


#endif // AMT_TERMINAL_ENGINE_TIMESTAMP_HPP
