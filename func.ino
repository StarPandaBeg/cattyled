/* Содержит различные полезные функции */

int getFromIndex(char* str, int idx, char div = ',') {
  int val = 0;
  uint16_t i = 0;
  int count = 0;
  bool sign = 0;
  while (str[i]) {
    if (idx == count) {
      if (str[i] == div) break;
      if (str[i] == '-') sign = -1;
      else {
        val *= 10L;
        val += str[i] - '0';
      }
    } else if (str[i] == div) count++;
    i++;
  }
  return sign ? -val : val;
}
