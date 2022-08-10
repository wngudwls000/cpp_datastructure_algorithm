#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

template <typename T>
auto partition(typename std::vector<T>::iterator begin, typename std::vector<T>::iterator end)
{
	// �� ���� �ݺ��ڸ� �����մϴ�.
	// �ϳ��� �ǹ��� ����Ű��, ������ ���� ������ ���۰� ������ ���Ҹ� ����ŵ�ϴ�.
	auto pivot_val = *begin;
	auto left_iter = begin + 1;
	auto right_iter = end;

	while (true)
	{
		// ������ ù ��° ���Һ��� �����Ͽ� �ǹ����� ū ���Ҹ� ã���ϴ�.
		while (*left_iter <= pivot_val && std::distance(left_iter, right_iter) > 0)
		{
			left_iter++;
		}
		// ������ ������ ��° ���Һ��� �����Ͽ� �ǹ����� ���� ���Ҹ� ã���ϴ�.
		while (*right_iter > pivot_val && std::distance(left_iter, right_iter) > 0)
		{
			right_iter--;
		}

		// ���� left_iter�� right_iter�� ���ٸ� ��ȯ�� ���Ұ� ������ �ǹ��մϴ�.
		// �׷��� ������	left_iter�� right_iter�� ����Ű�� ���Ҹ� ���� ��ȯ�մϴ�.
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
	// ���� ���Ϳ� �ϳ� �̻��� ���Ұ� �ִٸ�
	if (std::distance(begin, last) >= 1)
	{
		// ���� �۾��� ����
		auto partition_iter = partition<T>(begin, last);

		// ���� �۾��� ���� ������ ���͸� ��������� ����
		quick_sort<T>(begin, partition_iter - 1);
		quick_sort<T>(partition_iter, last);
	}
}

template <typename T>
void part_quick_sort(typename std::vector<T>::iterator begin, typename std::vector<T>::iterator last, size_t k)
{
	// ���� ���Ϳ� �ϳ� �̻��� ���Ұ� �ִٸ�
	if (std::distance(begin, last) >= 1)
	{
		// ���� �۾��� ����
		auto partition_iter = partition<T>(begin, last);

		// ���� �۾��� ���� ������ ���͸� ��������� ����
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

	std::cout << "�Է� ����:" << std::endl;
	print_vector<T>(random_vec);
	std::cout << std::endl;

	// �κ� �� ����
	auto begin_part = std::chrono::steady_clock::now();
	part_quick_sort<T>(random_vec.begin(), random_vec.end() - 1, k);
	auto end_part = std::chrono::steady_clock::now();
	auto diff_part = std::chrono::duration_cast<std::chrono::microseconds>(end_part - begin_part);
	std::cout << "�κ� �� ���� ���� �ð�: " << diff_part.count() << "us" << std::endl;
	std::cout << "(ó�� 10�� ���Ҹ�) �κ� ���ĵ� ����:" << std::endl;
	print_vector<T>(random_vec);
	std::cout << std::endl;

	// ��ü �� ����
	auto begin_entire = std::chrono::steady_clock::now();
	quick_sort<T>(random_vec_copy.begin(), random_vec_copy.end() - 1);
	auto end_entire = std::chrono::steady_clock::now();
	auto diff_entire = std::chrono::duration_cast<std::chrono::microseconds>(end_entire - begin_entire);
	std::cout << "��ü �� ���� ���� �ð�: " << diff_entire.count() << "us" << std::endl;
	std::cout << "��ü ���ĵ� ����:" << std::endl;
	print_vector<T>(random_vec_copy);
	std::cout << std::endl;
}

int main()
{
	run_part_quick_sort_test<unsigned>(100, 10);
	return 0;
}