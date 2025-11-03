#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "drift3.h"

// Struct to hold one data sample
struct LSVMSample {
    int label;
    std::vector<std::pair<int, double>> features;
};

// Function to parse an LSVM file into a vector of samples
std::vector<LSVMSample> parseLSVM(const std::string &filename) {
    std::vector<LSVMSample> data;
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return data;
    }

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        LSVMSample sample;

        // read label (first token)
        iss >> sample.label;

        // read features (index:value pairs)
        std::string token;
        while (iss >> token) {
            size_t pos = token.find(':');
            if (pos != std::string::npos) {
                int index = std::stoi(token.substr(0, pos));
                double value = std::stod(token.substr(pos + 1));
                sample.features.emplace_back(index, value);
            }
        }

        data.push_back(std::move(sample));
    }
    return data;
}

// Example usage
void fill(std::vector<float> &data, LSVMSample &lsvm ){
  for(int i = 0; i < (int) data.size(); i++){
    data[i] = 0.0;
  }
  
  for(int i = 0; i < lsvm.features.size(); i++){
    int indx = lsvm.features[i].first;
    data[indx] = (float) 1.0;
  }
}

void print(std::vector<float> &desired, std::vector<float> &infered){
  for(int i = 0; i < desired.size(); i++){
    if(desired[i]>0.01)
      printf("%8d : %9.6f %9.6f\n",i,desired[i],infered[i]);
    //if(infered[i]>0.00001) printf("%8d : %9.6f %9.6f\n",i,desired[i],infered[i]);
  }
}

int compare(std::vector<float> &desired, std::vector<float> &infered, double thr){
    int c = 0;
    int d = 0;
    for(int i = 0; i < desired.size(); i++){
      if(desired[i]>0.2) d++;
      if(desired[i]>0.2&&infered[i]>thr) c++;
    }
    return d==c?1:0;
}
  
int count(std::vector<float> &vec, double thrs){
  int counter = 0;
  for(int i = 0; i < vec.size(); i++)
    if(vec[i]>thrs) counter++;
  return counter;
}

int main() {
    std::string filename = "xag.lsvm";
    auto dataset = parseLSVM(filename);

    dc3_init("network/cnn_autoenc_258x88_0e_n.json");
    
    std::vector<float> vec;
    std::vector<float> des;

    vec.resize(258*88);
    des.resize(258*88);
    // Print first few samples
    for (size_t i = 0; i < dataset.size() && i < 5120; i+=2) {

      fill(vec,dataset[i]);
      int total = count(vec,0.1);
      
      fill(des,dataset[i+1]);
      
      dc5_process(vec);
      int surv = count(vec,0.1);
      int hits = count(des,0.1);

      int c = compare(des,vec,0.12);
      //if(c==1){
      printf("---- new instance total = %d, desired = %d, infered = %d\n",
	     total, hits,surv);
      print(des,vec);
	//}
      /*
      std::cout << "Label: " << dataset[i].label << " Features:";
        for (const auto &f : dataset[i].features) {
            std::cout << " (" << f.first << ":" << f.second << ")";
        }
        std::cout << "\n";
      */
    }


    
    return 0;
}
