#ifndef __LMSTL_UTILITY_H__
#define __LMSTL_UTILITY_H__

#include "type_traits.h"
#include <type_traits>

namespace lmstl {

template <typename T1, typename T2>
struct pair {
	typedef T1 first_type;
	typedef T2 second_type;

	first_type first;
	second_type second;

	template <typename = typename enable_if<
		std::is_default_constructible<T1>::value &&
		std::is_default_constructible<T2>::value , bool>::type>
	pair():
		first(), second() {}

	template <typename U1, typename U2, typename = typename enable_if<
		std::is_convertible<const U1&, T1>::value && 
		std::is_convertible<const U2&, T2>::value, bool>::type>
	pair(const U1& uf, const U2& us):
		first(uf), second(us) {}

	template <typename U1, typename U2, typename = typename enable_if<
		std::is_convertible<const U1&, T1>::value && 
		std::is_convertible<const U2&, T2>::value, bool>::type>
	pair(const pair<U1, U2>& x):
		first(x.first), second(x.second) {}

	pair(const pair&) = default;
	pair(pair&&) = default;

	inline pair& operator=(const pair& rhs)
	{
		if (this != &rhs)
		{
			first = rhs.first;
			second = rhs.second;
		}
		return *this;
	}

	template <typename U1, typename U2, typename = typename enable_if<
		std::is_convertible<const U1&, T1>::value && 
		std::is_convertible<const U2&, T2>::value, bool>::type>
	inline pair& operator=(const pair<U1, U2>& x) {
		first = x.first;
		second = x.second;
		return *this;
	}

};

/*
    template <class T>
    typename std::remove_reference<T>::type&& move(T&& arg) noexcept
    {
        return static_cast<typename std::remove_reference<T>::type&&>(arg);
    }

    // forward

    template <class T>
    T&& forward(typename std::remove_reference<T>::type& arg) noexcept
    {
        return static_cast<T&&>(arg);
    }

    template <class T>
    T&& forward(typename std::remove_reference<T>::type&& arg) noexcept
    {
        static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
        return static_cast<T&&>(arg);
    }

    // --------------------------------------------------------------------------------------
    // pair

    // �ṹ��ģ�� : pair
    // ����ģ������ֱ��ʾ�������ݵ�����
    // �� first �� second ���ֱ�ȡ����һ�����ݺ͵ڶ�������
    template <class Ty1, class Ty2>
    struct pair
    {
        typedef Ty1    first_type;
        typedef Ty2    second_type;

        first_type first;    // �����һ������
        second_type second;  // ����ڶ�������

        // default constructiable
        template <class Other1 = Ty1, class Other2 = Ty2,
            typename = typename std::enable_if<
            std::is_default_constructible<Other1>::value&&
            std::is_default_constructible<Other2>::value, void>::type>
            constexpr pair()
            : first(), second()
        {
        }

        // implicit constructiable for this type
        template <class U1 = Ty1, class U2 = Ty2,
            typename std::enable_if<
            std::is_copy_constructible<U1>::value&&
            std::is_copy_constructible<U2>::value&&
            std::is_convertible<const U1&, Ty1>::value&&
            std::is_convertible<const U2&, Ty2>::value, int>::type = 0>
            constexpr pair(const Ty1& a, const Ty2& b)
            : first(a), second(b)
        {
        }

        // explicit constructible for this type
        template <class U1 = Ty1, class U2 = Ty2,
            typename std::enable_if<
            std::is_copy_constructible<U1>::value&&
            std::is_copy_constructible<U2>::value &&
            (!std::is_convertible<const U1&, Ty1>::value ||
                !std::is_convertible<const U2&, Ty2>::value), int>::type = 0>
            explicit constexpr pair(const Ty1& a, const Ty2& b)
            : first(a), second(b)
        {
        }

        pair(const pair& rhs) = default;
        pair(pair&& rhs) = default;

        // implicit constructiable for other type
        template <class Other1, class Other2,
            typename std::enable_if<
            std::is_constructible<Ty1, Other1>::value&&
            std::is_constructible<Ty2, Other2>::value&&
            std::is_convertible<Other1&&, Ty1>::value&&
            std::is_convertible<Other2&&, Ty2>::value, int>::type = 0>
            constexpr pair(Other1&& a, Other2&& b)
            : first(lmstl::forward<Other1>(a)),
            second(lmstl::forward<Other2>(b))
        {
        }

        // explicit constructiable for other type
        template <class Other1, class Other2,
            typename std::enable_if<
            std::is_constructible<Ty1, Other1>::value&&
            std::is_constructible<Ty2, Other2>::value &&
            (!std::is_convertible<Other1, Ty1>::value ||
                !std::is_convertible<Other2, Ty2>::value), int>::type = 0>
            explicit constexpr pair(Other1&& a, Other2&& b)
            : first(lmstl::forward<Other1>(a)),
            second(lmstl::forward<Other2>(b))
        {
        }

        // implicit constructiable for other pair
        template <class Other1, class Other2,
            typename std::enable_if<
            std::is_constructible<Ty1, const Other1&>::value&&
            std::is_constructible<Ty2, const Other2&>::value&&
            std::is_convertible<const Other1&, Ty1>::value&&
            std::is_convertible<const Other2&, Ty2>::value, int>::type = 0>
            constexpr pair(const pair<Other1, Other2>& other)
            : first(other.first),
            second(other.second)
        {
        }

        // explicit constructiable for other pair
        template <class Other1, class Other2,
            typename std::enable_if<
            std::is_constructible<Ty1, const Other1&>::value&&
            std::is_constructible<Ty2, const Other2&>::value &&
            (!std::is_convertible<const Other1&, Ty1>::value ||
                !std::is_convertible<const Other2&, Ty2>::value), int>::type = 0>
            explicit constexpr pair(const pair<Other1, Other2>& other)
            : first(other.first),
            second(other.second)
        {
        }

        // implicit constructiable for other pair
        template <class Other1, class Other2,
            typename std::enable_if<
            std::is_constructible<Ty1, Other1>::value&&
            std::is_constructible<Ty2, Other2>::value&&
            std::is_convertible<Other1, Ty1>::value&&
            std::is_convertible<Other2, Ty2>::value, int>::type = 0>
            constexpr pair(pair<Other1, Other2>&& other)
            : first(lmstl::forward<Other1>(other.first)),
            second(lmstl::forward<Other2>(other.second))
        {
        }

        // explicit constructiable for other pair
        template <class Other1, class Other2,
            typename std::enable_if<
            std::is_constructible<Ty1, Other1>::value&&
            std::is_constructible<Ty2, Other2>::value &&
            (!std::is_convertible<Other1, Ty1>::value ||
                !std::is_convertible<Other2, Ty2>::value), int>::type = 0>
            explicit constexpr pair(pair<Other1, Other2>&& other)
            : first(lmstl::forward<Other1>(other.first)),
            second(lmstl::forward<Other2>(other.second))
        {
        }

        // copy assign for this pair
        pair& operator=(const pair& rhs)
        {
            if (this != &rhs)
            {
                first = rhs.first;
                second = rhs.second;
            }
            return *this;
        }

        // move assign for this pair
        pair& operator=(pair&& rhs)
        {
            if (this != &rhs)
            {
                first = lmstl::move(rhs.first);
                second = lmstl::move(rhs.second);
            }
            return *this;
        }

        // copy assign for other pair
        template <class Other1, class Other2>
        pair& operator=(const pair<Other1, Other2>& other)
        {
            first = other.first;
            second = other.second;
            return *this;
        }

        // move assign for other pair
        template <class Other1, class Other2>
        pair& operator=(pair<Other1, Other2>&& other)
        {
            first = lmstl::forward<Other1>(other.first);
            second = lmstl::forward<Other2>(other.second);
            return *this;
        }

        ~pair() = default;

        void swap(pair& other)
        {
            if (this != &other)
            {
                lmstl::swap(first, other.first);
                lmstl::swap(second, other.second);
            }
        }

    };

    // ���رȽϲ����� 
    template <class Ty1, class Ty2>
    bool operator==(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
    {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }

    template <class Ty1, class Ty2>
    bool operator<(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
    {
        return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
    }

    template <class Ty1, class Ty2>
    bool operator!=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
    {
        return !(lhs == rhs);
    }

    template <class Ty1, class Ty2>
    bool operator>(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
    {
        return rhs < lhs;
    }

    template <class Ty1, class Ty2>
    bool operator<=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
    {
        return !(rhs < lhs);
    }

    template <class Ty1, class Ty2>
    bool operator>=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
    {
        return !(lhs < rhs);
    }

    // ���� lmstl �� swap
    template <class Ty1, class Ty2>
    void swap(pair<Ty1, Ty2>& lhs, pair<Ty1, Ty2>& rhs)
    {
        lhs.swap(rhs);
    }

    // ȫ�ֺ��������������ݳ�Ϊһ�� pair
    template <class Ty1, class Ty2>
    pair<Ty1, Ty2> make_pair(Ty1&& first, Ty2&& second)
    {
        return pair<Ty1, Ty2>(lmstl::forward<Ty1>(first), lmstl::forward<Ty2>(second));
    }
*/

}
#endif // !__LMSTL_UTILITY_H__