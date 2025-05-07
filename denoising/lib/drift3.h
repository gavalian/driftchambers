//*******************************************************************************************************
//
// ░▒▓███████▓▒░░▒▓████████▓▒░▒▓███████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░░▒▓███████▓▒░▒▓████████▓▒░▒▓███████▓▒░    *
// ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░   *
// ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░   *
// ░▒▓█▓▒░░▒▓█▓▒░▒▓██████▓▒░ ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓██████▓▒░░▒▓██████▓▒░ ░▒▓███████▓▒░    *
// ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░   *
// ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░   *
// ░▒▓███████▓▒░░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░░▒▓█▓▒░▒▓███████▓▒░░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░   *
// 
//*******************************************************************************************************
//- Denosing Auto-Encoder, for denosing CLAS12 drift Chambers
//- Author: G.Gavalian (02/2025)
//*******************************************************************************************************


#include <fdeep/fdeep.hpp>
#include <vector>

#ifndef __DC_DRIFT3__
#define __DC_DRIFT3__


extern "C" {
  void dc3_init(const char *modelFile);
  void dc3_process(const char *buffer);
}

namespace dc {

  class drift3 {
    
  private:

    int dataOffset = 12; // this is where the data starts in the leaf
    int rowSize    = 4*6; // important parameter for parsin the leaf

    int      dcWires = 112;
    double threshold = 0.05;
    int  getIndex(int layer, int component);

  public:
    
    drift3(){}
    drift3(double __th){threshold = __th;}
    virtual ~drift3(){}
    void  setRows(int rows) { rowSize = rows;}        
    void  initvec(std::vector<float> &tensor);  
    void  create(std::vector<float> &tensor, char *buffer, int sector, int start, int rowSize, int nrows);  
    //void  create( std::vector<float> &tensor, const char *buffer, int sector);
    int   process( const fdeep::model &model, const char *buffer);
  };
}
/*
const  fdeep::model   *neuralModel;
dc::drift             driftChamber;

extern "C" {
  void loadModel(const char *modleFile);
  void process(hipo::composite &bank);
}

void loadModel(const char *modelFile){
  const auto modelLocal = fdeep::load_model(modelFile);
  //const auto modelLocal = fdeep::load_model("network/cnn_autoenc_cpp.json");
  neuralModel = &modelLocal;
}

void process(hipo::composite &bank){
  driftChamber.processLeaf(*neuralModel,bank);
}*/
#endif
