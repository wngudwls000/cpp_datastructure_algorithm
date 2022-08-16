#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

template <typename T>
auto partition(typename std::vector<T>::iterator begin, typename std::vector<T>::iterator end)
{
	// 세 개의 반복자를 생성합니다.
	// 하나는 피벗을 가리키고, 나머지 둘은 벡터의 시작과 마지막 원소를 가리킵니다.
	auto pivot_val = *begin;
	auto left_iter = begin + 1;
	auto right_iter = end;

	while (true)
	{
		// 벡터의 첫 번째 원소부터 시작하여 피벗보다 큰 원소를 찾습니다.
		while (*left_iter <= pivot_val && std::distance(left_iter, right_iter) > 0)
		{
			left_iter++;
		}
		// 벡터의 마지막 번째 원소부터 시작하여 피벗보다 작은 원소를 찾습니다.
		while (*right_iter > pivot_val && std::distance(left_iter, right_iter) > 0)
		{
			right_iter--;
		}

		// 만약 left_iter와 right_iter가 같다면 교환할 원소가 없음을 의미합니다.
		// 그렇지 않으면	left_iter와 right_iter가 가리키는 원소를 서로 교환합니다.
		if (left_iter == right_iter)
			break;
		else
			std::iter_swap(left_iter, right_iter);
	}

	if (pivot_val > *right_iter)
		std::iter_swap(begin, right_iter);

	return right_iter;
}

template <typename T>
void quick_sort(typename std::vector<T>::iterator begin, typename std::vector<T>::iterator last)
{
	// 만약 벡터에 하나 이상의 원소가 있다면
	if (std::distance(begin, last) >= 1)
	{
		// 분할 작업을 수행
		auto partition_iter = partition<T>(begin, last);

		// 분할 작업에 의해 생성된 벡터를 재귀적으로 정렬
		quick_sort<T>(begin, partition_iter - 1);
		quick_sort<T>(partition_iter, last);
	}
}

template <typename T>
void part_quick_sort(typename std::vector<T>::iterator begin, typename std::vector<T>::iterator last, size_t k)
{
	// 만약 벡터에 하나 이상의 원소가 있다면
	if (std::distance(begin, last) >= 1)
	{
		// 분할 작업을 수행
		auto partition_iter = partition<T>(begin, last);

		// 분할 작업에 의해 생성된 벡터를 재귀적으로 정렬
		part_quick_sort<T>(begin, partition_iter - 1, k);
		if (std::distance(begin, partition_iter) < k)
			part_quick_sort<T>(partition_iter, last, k);
	}
}

template <typename T>
void print_vector(std::vector<T> arr)
{
	for (auto i : arr)
		std::cout << i << " ";
	std::cout << std::endl;
}

template <typename T>
auto generate_random_vector(T size)
{
	std::vector<T> v;
	v.reserve(size);

	std::random_device rd;
	std::mt19937 rand(rd());

	std::uniform_int_distribution<std::mt19937::result_type> uniform_dist(1, size);
	for (auto i = 0; i < size; i++)
		v.push_back(uniform_dist(rand));

	return std::move(v);
}

template <typename T>
void run_part_quick_sort_test(size_t size, size_t k)
{
	auto random_vec = generate_random_vector<T>(size);
	auto random_vec_copy(random_vec);

	std::cout << "입력 벡터:" << std::endl;
	print_vector<T>(random_vec);
	std::cout << std::endl;

	// 부분 퀵 정렬
	auto begin_part = std::chrono::steady_clock::now();
	part_quick_sort<T>(random_vec.begin(), random_vec.end() - 1, k);
	auto end_part = std::chrono::steady_clock::now();
	auto diff_part = std::chrono::duration_cast<std::chrono::microseconds>(end_part - begin_part);
	std::cout << "부분 퀵 정렬 수행 시간: " << diff_part.count() << "us" << std::endl;
	std::cout << "(처음 10개 원소만) 부분 정렬된 벡터:" << std::endl;
	print_vector<T>(random_vec);
	std::cout << std::endl;

	// 전체 퀵 정렬
	auto begin_entire = std::chrono::steady_clock::now();
	quick_sort<T>(random_vec_copy.begin(), random_vec_copy.end() - 1);
	auto end_entire = std::chrono::steady_clock::now();
	auto diff_entire = std::chrono::duration_cast<std::chrono::microseconds>(end_entire - begin_entire);
	std::cout << "전체 퀵 정렬 수행 시간: " << diff_entire.count() << "us" << std::endl;
	std::cout << "전체 정렬된 벡터:" << std::endl;
	print_vector<T>(random_vec_copy);
	std::cout << std::endl;
}

int main()
{
	run_part_quick_sort_test<unsigned>(100, 10);
	return 0;
}