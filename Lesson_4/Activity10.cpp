#include <iostream>
#include "Debug/mapreduce/include/mapreduce.hpp"
#include <numeric> 

namespace wordcount {

	struct map_task : public mapreduce::map_task<
		std::string,                             // MapKey (filename)
		std::pair<char const*, std::uintmax_t>>  // MapValue (memory mapped file contents)
	{
		template<typename Runtime>
		void operator()(Runtime& runtime, key_type const& key, value_type& value) const
		{
			// ���⿡ �ڵ带 �ۼ��ϼ���.
			// runtime.emit_intermediate() �Լ� ���
			bool in_word = false;
			char const* ptr = value.first;
			char const* end = ptr + value.second;
			char const* word = ptr;

			for (; ptr != end; ++ptr)
			{
				char const ch = std::toupper(*ptr, std::locale::classic());

				if (in_word)
				{
					if ((ch < 'A' || ch >'Z') && ch != '\'')
					{
						runtime.emit_intermediate(std::pair<char const*, std::uintmax_t>(word, ptr - word), 1);
						in_word = false;
					}
				}
				else if (ch >= 'A' && ch <= 'Z')
				{
					word = ptr;
					in_word = true;
				}
			}

			if (in_word)
			{
				assert(ptr > word);
				runtime.emit_intermediate(std::pair<char const*, std::uintmax_t>(word, ptr - word), 1);
			}

		}
	};

	template<typename KeyType>
	struct reduce_task : public mapreduce::reduce_task<KeyType, unsigned>
	{
		using typename mapreduce::reduce_task<KeyType, unsigned>::key_type;

		template<typename Runtime, typename It>
		void operator()(Runtime& runtime, key_type const& key, It it, It const ite) const
		{
			runtime.emit(key, std::accumulate(it, ite, 0));
		}
	};

}   // namespace wordcount


template<>
inline uintmax_t const mapreduce::length(std::pair<char const*, uintmax_t> const& string)
{
	return string.second;
}

template<>
inline char const* const mapreduce::data(std::pair<char const*, uintmax_t> const& string)
{
	return string.first;
}

template<>
inline
size_t const
mapreduce::hash_partitioner::operator()(
	std::pair<char const*, std::uintmax_t> const& key,
	size_t partitions) const
{
	return boost::hash_range(key.first, key.first + key.second) % partitions;
}

namespace {

	template<typename T>
	double const sum(T const& durations)
	{
		double sum = 0.0;
		for (auto& chrono : durations)
			sum += chrono.count();
		return sum;
	}

	void write_stats(mapreduce::results const& result)
	{
		if (result.map_times.size() == 0 || result.reduce_times.size() == 0)
			return;

		std::cout << std::endl << "\nMapReduce statistics:";
		std::cout << "\n  MapReduce job runtime                     : " << result.job_runtime.count() << "s of which...";
		std::cout << "\n    Map phase runtime                       : " << result.map_runtime.count() << "s";
		std::cout << "\n    Reduce phase runtime                    : " << result.reduce_runtime.count() << "s";
		std::cout << "\n\n  Map:";
		std::cout << "\n    Total Map keys                          : " << result.counters.map_keys_executed;
		std::cout << "\n    Map keys processed                      : " << result.counters.map_keys_completed;
		std::cout << "\n    Map key processing errors               : " << result.counters.map_key_errors;
		std::cout << "\n    Number of Map Tasks run (in parallel)   : " << result.counters.actual_map_tasks;
		std::cout << "\n    Fastest Map key processed in            : " << std::min_element(result.map_times.cbegin(), result.map_times.cend())->count() << "s";
		std::cout << "\n    Slowest Map key processed in            : " << std::max_element(result.map_times.cbegin(), result.map_times.cend())->count() << "s";
		std::cout << "\n    Average time to process Map keys        : " << sum(result.map_times) / double(result.map_times.size()) << "s";

		std::cout << "\n\n  Reduce:";
		std::cout << "\n    Total Reduce keys                       : " << result.counters.reduce_keys_executed;
		std::cout << "\n    Reduce keys processed                   : " << result.counters.reduce_keys_completed;
		std::cout << "\n    Reduce key processing errors            : " << result.counters.reduce_key_errors;
		std::cout << "\n    Number of Reduce Tasks run (in parallel): " << result.counters.actual_reduce_tasks;
		std::cout << "\n    Number of Result Files                  : " << result.counters.num_result_files;
		if (result.reduce_times.size() > 0)
		{
			std::cout << "\n    Fastest Reduce key processed in         : " << std::min_element(result.reduce_times.cbegin(), result.reduce_times.cend())->count() << "s";
			std::cout << "\n    Slowest Reduce key processed in         : " << std::max_element(result.reduce_times.cbegin(), result.reduce_times.cend())->count() << "s";
			std::cout << "\n    Average time to process Reduce keys     : " << sum(result.reduce_times) / double(result.map_times.size()) << "s";
		}
		std::cout << std::endl;
	}

	std::ostream& operator<<(std::ostream& o, std::pair<char const*, uintmax_t> const& str)
	{
		std::copy(str.first, str.first + str.second, std::ostream_iterator<char>(o, ""));
		return o;
	}

	template<typename Job>
	void write_frequency_table(Job const& job)
	{
		flush(std::cout);

		auto it = job.begin_results();
		auto ite = job.end_results();
		if (it != ite)
		{
			std::cout << std::endl << "�ʸ��ེ ���:" << std::endl;

			using frequencies_t = std::list<typename Job::keyvalue_t>;
			frequencies_t frequencies;
			frequencies.push_back(*it);
			auto it_smallest = frequencies.rbegin();
			for (++it; it != ite; ++it)
			{
				if (frequencies.size() < 1000)    // show top 10
				{
					frequencies.push_back(*it);
					if (it->second < it_smallest->second)
						it_smallest = frequencies.rbegin();
				}
				else if (it->second > it_smallest->second)
				{
					*it_smallest = *it;

					it_smallest = std::min_element(
						frequencies.rbegin(),
						frequencies.rend(),
						mapreduce::detail::less_2nd<typename Job::keyvalue_t>);
				}
			}

			frequencies.sort(mapreduce::detail::greater_2nd<typename Job::keyvalue_t>);
			for (auto& freq : frequencies)
				std::cout << freq.first << "\t" << freq.second << std::endl;
		}
	}

}   // anonymous namespace

int main(int argc, char* argv[])
{
	std::cout << "�ʸ��ེ�� �̿��� ����ī��Ʈ ���α׷�";
	if (argc < 2)
	{
		std::cerr << "����: wordcount <���丮> [num_map_tasks]\n";
		return 1;
	}

	// �Է� ���丮 �̸����κ��� �ʸ��ེ specification�� �غ�
	mapreduce::specification spec;
	spec.input_directory = argv[1];

	spec.map_tasks = std::max(1U, std::thread::hardware_concurrency());
	spec.reduce_tasks = std::max(1U, std::thread::hardware_concurrency());

	std::cout << std::endl << std::max(1U, std::thread::hardware_concurrency())
		<< "���� CPU �ھ� ���" << std::endl;

	// job�� �����ϰ� ����
	using Job = mapreduce::job<wordcount::map_task, wordcount::reduce_task<std::string>>;
	try
	{
		mapreduce::results result;
		typename Job::datasource_type datasource(spec);

		Job job(datasource, spec);
		std::cout << "����ī��Ʈ�� ���� �ʸ��ེ ���� ����..." << std::endl;
		job.run<mapreduce::schedule_policy::cpu_parallel<Job> >(result);

		std::cout << "�ʸ��ེ ����" << std::endl;

		write_stats(result);
		write_frequency_table(job);
	}
	catch (std::exception& e)
	{
		std::cout << "\nError: " << e.what();
	}

	return 0;
}