#include <fstream>
#include <iostream>
#include <string>

int main() {
  // Запись строки в файл
  std::ofstream outFile("data.bin", std::ios::binary);
  std::string str = "Hello, World!";
  int size = str.size();
  outFile.write(reinterpret_cast<char*>(&size), sizeof(int));
  outFile.write(str.c_str(), size);
  outFile.close();

  // Чтение строки из файла
  std::ifstream inFile("data.bin", std::ios::binary);
  int readSize;
  inFile.read(reinterpret_cast<char*>(&readSize), sizeof(int));
  char* buffer = new char[readSize];
  inFile.read(buffer, readSize);
  std::string readStr(buffer, readSize);
  delete[] buffer;
  inFile.close();

  // Вывод считанной строки
  std::cout << "Read string: " << readStr << std::endl;

  return 0;
}