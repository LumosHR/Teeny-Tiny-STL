#ifndef __LMSTL_ALGOBASE_H__
#define __LMSTL_ALGOBASE_H__

#include "iterator.h"
#include "utility.h"
#include <type_traits>
#include <string.h>

namespace lmstl {

#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif // max

#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif // min

template <typename InputIter1, typename InputIter2>
inline bool equal(InputIter1 beg1, InputIter1 end1, InputIter2 beg2) {
	for (; beg1 != end1; ++beg1, ++beg2) {
		if ((*beg1) != (*beg2))
			return false;
	}
	return true;
}

template <typename InputIter1, typename InputIter2, typename BinaryPredicate>
inline bool equal(InputIter1 beg1, 
	InputIter1 end1, 
	InputIter2 beg2, 
	BinaryPredicate bi_pred) {
	for (; beg1 != end1; ++beg1, ++beg2) {
		if (!bi_pred((*beg1), (*beg2)))
			return false;
	}
	return true;
}

template <typename ForwardIter, typename T>
inline void fill(ForwardIter beg, ForwardIter end, const T& value) {
	for (; beg != end; ++beg)
		*beg = value;
}

template <typename ForwardIter, typename Size, typename T>
inline void fill_n(ForwardIter beg, Size n, const T& value) {
	for (; n > 0; --n, ++beg)
		*beg = value;
}

template <typename ForwardIter1, typename ForwardIter2>
inline void iter_swap(ForwardIter1 a, ForwardIter2 b) {
	typename iterator_traits<ForwardIter1>::value_type tmp = *a;
	*a = *b;
	*b = tmp;
}

template <typename T>
inline const T& max(const T& a, const T& b) {
	return a < b ? b : a;
}

template <typename T, typename Compare>
inline const T& max(const T& a, const T& b, Compare comp) {
	return comp(a, b) ? b : a;
}

template <typename T>
inline const T& min(const T& a, const T& b) {
	return b < a ? b : a;
}

template <typename T, typename Compare>
inline const T& min(const T& a, const T& b, Compare comp) {
	return comp(b, a) ? b : a;
}

template <typename InputIter1, typename InputIter2>
inline bool lexicographical_compare(InputIter1 beg1,
	InputIter1 end1,
	InputIter2 beg2,
	InputIter2 end2) {
	for (; beg1 != end1 && beg2 != end2; ++beg1, ++beg2) {
		if (*beg1 < *beg2)
			return true;
		if (*beg1 > *beg2)
			return false;
	}
	return (beg1 == end1 && beg2 != end2);
}

template <typename InputIter1, typename InputIter2, typename Compare>
inline bool lexicographical_compare(InputIter1 beg1,
	InputIter1 end1,
	InputIter2 beg2,
	InputIter2 end2,
	Compare comp) {
	for (; beg1 != end1 && beg2 != end2; ++beg1, ++beg2) {
		if (comp(*beg1, *beg2))
			return true;
		if (comp(*beg2, *beg1))
			return false;
	}
	return (beg1 == end1 && beg2 != end2);
}

inline bool lexicographical_compare(const unsigned char* beg1,
	const unsigned char* end1,
	const unsigned char* beg2,
	const unsigned char* end2) {
	const size_t len1 = end1 - beg1;
	const size_t len2 = end2 - beg2;
	const int result = memcmp(beg1, end1, min(len1, len2));
	return result ? (result < 0) : (len1 < len2);
}

inline bool lexicographical_compare( const char* beg1,
	const char* end1,
	const char* beg2,
	const char* end2) {
	const size_t len1 = end1 - beg1;
	const size_t len2 = end2 - beg2;
	const int result = memcmp(beg1, end1, min(len1, len2));
	return result ? (result < 0) : (len1 < len2);
}

template <typename InputIter1, typename InputIter2>
pair<InputIter1, InputIter2> mismatch(InputIter1 beg1,
	InputIter1 end1,
	InputIter2 beg2) {
	while (beg1 != end1 && (*beg1) == (*beg2)) {
		++beg1;
		++beg2;
	}
	return pair<InputIter1, InputIter2>(beg1, beg2);
}

template <typename InputIter1, typename InputIter2, typename BinaryPredicate>
pair<InputIter1, InputIter2> mismatch(InputIter1 beg1,
	InputIter1 end1,
	InputIter2 beg2,
	BinaryPredicate bi_pred) {
	while (beg1 != end1 && bi_pred(*beg1, *beg2)) {
		++beg1;
		++beg2;
	}
	return pair<InputIter1, InputIter2>(beg1, beg2);
}

template <typename T>
inline void swap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}

template <typename InputIter, typename OutputIter>
inline OutputIter __copy_t(InputIter beg, InputIter end, OutputIter result, input_iterator_tag) {
	for (; beg != end; ++beg, ++result)
		*result = *beg;
	return result;
}

template <typename RandomAccessIter, typename OutputIter>
inline OutputIter __copy_t(RandomAccessIter beg, RandomAccessIter end, OutputIter result, random_access_iterator_tag) {
	typedef typename iterator_traits<RandomAccessIter>::difference_type Distance;
	for (Distance n = end - beg; n > 0; --n, ++result, ++beg)
		*result = *beg;
	return result;
}

template <typename InputIter, typename OutputIter>
inline OutputIter __copy(InputIter beg, InputIter end, OutputIter result) {
	typedef typename iterator_traits<InputIter>::iterator_category iterator_category;
	return __copy_t(beg, end, result, iterator_category());
}

template <typename T, typename U>
inline typename enable_if<std::is_same<typename std::remove_const<T>::type, U>::value &&
	std::is_trivially_copy_assignable<U>::value, U*>::type __copy(T* beg, T* end, U* result) {
	size_t n = static_cast<size_t>(end - beg);
	memmove(result, beg, sizeof(U) * n);
	return result + n;
}

template <typename InputIter, typename OutputIter>
inline OutputIter copy(InputIter beg, InputIter end, OutputIter result) {
	return __copy(beg, end, result);
}

inline char* copy(const char* beg, const char* end, char* result) {
	size_t n = static_cast<size_t>(end - beg);
	memmove(result, beg, n);
	return result + n;
}

inline wchar_t* copy(const wchar_t* beg, const wchar_t* end, wchar_t* result) {
	size_t n = static_cast<size_t>(end - beg);
	memmove(result, beg, n);
	return result + n;
}

template <typename BidirectIter1, typename BidirectIter2>
inline BidirectIter2 __copy_backward_t(BidirectIter1 beg, BidirectIter1 end, BidirectIter2 result, bidirectional_iterator_tag) {
	for (; end != beg; ) {
		*(--result) = *(--end);
	}
	return result;
}

template <typename BidirectIter1, typename BidirectIter2>
inline BidirectIter2 __copy_backward_t(BidirectIter1 beg, BidirectIter1 end, BidirectIter2 result, random_access_iterator_tag) {
	typedef typename iterator_traits<BidirectIter1>::difference_type Distance;
	for (Distance n = end - beg; n > 0; --n)
		*(--result) = *(--end);
	return result;
}

template <typename BidirectIter1, typename BidirectIter2>
inline BidirectIter2 __copy_backward(BidirectIter1 beg, BidirectIter1 end, BidirectIter2 result) {
	typedef typename iterator_traits<BidirectIter1>::iterator_category iterator_category;
	return __copy_backward_t(beg, end, result, iterator_category());
}

template <typename T, typename U>
inline typename enable_if<std::is_same<typename std::remove_const<T>::type, U>::value&&
	std::is_trivially_copy_assignable<U>::value, U*>::type __copy_backward(T* beg, T* end, U* result){
	const auto n = static_cast<size_t>(end - beg);
	if (n)
	{
		result -= n;
		std::memmove(result, beg, n * sizeof(U));
	}
	return result;
}

template <typename BidirectIter1, typename BidirectIter2>
inline BidirectIter2 copy_backward(BidirectIter1 beg, BidirectIter1 end, BidirectIter2 result) {
	return __copy_backward(beg, end, result);
}

inline char* copy_backward(const char* beg, const char* end, char* result) {
	size_t n = static_cast<size_t>(end - beg);
	if (n) {
		result -= n;
		memmove(result, beg, n);
	}
	return result;
}

inline wchar_t* copy_backward(const wchar_t* beg, const wchar_t* end, wchar_t* result) {
	size_t n = static_cast<size_t>(end - beg);
	if (n) {
		result -= n;
		memmove(result, beg, n);
	}
	return result;
}

}

#endif // !__LMSTL_ALGOBASE_H__