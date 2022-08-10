#include <iostream>

template <typename T>
struct singly_cll_node
{
	T* data;
	singly_cll_node* next;
	singly_cll_node* prev;

	~singly_cll_node()
	{
		delete data;
	}
};

template <typename T>
class singly_cll
{
public:
	using node = singly_cll_node<T>;
	using node_ptr = node*;

private:
	node_ptr head;
	size_t n;

public:
	singly_cll() : n(0)
	{
		head = new node { NULL, NULL, NULL };
		head->next = head;
		head->prev = head;
	}
	
	size_t size() const
	{
		return n;
	}

	void push(const T& val)
	{
		node_ptr new_node = new node{ new T(val), NULL, NULL};
		n++;
		auto dummy = head->prev;
		dummy->next = new_node;
		new_node->prev = dummy;
		if (head == dummy)
		{
			dummy->prev = new_node;
			new_node->next = dummy;
			head = new_node;
			return;
		}
		new_node->next = head;
		head->prev = new_node;
		head = new_node;
		return;
	}
	void pop(const T& val)
	{
		auto cur = head, dummy = head->prev;
		while (cur != dummy)
		{
			if (*(cur->data) == val)
			{
				cur->prev->next = cur->next;
				cur->next->prev = cur->prev;
				if (cur == head)
					head = head->next;
				delete cur;
				n--;
				return;
			}
			cur = cur->next;
		}

	}

	struct singly_cll_iterator
	{
	private:
		node_ptr ptr;

	public:
		singly_cll_iterator(node_ptr p) : ptr(p) {}

		T& operator*() { return *(ptr->data); }

		node_ptr get() { return ptr; }

		singly_cll_iterator& operator++()
		{
			ptr = ptr->next;
			return *this;
		}
		singly_cll_iterator operator++(int)
		{
			singly_cll_iterator result = *this;
			++(*this);
			return result;
		}
		singly_cll_iterator& operator--()
		{
			ptr = ptr->prev;
			return *this;
		}
		singly_cll_iterator operator--(int)
		{
			singly_cll_iterator result = *this;
			--(*this);
			return result;
		}
		
		friend bool operator==(const singly_cll_iterator& left, const singly_cll_iterator& right)
		{
			return left.ptr == right.ptr;
		}

		friend bool operator!=(const singly_cll_iterator& left, const singly_cll_iterator& right)
		{
			return left.ptr != right.ptr;
		}
	};

	singly_cll_iterator begin() { return singly_cll_iterator(head); }
	singly_cll_iterator end() { return singly_cll_iterator(head->prev); }
	singly_cll_iterator begin() const { return singly_cll_iterator(head); }
	singly_cll_iterator end() const { return singly_cll_iterator(head->prev); }

	singly_cll(const singly_cll<T> & il) : singly_cll()
	{
		for (const auto& i : il)
			push(i);
	}

	singly_cll(const std::initializer_list<T> & ilist) : head(NULL), n(0)
	{
		for (const auto& i : ilist)
			push(i);
	}
	
	~singly_cll()
	{
		while (size())
		{
			pop(*(head->data));
		}

		delete head;
	}
};

class playlist
{
private:
	singly_cll<int> cll;
public:
	void insert(int song)
	{
		cll.push(song);
	}
	void erase(int song)
	{
		cll.pop(song);
	}
	void loopOnce()
	{
		for (auto& song : cll)
			std::cout << song << " ";
		std::cout << std::endl;
	}

};

int main()
{
	playlist pl;
	pl.insert(1);
	pl.insert(2);
	std::cout << "재생 목록 : ";
	pl.loopOnce();

	playlist pl2 = pl;
	pl2.erase(2);
	pl2.insert(3);
	std::cout << "두 번째 재생 목록 : ";
	pl2.loopOnce();
}
