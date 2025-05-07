#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include "drift3.h"

// -------------------------------------------------
// Function you want to benchmark
// -------------------------------------------------
inline void work()
{
    // dummy workload — replace / remove this
    volatile double x = 0.0;
    for (int i = 0; i < 1000; ++i)
        x += std::sin(i);
}

std::vector<char> hexToBytes(const std::string& hexStr);
std::vector<char> createLeaf();
// -------------------------------------------------
// Benchmark driver
// -------------------------------------------------
int main(int argc, char* argv[])
{
    // ---- 1. read n ---------------------------------------------------------
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <iterations> <network file>\n";
        return EXIT_FAILURE;
    }
    const std::size_t n = std::strtoull(argv[1], nullptr, 10);
    const std::string& userInput = argv[2];
    if (n == 0) {
        std::cerr << "Iteration count must be > 0\n";
        return EXIT_FAILURE;
    }

    // ---- 2. run and time ---------------------------------------------------


    std::cout << "LOADING NETWORK : " << userInput << std::endl
	      << "----------------------------------------------------------" << std::endl;
    dc3_init(userInput.c_str());
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "START THE BENCHMARK:: " << std::endl;

    std::vector<char>  buffer = createLeaf();
    
    using clock = std::chrono::high_resolution_clock;
    const auto start = clock::now();
    

    for (std::size_t i = 0; i < n; ++i){
      //work();
      dc3_process(&buffer[0]);
      
    }
    const auto end   = clock::now();
    const auto total = end - start;                     // chrono::duration
    const double total_us   = std::chrono::duration<double, std::micro>(total).count();
    const double total_ms   = total_us / 1'000.0;
    const double avg_us     = total_us / static_cast<double>(n);

    // ---- 3. report ---------------------------------------------------------
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "iterations       : " << n               << '\n'
              << "total time (ms)  : " << total_ms        << '\n'
              << "avg / call (µs)  : " << avg_us          << '\n'
	      << "avg rate   (hz)  : " << ((double) n)/(total_ms/1000.0) << '\n';
}

/**--------------------------------------------------------------------
   Helper functions
 */

std::vector<char> createLeaf(){
  std::string data = "1B 00 01 0A 65 04 00 05 35 69 66 69 69 06 00 00 00 02 00 00 00 14 00 00 00 25 00 00 00 01 00 00 00 00 00 80 3F 91 00 00 00 00 00 00 00 06 00 00 00 02 00 00 00 15 00 00 00 24 00 00 00 01 00 00 00 00 00 80 3F 78 01 00 00 00 00 00 00 06 00 00 00 02 00 00 00 13 00 00 00 25 00 00 00 01 00 00 00 00 00 80 3F 2F 01 00 00 00 00 00 00 06 00 00 00 02 00 00 00 17 00 00 00 24 00 00 00 01 00 00 00 00 00 80 3F 17 01 00 00 00 00 00 00 06 00 00 00 02 00 00 00 16 00 00 00 24 00 00 00 01 00 00 00 00 00 80 3F 31 02 00 00 00 00 00 00 06 00 00 00 02 00 00 00 1E 00 00 00 1A 00 00 00 01 00 00 00 00 00 80 3F AA 00 00 00 00 00 00 00 06 00 00 00 02 00 00 00 1C 00 00 00 1B 00 00 00 01 00 00 00 00 00 80 3F 97 01 00 00 00 00 00 00 06 00 00 00 02 00 00 00 19 00 00 00 1B 00 00 00 01 00 00 00 00 00 80 3F 2D 01 00 00 00 00 00 00 06 00 00 00 02 00 00 00 1D 00 00 00 1A 00 00 00 01 00 00 00 00 00 80 3F 0D 01 00 00 00 00 00 00 06 00 00 00 02 00 00 00 1A 00 00 00 1B 00 00 00 01 00 00 00 00 00 80 3F 17 02 00 00 00 00 00 00 06 00 00 00 02 00 00 00 1B 00 00 00 1B 00 00 00 01 00 00 00 00 00 80 3F D6 02 00 00 00 00 00 00 06 00 00 00 02 00 00 00 08 00 00 00 2B 00 00 00 01 00 00 00 00 00 80 3F AC 00 00 00 00 00 00 00 06 00 00 00 02 00 00 00 0A 00 00 00 2B 00 00 00 01 00 00 00 00 00 80 3F A5 00 00 00 00 00 00 00 06 00 00 00 02 00 00 00 0C 00 00 00 2B 00 00 00 01 00 00 00 00 00 80 3F A0 00 00 00 00 00 00 00 06 00 00 00 02 00 00 00 0B 00 00 00 2A 00 00 00 01 00 00 00 00 00 80 3F E1 00 00 00 00 00 00 00 06 00 00 00 02 00 00 00 07 00 00 00 2B 00 00 00 01 00 00 00 00 00 80 3F DB 00 00 00 00 00 00 00 06 00 00 00 02 00 00 00 09 00 00 00 2B 00 00 00 01 00 00 00 00 00 80 3F F3 00 00 00 00 00 00 00 06 00 00 00 02 00 00 00 20 00 00 00 1A 00 00 00 01 00 00 00 00 00 80 3F BC 00 00 00 00 00 00 00 06 00 00 00 02 00 00 00 21 00 00 00 19 00 00 00 01 00 00 00 00 00 80 3F 05 01 00 00 00 00 00 00 06 00 00 00 02 00 00 00 1F 00 00 00 1A 00 00 00 01 00 00 00 00 00 80 3F DE 00 00 00 00 00 00 00 06 00 00 00 02 00 00 00 24 00 00 00 19 00 00 00 01 00 00 00 00 00 80 3F 17 02 00 00 00 00 00 00 06 00 00 00 02 00 00 00 22 00 00 00 19 00 00 00 01 00 00 00 00 00 80 3F DB 01 00 00 00 00 00 00 06 00 00 00 02 00 00 00 0E 00 00 00 26 00 00 00 01 00 00 00 00 00 80 3F 9C 00 00 00 00 00 00 00 06 00 00 00 02 00 00 00 12 00 00 00 25 00 00 00 01 00 00 00 00 00 80 3F 8E 00 00 00 00 00 00 00 06 00 00 00 02 00 00 00 11 00 00 00 25 00 00 00 01 00 00 00 00 00 80 3F FC 00 00 00 00 00 00 00 06 00 00 00 02 00 00 00 04 00 00 00 2A 00 00 00 01 00 00 00 00 00 80 3F B0 00 00 00 00 00 00 00 06 00 00 00 02 00 00 00 0F 00 00 00 25 00 00 00 01 00 00 00 00 00 80 3F E2 00 00 00 00 00 00 00 06 00 00 00 02 00 00 00 06 00 00 00 2A 00 00 00 01 00 00 00 00 00 80 3F A5 00 00 00 00 00 00 00 06 00 00 00 02 00 00 00 10 00 00 00 25 00 00 00 01 00 00 00 00 00 80 3F 08 02 00 00 00 00 00 00 06 00 00 00 02 00 00 00 03 00 00 00 2A 00 00 00 01 00 00 00 00 00 80 3F 9C 00 00 00 00 00 00 00 06 00 00 00 02 00 00 00 0D 00 00 00 26 00 00 00 01 00 00 00 00 00 80 3F 8E 01 00 00 00 00 00 00 06 00 00 00 02 00 00 00 01 00 00 00 2A 00 00 00 01 00 00 00 00 00 80 3F 87 00 00 00 00 00 00 00 06 00 00 00 02 00 00 00 02 00 00 00 2A 00 00 00 01 00 00 00 00 00 80 3F FC 00 00 00 00 00 00 00 06 00 00 00 02 00 00 00 05 00 00 00 2A 00 00 00 01 00 00 00 00 00 80 3F DE 00 00 00 00 00 00 00 06 00 00 00 02 00 00 00 18 00 00 00 24 00 00 00 01 00 00 00 00 00 80 3F 97 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";  
  return hexToBytes(data);
}
std::vector<char> hexToBytes(const std::string& hexStr)
{
    std::vector<char> bytes;
    bytes.reserve(hexStr.size() / 2);              // rough upper bound

    auto isHex = [](char c) {
        return std::isxdigit(static_cast<unsigned char>(c));
    };

    std::string nibble;   // collects two hex digits
    nibble.reserve(2);

    for (char c : hexStr) {
        if (std::isspace(static_cast<unsigned char>(c)))   // skip spaces/tabs/newlines
            continue;

        if (!isHex(c))
            throw std::invalid_argument("Non‑hex character in input");

        nibble.push_back(c);
        if (nibble.size() == 2) {
            // parse the two‑digit hex number
            char byte = static_cast<char>(std::strtol(nibble.c_str(), nullptr, 16));
            bytes.push_back(byte);
            nibble.clear();
        }
    }

    if (!nibble.empty())
        throw std::invalid_argument("Odd number of hex digits");

    return bytes;
}
