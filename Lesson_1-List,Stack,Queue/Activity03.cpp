#include <iostream>
#include <queue>

class Job
{
private:
	int id;
 	std::string name;
	int print_pages;

	static int count;

public:
	Job(const std::string& name_, int pages_) : name(name_), print_pages(pages_), id(++count) { }
	
	friend std::ostream& operator<<(std::ostream& os, const Job& j)
	{
		os << "id: " << j.id << ", 사용자: " << j.name << ", 페이지수: " << j.print_pages << "장";
		return os;
	}
};
int Job::count = 0;

template <size_t N>
class Printer 
{
private:
	std::queue<Job> job_q;

public:
	bool addNewJob(const Job& job)
	{
		if (job_q.size() == N)
		{
			std::cout << "인쇄 대기열에 추가 실패: " << job << std::endl;
			return false;
		}

		std::cout << "인쇄 대기열에 추가: " << job << std::endl;
		job_q.push(job);
		return true;
	}

	void startPrinting()
	{
		while (not job_q.empty())
		{
			std::cout << "인쇄 중: " << job_q.front() << std::endl;
			job_q.pop();
		}
	}
};

int main()
{
	Printer<5> printer;

	Job j1("광희", 10);
	Job j2("정다", 4);
	Job j3("현수", 5);
	Job j4("유미", 7);
	Job j5("채원", 8);
	Job j6("시원", 10);

	printer.addNewJob(j1);
	printer.addNewJob(j2);
	printer.addNewJob(j3);
	printer.addNewJob(j4);
	printer.addNewJob(j5);
	printer.addNewJob(j6); // 인쇄 대기열이 가득 차 있어서 추가할 수 없음
	printer.startPrinting();

	printer.addNewJob(j6); // 인쇄 대기열이 비었으므로 추가 가능
	printer.startPrinting();

}