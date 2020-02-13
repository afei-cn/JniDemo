
#ifndef _ST_SNPE_UTIL_H_
#define _ST_SNPE_UTIL_H_

#include <algorithm>
#include <chrono>
#include <exception>
#include <iostream>
#include <numeric>
#include <string>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <vector>
#include <fstream>

#include <zdl/SNPE/SNPE.hpp>
#include <zdl/SNPE/SNPEBuilder.hpp>
#include <zdl/SNPE/SNPEFactory.hpp>
#include <zdl/DlContainer/IDlContainer.hpp>
#include <zdl/DlSystem/ITensorFactory.hpp>
#include <zdl/DlSystem/TensorShape.hpp>
#include <zdl/DlSystem/DlError.hpp>

#include "st_snpe_api.h"
#include "st_snpe_common.h"

using namespace std;


const static char MODELENCRYPT_KEY  = 'c';
const static char DUMP_PATH[] = "/mnt/sdcard/DCIM/ST_SNPE/dump";

#ifdef SNPE_DUMP_DEBUG
    void dump_SNPE_input(std::vector<float> input, const char* dump_path);
    void dump_Tensor_Output(const zdl::DlSystem::ITensor* tensor, const char* filePath);
#endif

zdl::DlSystem::Runtime_t checkRuntime_SNPE(zdl::DlSystem::Runtime_t runtime);

#ifdef WATCH_MODEL_DIMENS
    void glimpse_InputTensor_Dimen(zdl::DlSystem::ITensor* inTensor);
#endif

#ifdef SNPE_FLIP_ACTION
void resequence_BufferBy_Channel(std::vector<float>& src_vec,
                                 int channel,
                                 int inWidth, 
                                 int inHeight,
                                 Model_Screen_Type screen_orientation);
#endif                                 

std::unique_ptr<zdl::SNPE::SNPE> getSNPE_Handle(Snpe_Runtime_Type runType,
                                                std::unique_ptr<zdl::DlContainer::IDlContainer>& container,
                                                bool fallback,
                                                const char* performance);

#ifdef DUMP_MODEL_CACHE_MACRO
void saveEncryptCacheModel(std::string h_plain_container, std::string v_plain_container,
                           std::string encypt_cacheModel_H, std::string encrypt_cacheMOdel);
#endif

#endif