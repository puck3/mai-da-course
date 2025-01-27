#include <iostream>
#include <string>
#include <vector>

class Solution {
 public:
  bool checkValidString(std::string s) {
    int n = s.size();
    std::vector<std::vector<bool>> dp(n + 1, std::vector<bool>(n + 1, false));
    dp[0][0] = true;

    for (int i = 1; i <= n; ++i) {
      for (int j = 0; j <= n; ++j) {
        if (dp[i - 1][j]) {
          if ((s[i - 1] == '(') && (j < n)) {
            dp[i][j + 1] = true;
          } else if ((s[i - 1] == ')') && (j > 0)) {
            dp[i][j - 1] = true;
          } else if (s[i - 1] == '*') {
            dp[i][j] = true;
            if (j < n) {
              dp[i][j + 1] = true;
            }
            if ((j > 0)) {
              dp[i][j - 1] = true;
            }
          }
        }
      }
    }

    return dp[n][0];
  }
};