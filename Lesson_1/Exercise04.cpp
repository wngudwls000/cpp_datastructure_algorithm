#include <iostream>
#include <forward_list>
#include <vector>

int main()
{
	std::vector<std::string> vec = {
		"Lewis Hamilton", "Lewis Hamilton", "Nico Roseberg", "Sebastian Vettel", "Lewis Hamilton", "Sebastial Vettel", "Sebastial Vettel", "Sebastial Vettel", "Fernando Alonso"
	};

	auto it = vec.begin(); // ��� �ð�
	std::cout << "���� �ֱ� �����: " << *it << std::endl;

	it += 8; // ��� �ð�
	std::cout << "8�� �� �����: " << *it << std::endl;

	advance(it, -3); // ��� �ð�
	std::cout << "���� 3�� �� �����: " << *it << std::endl;


	std::forward_list<std::string> fwd(vec.begin(), vec.end());

	auto it1 = fwd.begin();
	std::cout << "���� �ֱ� �����: " << *it1 << std::endl;

	advance(it1, 5); // ���� �ð�
	std::cout << "5�� �� �����: " << *it1 << std::endl;

	//std::forward_list�� ���������θ� �̵��� �� �����Ƿ� �Ʒ� �ڵ�� ������ �߻��մϴ�.
	//advance(it1, -2);
}