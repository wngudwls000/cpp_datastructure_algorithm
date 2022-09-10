#include <vector>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <iomanip>

using namespace std;

#define DEBUG 0
#if DEBUG
#define PRINT(x) cerr << x
#else
#define PRINT(x)
#endif

const int UNKNOWN = INT_MAX;

vector<string> types =
{
	"BRUTE FORCE",
	"MEMOIZATION",
	"TABULATION"
};

void GetTime(clock_t& timer, string type)
{
	timer = clock() - timer;

	cout << type << " ��� ��� �ð�: ";
	cout << fixed << setprecision(5) << (float)timer / CLOCKS_PER_SEC << endl;

	timer = clock();
}

vector<vector<pair<int, int>>> found;

int LCS_BruteForce(string A, string B, int i, int j, vector<pair<int, int>> subsequence)
{
	// ���� I�� a�� ���̺��� Ŀ���ų� �Ǵ� J�� b�� ���̺��� Ŀ����:
	if (i >= A.size() || j >= B.size())
	{
		found.push_back(subsequence);

		// ��͸� �����ϰ� �κ� �������� ���̸� ��ȯ�մϴ�.
		return subsequence.size();
	}

	// ���� A[i] = B[j]�̸�:
	if (A[i] == B[j])
	{
		// �κ� ������ ���̸� 1��ŭ �����մϴ�.
		subsequence.push_back({ i, j });

		// i�� j�� ���� 1�� �����մϴ�.
		return LCS_BruteForce(A, B, i + 1, j + 1, subsequence);
	}

	/* �׷��� ������:

		�ɼ�1) (i + 1)��°�� j���� ���ڿ� ���� �˻縦 �����մϴ�.
		�ɼ�2) i��°�� (j + 1)��° ���ڿ� ���� �˻縦 �����մϴ�.

		�� ������ LCS�� �ɼ� 1 �� �ɼ� 2�� �ִ񰪰� �����ϴ�.
	*/

	return max(LCS_BruteForce(A, B, i + 1, j, subsequence), LCS_BruteForce(A, B, i, j + 1, subsequence));
}

int LCS_Memoization(string A, string B, int i, int j, vector<vector<int>>& memo)
{
	if (i == 0 || j == 0)
	{
		return 0;
	}
	
	if (memo[i - 1][j - 1] != UNKNOWN)
	{
		return memo[i - 1][j - 1];
	}

	if (A[i - 1] == B[j - 1])
	{
		memo[i - 1][j - 1] = 1 + LCS_Memoization(A, B, i - 1, j - 1, memo);
		return memo[i - 1][j - 1];
	}
	
	memo[i - 1][j - 1] = max(LCS_Memoization(A, B, i - 1, j, memo), LCS_Memoization(A, B, i, j - 1, memo));

	return memo[i - 1][j - 1];
}

void PrintSubsequences(string A, string B)
{
	// ���� �Լ��� �̿��� ����
	sort(found.begin(), found.end(), [](auto a, auto b)
		{
			// �κ� ������ ���̸� �������� ����
			if (a.size() != b.size())
			{
				return a.size() < b.size();
			}

			// �� �κ� ������ ���̰� ���ٸ� �ε����� ���� ������ ����
			return a < b;
		});

	// �ߺ� ����
	found.erase(unique(found.begin(), found.end()), found.end());

	int previousSize = 0;

	for (auto subsequnece : found)
	{
		if (subsequnece.size() != previousSize)
		{
			previousSize = subsequnece.size();
			PRINT("SIZE = " << previousSize << endl);
		}
		// ������ �̿��Ͽ� ������ �ڸ��� ǥ��
		string a_seq(A.size(), '_');
		string b_seq(B.size(), '_');

		for (auto pair : subsequnece)
		{
			// ��ĭ�� �κ� ���ڿ� ���ڷ� ä���
			a_seq[pair.first] = A[pair.first];
			b_seq[pair.second] = B[pair.second];
		}

		PRINT("\t" << a_seq << " " << b_seq << endl);
	}
}

int main()
{
	string A, B;
	cin >> A >> B;

	int tests = 2;

	for (int i = 0; i < tests; i++)
	{
		clock_t timer = clock();

		int LCS;

		switch (i)
		{
		case 0:
		{
			LCS = LCS_BruteForce(A, B, 0, 0, {});

#if DEBUG
			PrintSubsequences(A, B);
#endif

			break;
		}
		case 1:
		{
			vector<vector<int>> memo(A.size(), vector<int>(B.size(), UNKNOWN));;
			LCS = LCS_Memoization(A, B, A.size(), B.size(), memo);
			break;
		}
		}
		cout << A << "�� " << B << "�� ���� ���� �κ� ������ ����: " << LCS << endl;

		GetTime(timer, types[i]);
		cout << endl;
	}
}

/*
123456 QWERTY
= 0

ACBEBC ABCBC
= 4

AZYBYXCXW ZZAYYBXXXCWW
= 6

ABCABDBEFBA ABCBEFBEAB
= 8

ABZCYDABAZADAEA YABAZADBBEAAECYACAZ
= 10
*/

