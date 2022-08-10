#include <iostream>
#include <algorithm>
#include <vector>

struct Object
{
	int id;
	int weight;
	double value;
	double value_per_unit_weight;

	Object(int i, int w, double v) : id(i), weight(w), value(v), value_per_unit_weight(v / w) {}

	// std::sort()���� ���
	inline bool operator< (const Object& obj) const
	{
		return this->value_per_unit_weight < obj.value_per_unit_weight;
	}

	//�ܼ� ��� ����. std::cout << obj << std::endl; �ڵ� ��� ����
	friend std::ostream& operator<<(std::ostream& os, const Object& obj);
};

std::ostream& operator<<(std::ostream& os, const Object& obj)
{
	os << "[" << obj.id << "] ����: " << obj.value << " \t����: " << obj.weight << " kg\t���� ���� �� ����: " << obj.value_per_unit_weight;
	return os;
}

// ���� ���� �賶 ���� �˰��� ���� �Լ�
auto fill_knapsack(std::vector<Object>& objects, int knapsack_capacity)
{
	std::vector<Object> knapsack_contents;

	// ���ǵ��� ������������ ����(���� ���� �� ���� ����)
	std::sort(objects.begin(), objects.end());
	std::reverse(objects.begin(), objects.end());

	// '���� ��ġ �ִ�' ���ǵ� ���� �賶�� �߰�
	auto current_object = objects.begin();
	int current_total_weight = 0;
	while (current_total_weight < knapsack_capacity && current_object != objects.end())
	{
		knapsack_contents.push_back(*current_object);

		current_total_weight += current_object->weight;
		current_object++;
	}

	// ������ �߰��� ���ǿ� ���� �賶 �ִ� ��� ���԰� �ʰ��� ���
	int weight_of_last_obj_to_remove = current_total_weight - knapsack_capacity;
	Object& last_object = knapsack_contents.back();
	if (weight_of_last_obj_to_remove > 0)
	{
		last_object.weight -= weight_of_last_obj_to_remove;
		last_object.value -= last_object.value_per_unit_weight * weight_of_last_obj_to_remove;
	}

	return knapsack_contents;
}

int main(int argc, char* argv[])
{
	std::vector<Object> objects;
	objects.reserve(7);

	std::vector<int> weights{ 1, 2, 5, 9, 2, 3, 4 };
	std::vector<double> values{ 10, 7, 15, 10, 12, 11, 5 };

	for (auto i = 0; i < 7; i++)
	{
		objects.push_back(Object(i + 1, weights[i], values[i]));
	}

	// ����� �� �ִ� ���� ���� ���
	std::cout << "[����� �� �ִ� ���� ����]" << std::endl;
	for (auto& o : objects)
		std::cout << o << std::endl;
	std::cout << std::endl;

	// ���� ���� �賶 ���� �˰��� ����, �賶�� �ִ� ��� ���Դ� 7�� ����
	int knapsack_capacity = 7;
	auto solution = fill_knapsack(objects, knapsack_capacity);

	// �賶�� ���� ���� ���� ���
	std::cout << "[�賶�� ���� ���ǵ� (�ִ� �뷮 = " << knapsack_capacity << ")]" << std::endl;
	for (auto& o : solution)
		std::cout << o << std::endl;
	std::cout << std::endl;
}

