//****************************************************************/
//      ·▄▄▄▄  ▄▄▄ . ▐ ▄       ▪  .▄▄ · ▄▄▄ .▄▄▄  
//      ██▪ ██ ▀▄.▀·•█▌▐█▪     ██ ▐█ ▀. ▀▄.▀·▀▄ █·
//      ▐█· ▐█▌▐▀▀▪▄▐█▐▐▌ ▄█▀▄ ▐█·▄▀▀▀█▄▐▀▀▪▄▐▀▀▄ 
//      ██. ██ ▐█▄▄▌██▐█▌▐█▌.▐▌▐█▌▐█▄▪▐█▐█▄▄▌▐█•█▌
//      ▀▀▀▀▀•  ▀▀▀ ▀▀ █▪ ▀█▄▀▪▀▀▀ ▀▀▀▀  ▀▀▀ .▀  ▀
//****************************************************************/
// Author: G.Gavalian (02/2025)
//================================================================

#include "drift3.h"

std::unique_ptr<fdeep::model> neuralNetwork;
dc::drift3  chamber;

void dc3_init(const char *modelFile){
  const auto modelLocal = fdeep::load_model(modelFile); 
  neuralNetwork = std::make_unique<fdeep::model>(std::move(modelLocal));
  printf("\n**** UPDATED LIBRARY: INCLUDES DC4_PROCESS FOR DENOISING 4 ****\n\n");
}

//std::unique_ptr<fdeep::model> dc3_model(){ return neuralNetwork;}
void dc3_process(const char *leaf){
  chamber.process(*neuralNetwork,leaf);
}

void dc4_process(const char *leaf){
  chamber.process4(*neuralNetwork,leaf);
}

void dc5_process(std::vector<float> &data){
  fdeep::tensors result = (*neuralNetwork).predict(
					      { fdeep::tensor(
			 fdeep::tensor_shape(static_cast<std::size_t>(504),
				       static_cast<std::size_t>(36),
				       static_cast<std::size_t>(1)),data)});
  auto output = result[0].as_vector();

  for(int i = 0; i < data.size(); i++) data[i] = (*output)[i];
}

namespace dc {

  int  drift3::getIndex(int layer, int component){
    int index = (layer-1)*dcWires + (component-1);
    return index;
  }
  
  void  drift3::initvec(std::vector<float> &tensor){
    if(((int) tensor.size())!=dcWires*36) tensor.resize(dcWires*36);
    for(int i = 0; i < dcWires*36; i++) tensor[i] = 0.0;
  }
  
  void  drift3::create(std::vector<float> &tensor, char *buffer, int sector, int start, int rowSize, int nrows){
      //int nrows = 128;      
      for(int i = 0; i < nrows; i++){	
         int offset = start + i*rowSize;
         int    sec = *reinterpret_cast<int*>(&buffer[offset+4]);
	 int  order = *reinterpret_cast<int*>(&buffer[offset+16]);
         if(sec==sector&&order>0){
            int layer = *reinterpret_cast<int*>(&buffer[offset+8]);
            int  wire = *reinterpret_cast<int*>(&buffer[offset+12]);
            int position = getIndex(layer,wire);
            tensor[position] = 1.0;
         }
      }
  }

  int drift3::process4(const fdeep::model &model, const char *buffer){
        std::vector<float>  cmat;
	char *data = const_cast<char*> (buffer);
	int  group = *reinterpret_cast<uint16_t*>(&data[0]);
	int   item = *reinterpret_cast< uint8_t*>(&data[2]);
	int   type = *reinterpret_cast< uint8_t*>(&data[3]);
	int   word = *reinterpret_cast<uint32_t*>(&data[4]);
	
	int   format = (word>>24)&0x000000FF;
	int   length = word&0x00FFFFFF;
	format = 1;
	length = 90*260*4+1;
	//printf("%d, %d, %d %d %d\n",group,item, type, format, length);
	int     size = length - format;
	int    nrows = size/(4);
	if(nrows!=90*260) { printf("dc4_process:: error :: the rows of the node is %d, should have been %d\n",
				   nrows,90*260); return -1; }
	cmat.resize(90*260);
	for(int i = 0; i < 90*260; i++){
	  float value = *reinterpret_cast<float*>(&data[8+format+i*4]); 
	  cmat[i] = value>0.00001?1.0:0.0;
	}

	fdeep::tensors result = model.predict(
					      { fdeep::tensor(
		   fdeep::tensor_shape(static_cast<std::size_t>(90),
				       static_cast<std::size_t>(260),
				       static_cast<std::size_t>(1)),cmat)});
	 auto output = result[0].as_vector();
	 
	 for(int i = 0; i < 90*260; i++){
	   *reinterpret_cast<float*>(&data[8+format+i*4]) = (*output)[i];
	 }

	 return 0;
  }
  
  int drift3::process(const fdeep::model &model, const char *buffer){
    std::vector<float>  dcmat;
    char *data = const_cast<char*> (buffer);
    int  group = *reinterpret_cast<uint16_t*>(&data[0]);
    int   item = *reinterpret_cast< uint8_t*>(&data[2]);
    int   type = *reinterpret_cast< uint8_t*>(&data[3]);
    int   word = *reinterpret_cast<uint32_t*>(&data[4]);

    int   format = (word>>24)&0x000000FF;
    int   length = word&0x00FFFFFF;
    //printf("%d, %d, %d %d %d\n",group,item, type, format, length);
    int     size = length - format;
    int    nrows = size/(8*4); 
    for(int s = 1; s <= 6; s++){
      initvec(dcmat);
      create(dcmat,data,s,8+format,8*4,nrows);
      //printf("--- processing sector %d array size = %d\n",s, (int) dcmat.size());       
      fdeep::tensors result = model.predict(
	   { fdeep::tensor(
	     fdeep::tensor_shape(static_cast<std::size_t>(36),
	                         static_cast<std::size_t>(dcWires),
				 static_cast<std::size_t>(1)),dcmat)});
      auto output = result[0].as_vector();  
      
      for(int k = 0; k < nrows; k++){
         int offset = 8+format + k*(8*4);
         int sect = *reinterpret_cast<uint32_t*>(&data[offset+4]);
         int layr = *reinterpret_cast<uint32_t*>(&data[offset+8]);
         int wire = *reinterpret_cast<uint32_t*>(&data[offset+12]);
         if(sect==s){
            int  index = getIndex(layr,wire);
            float prob = (*output)[index];
            *reinterpret_cast<float*>(&data[offset+20]) = prob;
         }
      }
    }


    return 1;
  }
}
