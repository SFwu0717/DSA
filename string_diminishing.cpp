#include <string>
#include <iostream>
using namespace std;

//採dfs的greedy原則 -> 不斷截斷相同且相連的字元直到將字串清空回傳1，反之，回傳0
int dfs(string &s) {
	if (s == "")											//空字串可清空回傳1
		return 1;
	string sub;
	int i, j;
	int ret = 0;
		for (i = 0; i < s.length(); i++) {
			for (j = i; j < s.length(); j++)
				if (s[i] != s[j])
					break;
			if (j >= i + 2) {								//判斷字串中是否有>=兩個以上相同字元
				sub = s.substr(0, i) + s.substr(j);			//將偵測到的字串截斷並將前後連接產生子字串
				ret = dfs(sub);								//將子字串再做下一次的dfs
				if (ret)									
					return ret;								//回傳上一次dfs之結果 
			}
		}
	return ret;												//所有子字串皆無法清空回傳0
}

int main() {
	int testcase;
	string s;
	cin >> testcase;
	while (testcase--) {
		cin >> s;
		cout << dfs(s) << endl;
	}
	return 0;
}