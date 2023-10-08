#include <__config>
#include <__memory/allocate_at_least.h>
#include <__memory/allocator_traits.h>
#include <__utility/forward.h>
#include <cstddef>
#include <new>
#include <stdexcept>
#include <type_traits>

using namespace std;

template <class _Tp>
class  allocator
    : private __non_trivial_if<!is_void<_Tp>::value, allocator<_Tp> >
{
    /** // Note::static_assert 是cpp的关键字,作用是编译的时候做出判断
    */
    static_assert(!is_volatile<_Tp>::value, "std::allocator does not support volatile types");
public:
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;
    typedef _Tp         value_type;
    /** //  Note:: true_type 返回真值 true
     * typedef integral_constant<bool, true>  true_type;
    */
    typedef true_type   propagate_on_container_move_assignment;
    typedef true_type   is_always_equal;

    
    allocator() _NOEXCEPT = default;

    template <class _Up>  //Note:: 应该是拷贝构造函数
    allocator(const allocator<_Up>&) _NOEXCEPT { }

    _LIBCPP_NODISCARD_AFTER_CXX17 
    //Note:: 分配 __n个 _Tp类型
    _Tp* allocate(size_t __n) {
        if (__n > allocator_traits<allocator>::max_size(*this))
            __throw_bad_array_new_length();
        if (__libcpp_is_constant_evaluated()) {
            //Note:: 全局new
            return static_cast<_Tp*>(::operator new(__n * sizeof(_Tp)));
        } else {
            //Note:: 按照_Tp 的类型内存对齐
            return static_cast<_Tp*>(_VSTD::__libcpp_allocate(__n * sizeof(_Tp), _LIBCPP_ALIGNOF(_Tp)));
        }
    }

#if _LIBCPP_STD_VER > 20
    [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr
    allocation_result<_Tp*> allocate_at_least(size_t __n) {
        return {allocate(__n), __n};
    }
#endif

    
    void deallocate(_Tp* __p, size_t __n) _NOEXCEPT {
        if (__libcpp_is_constant_evaluated()) {
            /** // Note::全局delete
            */
            ::operator delete(__p);
        } else {
            //Note:: 按照_Tp 的类型内存对齐
            _VSTD::__libcpp_deallocate((void*)__p, __n * sizeof(_Tp), _LIBCPP_ALIGNOF(_Tp));
        }
    }

    // C++20 Removed members
#if _LIBCPP_STD_VER <= 17 || defined(_LIBCPP_ENABLE_CXX20_REMOVED_ALLOCATOR_MEMBERS)
    _LIBCPP_DEPRECATED_IN_CXX17 typedef _Tp*       pointer;
    _LIBCPP_DEPRECATED_IN_CXX17 typedef const _Tp* const_pointer;
    _LIBCPP_DEPRECATED_IN_CXX17 typedef _Tp&       reference;
    _LIBCPP_DEPRECATED_IN_CXX17 typedef const _Tp& const_reference;

    template <class _Up> 
    struct _LIBCPP_DEPRECATED_IN_CXX17 rebind {
        //Note::有点没看明白，只有一个typedef 模版
        /** 用法:
         std::allocator<int> myAllocator;
         typename std::allocator<int>::rebind<double>::other myReboundAllocator;

         or: //Note:: 不明白有啥意思 ｜ 有可能是增加耦合性
        std::allocator<int>::rebind<double>::other  othersType;
        auto *dp =othersType.allocate(1);
        *dp=2.111;
        */
        typedef allocator<_Up> other;
    };

    _LIBCPP_DEPRECATED_IN_CXX17 _LIBCPP_INLINE_VISIBILITY
    pointer address(reference __x) const _NOEXCEPT {
        //  Note::_VSTD 编译的时候会替换成std
        return _VSTD::addressof(__x);
    }
    _LIBCPP_DEPRECATED_IN_CXX17 _LIBCPP_INLINE_VISIBILITY
    const_pointer address(const_reference __x) const _NOEXCEPT {
        return _VSTD::addressof(__x);
    }

    _LIBCPP_NODISCARD_AFTER_CXX17 _LIBCPP_INLINE_VISIBILITY _LIBCPP_DEPRECATED_IN_CXX17
    _Tp* allocate(size_t __n, const void*) {
        return allocate(__n);
    }

    _LIBCPP_DEPRECATED_IN_CXX17 _LIBCPP_INLINE_VISIBILITY size_type max_size() const _NOEXCEPT {
        //  Note::   size_type(~0) 返回最大 unsigned long long 值  按位取反
        return size_type(~0) / sizeof(_Tp);
    }

    template <class _Up, class... _Args>
    _LIBCPP_DEPRECATED_IN_CXX17 _LIBCPP_INLINE_VISIBILITY
        //Note:: 可选参数
    void construct(_Up* __p, _Args&&... __args) {
        ::new ((void*)__p) _Up(_VSTD::forward<_Args>(__args)...);
    }

    _LIBCPP_DEPRECATED_IN_CXX17 _LIBCPP_INLINE_VISIBILITY
        //Note:: 调用参数的析构函数
        //typedef _Tp*       pointer;
    void destroy(pointer __p) {
        __p->~_Tp();
    }
#endif
};
