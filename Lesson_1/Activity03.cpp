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
		os << "id: " << j.id << ", �����: " << j.name << ", ��������: " << j.print_pages << "��";
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
			std::cout << "�μ� ��⿭�� �߰� ����: " << job << std::endl;
			return false;
		}

		std::cout << "�μ� ��⿭�� �߰�: " << job << std::endl;
		job_q.push(job);
		return true;
	}

	void startPrinting()
	{
		while (not job_q.empty())
		{
			std::cout << "�μ� ��: " << job_q.front() << std::endl;
			job_q.pop();
		}
	}
};

int main()
{
	Printer<5> printer;

	Job j1("����", 10);
	Job j2("����", 4);
	Job j3("����", 5);
	Job j4("����", 7);
	Job j5("ä��", 8);
	Job j6("�ÿ�", 10);

	printer.addNewJob(j1);
	printer.addNewJob(j2);
	printer.addNewJob(j3);
	printer.addNewJob(j4);
	printer.addNewJob(j5);
	printer.addNewJob(j6); // �μ� ��⿭�� ���� �� �־ �߰��� �� ����
	printer.startPrinting();

	printer.addNewJob(j6); // �μ� ��⿭�� ������Ƿ� �߰� ����
	printer.startPrinting();

}