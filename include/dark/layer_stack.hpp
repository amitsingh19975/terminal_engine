#if !defined(AMT_TERMINAL_ENGINE_LAYER_STACK_HPP)
#define AMT_TERMINAL_ENGINE_LAYER_STACK_HPP

#include <vector>
#include <memory>
#include <dark/layer.hpp>

namespace amt{
    
    struct layer_stack_t{
                
        using base_type = std::vector< std::shared_ptr<layer_t> >;
        using size_type = typename base_type::size_type;
        using value_type = typename base_type::value_type;
        using iterator = typename base_type::iterator;
        using const_iterator = typename base_type::const_iterator;
        using reverse_iterator = typename base_type::reverse_iterator;
        using const_reverse_iterator = typename base_type::const_reverse_iterator;

        layer_stack_t() = default;
        ~layer_stack_t() = default;

        void push_layer( value_type& layer ){
            m_layer.emplace(begin() + m_layerInsertIndex, layer);
            ++m_layerInsertIndex;
        }

        void push_overlay( value_type& overlay ){
            m_layer.emplace_back(overlay);
        }

        void pop_layer( value_type& layer ){
            auto it = std::find(begin(), begin() + m_layerInsertIndex, layer);
            if( it != begin() + m_layerInsertIndex ){
                m_layer.erase(it);
                --m_layerInsertIndex;
            }
        }

        void pop_overlay( value_type& overlay ){
            
            auto it = std::find(begin() + m_layerInsertIndex, end() , overlay);
            if( it != begin() + m_layerInsertIndex ){
                m_layer.erase(it);
            }
            
        }


        inline constexpr iterator begin() noexcept { return m_layer.begin(); }
        inline constexpr iterator end() noexcept { return m_layer.end(); }
        inline constexpr reverse_iterator rbegin() noexcept { return m_layer.rbegin(); }
        inline constexpr reverse_iterator rend() noexcept { return m_layer.rend(); }
        
        inline constexpr const_iterator begin() const noexcept { return m_layer.begin(); }
        inline constexpr const_iterator end() const noexcept { return m_layer.end(); }
        inline constexpr const_reverse_iterator rbegin() const noexcept { return m_layer.rbegin(); }
        inline constexpr const_reverse_iterator rend() const noexcept { return m_layer.rend(); }

    private:
        base_type m_layer;
        uint32_t m_layerInsertIndex{0};

    };
    

} // namespace amt


#endif // AMT_TERMINAL_ENGINE_LAYER_STACK_HPP
