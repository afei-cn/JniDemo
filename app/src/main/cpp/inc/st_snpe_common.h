
#ifndef _ST_SNPE_COMMON_H_
#define _ST_SNPE_COMMON_H_

#include "st_snpe_util.h"

// #define SNPE_DUMP_DEBUG
// #define SNPE_FLIP_ACTION

using namespace std;

typedef struct {
    int batch;
    int width;
    int height;
    int channel;
}Snpe_ShapeParam;

typedef struct {
    Snpe_ShapeParam modelInput_ShapeParam;
    Snpe_ShapeParam modelOut_ShapeParam;
}SnpeBidrect_ShapeParams;

typedef enum {
    PLAIN = 0,
    ENCRYPT_MODEL_FILE,
    ENCRYPT_MODEL_HEADER
}Model_EncryptType;

static std::vector<string> SNPE_Runtime_Item = {
    "CPU",
    "DSP",
    "GPU",
    "GPU_FLOAT16",
    "AIX"
};

static std::unordered_map<std::string, zdl::DlSystem::Runtime_t> SNPE_Runtime_Map = {
    { "CPU", zdl::DlSystem::Runtime_t::CPU },
    { "DSP", zdl::DlSystem::Runtime_t::DSP },
    { "GPU", zdl::DlSystem::Runtime_t::GPU },
    { "GPU_FLOAT16", zdl::DlSystem::Runtime_t::GPU_FLOAT16 },
    { "AIX", zdl::DlSystem::Runtime_t::AIP_FIXED8_TF }
};

static std::unordered_map<std::string, zdl::DlSystem::PerformanceProfile_t> SNPE_Perform_Map = {
    { "DEFAULT",  zdl::DlSystem::PerformanceProfile_t::DEFAULT },
    { "BALANCED", zdl::DlSystem::PerformanceProfile_t::BALANCED },
    { "HIGH_PERFORMANCE", zdl::DlSystem::PerformanceProfile_t::HIGH_PERFORMANCE },
    { "POWER_SAVER", zdl::DlSystem::PerformanceProfile_t::POWER_SAVER },
    { "SYSTEM_SETTINGS", zdl::DlSystem::PerformanceProfile_t::SYSTEM_SETTINGS },
    { "SUSTAINED_HIGH_PERFORMANCE", zdl::DlSystem::PerformanceProfile_t::SUSTAINED_HIGH_PERFORMANCE }
};

#endif