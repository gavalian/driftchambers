#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "drift3.h"
#include <random>

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
    data[indx-1] = (float) 1.0;
  }
}

void print(std::vector<float> &desired, std::vector<float> &infered){
  for(int i = 0; i < desired.size(); i++){
    if(desired[i]>0.01)
      printf("%8d : %9.6f %9.6f\n",i,desired[i],infered[i]);
    //if(infered[i]>0.00001) printf("%8d : %9.6f %9.6f\n",i,desired[i],infered[i]);
  }
}

void addnoise(std::vector<float> &data){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, data.size());
  for(int i = 0; i < 10; i++){
    int random_number = distrib(gen);
    data[random_number] = 1.0;
    //printf("%d %d\n",i,random_number);
  }
}

void print2( std::vector<float> &vec, std::vector<float> &desired, std::vector<float> &infered){
  for(int i = 0; i < desired.size(); i++){
    if(vec[i]>0.01&&desired[i]>0.5)
      printf("%8d,  %9.6f, %9.6f, %9.6f\n",i,vec[i],desired[i],infered[i]);
    //if(infered[i]>0.00001) printf("%8d : %9.6f %9.6f\n",i,desired[i],infered[i]);
  }
}
void print3( std::vector<float> &vec, std::vector<float> &desired, std::vector<float> &infered){
  for(int i = 0; i < desired.size(); i++){
    if(vec[i]>0.01&&desired[i]<0.5)
      printf("%8d,  %9.6f, %9.6f, %9.6f\n",i,vec[i],desired[i],infered[i]);
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
std::string toCSV( std::vector<float>& values) {
    std::ostringstream oss;
    for (size_t i = 0; i < values.size(); ++i) {
        oss << values[i];
        if (i != values.size() - 1) {
            oss << ",";
        }
    }
    return oss.str();
}
void write(std::vector<float> &vec0, std::vector<float> &vec1,std::vector<float> &vec2){
  std::string s0 = toCSV(vec0);
  std::string s1 = toCSV(vec1);
  std::string s2 = toCSV(vec2);
  printf("%s\n",s0.c_str());
  printf("%s\n",s1.c_str());
  printf("%s\n",s2.c_str());
}
int count_back(std::vector<float> &vec,std::vector<float> &des){
  int counter = 0;
  for(int i = 0; i < vec.size(); i++){
    if(vec[i]>0.5&&des[i]<0.5) counter++;
  }
  return counter;
}

int count_back_inf(std::vector<float> &vec,std::vector<float> &des,std::vector<float> &infr, double thr ){
  int counter = 0;
  for(int i = 0; i < vec.size(); i++){
    if(vec[i]>0.5&&des[i]<0.5&&infr[i]>thr) counter++;
  }
  return counter;
}

int count_sig(std::vector<float> &des,std::vector<float> &infr, double thr ){
  int counter = 0;
  for(int i = 0; i < des.size(); i++){
    if(des[i]>0.5&&infr[i]>thr) counter++;
  }
  return counter;
}

void modify(std::vector<float> &vec,std::vector<float> &infr, double thr){
  for(int i = 0; i < vec.size(); i++){
    if(vec[i]>0.5){
      if(infr[i]>thr) infr[i]=1.0; else infr[i] = 0.0;
    } else {infr[i] = 0.0;}
  }
}

int main() {

  std::string filename = "denoiser_sector.lsvm";
    auto dataset = parseLSVM(filename);

    dc3_init("cnn_autoenc_504x36.json");

    
    std::vector<float> vec;
    std::vector<float> vecin;
    std::vector<float> des;

    vec.resize(504*36);
    des.resize(504*36);
    vecin.resize(504*36);
    // Print first few samples
    for (size_t i = 0; i < dataset.size() && i < 5120; i+=2) {

      fill(vec,dataset[i]);
      fill(vecin,dataset[i]);
      //addnoise(vec);
      int total = count(vec,0.1);
      fill(des,dataset[i+1]);
      dc5_process(vec);
      
      int surv = count(vec,0.1);
      int hits = count(des,0.1);

      int coin = count(vecin,0.1);
      int c = compare(des,vec,0.12);
      int cnt = count_sig(des,vec,0.005);
      if(cnt>21){
	//printf("%8d %8d %8d --> %8d %8d \n",
	//       count_back(vecin,des),count_back_inf(vecin,des,vec,0.002),count_back_inf(vecin,des,vec,0.005),
	//       count_sig(des,vec,0.002),count_sig(des,vec,0.005));
	modify(vecin,vec,0.005);
	write(vecin,vec,des);
      }
      //if(c==1){
      //printf("---- new instance total = %d, desired = %d, infered = %d, total = %d\n",
      //	     total, hits,surv,coin);
      //print(des,vec);
      //if(coin>24){
      //print2(vecin,des,vec);
	//print3(vecin,des,vec);
	//}
	//}

      //write(vecin,vec,des);
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
