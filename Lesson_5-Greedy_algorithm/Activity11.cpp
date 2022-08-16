#include <iostream>
#include <algorithm>
#include <list>
#include <random>

struct Work
{
	unsigned id;
	unsigned start_time;
	unsigned end_time;
};

auto init_works(int num_works, int max_end_time)
{
	std::random_device rd;
	std::mt19937 rand(rd());
	std::uniform_int_distribution<std::mt19937::result_type> uniform_dist(1, max_end_time);

	std::list<Work> works;

	for (unsigned i = 0; i < num_works; i++)
	{
		auto start_time = uniform_dist(rand);
		auto end_time = uniform_dist(rand);

		if (start_time == end_time) end_time++;
		if (start_time > end_time) std::swap(start_time, end_time);

		works.push_back({ i + 1, start_time, end_time });
	}

	return works;
}

auto work_scheduling(std::list<Work>& works)
{
	works.sort([](const auto& lhs, const auto& rhs) {
		return lhs.end_time < rhs.end_time;
		});

	for (auto curr_work = works.begin(); curr_work != works.end(); curr_work++)
	{
		auto next_work = std::next(curr_work, 1);

		while (next_work != works.end() && next_work->start_time < curr_work->end_time)
		{
			next_work = works.erase(next_work);
		}
	}
	return works;
}

void print(std::list<Work>& works, int max_end_time)
{
	for (auto w : works)
	{
		std::cout << "[" << w.id << "] " << w.start_time << " -> " << w.end_time << "\t|";

		int i = 0;
		for (; i < w.start_time; i++) std::cout << " ";
		for (; i < w.end_time; i++) std::cout << "*";
		for (; i < max_end_time; i++) std::cout << " ";
		std::cout << "|" << std::endl;
	}
}


int main(int argc, char* argv[])
{
	int num_works = 10;
	int max_end_time = 20;

	auto works = init_works(num_works, max_end_time);
	std::cout << "[전체 작업]" << std::endl;
	print(works, max_end_time);

	auto solution = work_scheduling(works);
	std::cout << "[스케쥴 조정한 작업]" << std::endl;
	print(works, max_end_time);
}
