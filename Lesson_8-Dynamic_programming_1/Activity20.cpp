#include <vector>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <iomanip>	

using namespace std;

#define DEBUG 1
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

string ReconstructLCS(vector<vector<int>> DP, string A, string B, int i, int j)
{
	if (i == 0 || j == 0)
	{
		return "";
	}

	if (A[i - 1] == B[j - 1])
	{
		return ReconstructLCS(DP, A, B, i - 1, j - 1) + A[i - 1];
	}
	else if (DP[i - 1][j] > DP[i][j - 1])
	{
		return ReconstructLCS(DP, A, B, i - 1, j);
	}
	else
	{
		return ReconstructLCS(DP, A, B, i, j - 1);
	}
}

string LCS_Tabulation(string A, string B)
{
	vector<vector<int>> DP(A.size() + 1, vector<int>(B.size() + 1));

	for (int i = 0; i < A.size(); i++)
	{
		for (int j = 0; j < B.size(); j++)
		{
			if (i == 0 || j == 0)
			{
				DP[i][j] = 0;
			}
			else if (A[i - 1] == B[j - 1])
			{
				DP[i][j] = DP[i - 1][j - 1] + 1;
			}
			else
			{
				DP[i][j] = max(DP[i - 1][j], DP[i][j - 1]);
			}
		}
	}
	int length = DP[A.size()][B.size()];

	string lcs = ReconstructLCS(DP, A, B, A.size(), B.size());

	return lcs;
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

	int tests = 3;

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
		case 2:
		{
			string lcs = LCS_Tabulation(A, B);
			LCS = lcs.size();

			cout << A << "�� " << B << "�� ���� ���� �κ� ������: " << lcs << endl;
			break;
		}
		}
		cout << A << "�� " << B << "�� ���� ���� �κ� ������ ����: " << LCS << endl;

		GetTime(timer, types[i]);
		cout << endl;
	}
}
/*
A1B2C3D4E ABCDE
5
ABCDE

ABZCYDABAZADAEA YABAZADBBEAAECYACAZ
10
YABAZADAEA

QWJEHFBEMVJEIIFJEEVFBEHFJXAJXBE BVBQHEJEJCHEEHXBNEBCHHEHHFEHSBE
14
QHEJEJEEBEHFBE

AAA12AAA3AA4AA56AA7AAA8 AA1AA2AAA3A4A5A6AA7A89AAA
19
AA12AAA3A4A56AA7AAA

 */