
#include "picosha2.h"
#include <iostream>
#include <string>
#include <clocale>
#include <thread>
#include <random>

std::string suffix;
volatile bool solved = false;
std::string solution;

bool endswith(std::string const &fullString, std::string const &ending) {
#if __cplusplus > 201703L
  return fullString.endswith(ending);
#else
  if (fullString.length() >= ending.length()) {
    return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
  } else {
    return false;
  }
#endif
}

void bruteforce(int seed) {
  std::mt19937 mt_rand(seed);

  while (!solved) {
    std::vector<uint8_t> x(12);

    for (int i = 0; i < 12; i++) {
      x[i] = mt_rand();
    }

    picosha2::hash256_one_by_one hasher;
    hasher.process(x.begin(), x.end());
    hasher.finish();

    std::vector<unsigned char> hash(picosha2::k_digest_size);
    hasher.get_hash_bytes(hash.begin(), hash.end());

    std::string hex_str = picosha2::get_hash_hex_string(hasher);

    if (endswith(hex_str, suffix)) {
      std::cerr << "hash " << hex_str << std::endl;
      // TODO we hope it doesn't find two hashes at the exact cycle needs a scoped lock
      solution = picosha2::bytes_to_hex_string(x);
      solved = true;
      return;
    }
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "USAGE: shapow <hexsuffix>" << std::endl;
    return 1;
  }

  suffix = argv[1];

  if (!suffix.length()) {
    std::cerr << "missing hash suffix" << std::endl;
    return 1;
  }

  for (auto c : suffix) {
    if (!std::isxdigit(c)) {
      std::cerr << "suffix should be all hex chars, bad char: " << c << std::endl;
      return 1;
    }
  }

  srand(clock());

  std::vector<std::thread> threads;
  int num_threads = std::thread::hardware_concurrency() - 1;

  std::cerr << "spawning " << num_threads << " threads" << std::endl;

  for (int i = 0; i < num_threads; i++) {
    threads.push_back(std::thread(bruteforce, rand()));
  }

  for (auto& thread : threads) {
    thread.join();
  }

  std::cout << solution << std::endl;
  return 0;
}
