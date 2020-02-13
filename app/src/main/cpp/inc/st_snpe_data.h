
#ifndef _ST_SNPE_DATA_H_
#define _ST_SNPE_DATA_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LANDSCAPE = 0,
    PORTRAIT
}Model_Screen_Type;

typedef enum {
    CPU = 0,
    DSP,
    GPU,
    GPU_FLOAT16,
    AIX
}Snpe_Runtime_Type;

typedef struct {
    Snpe_Runtime_Type  runType;                 //snpe runtime type
    int                inputWidth;              //model input width
    int                inputHeight;             //model input height
    int                inputChannel;            //model input channels
    int                outputWidth;             //model output width
    int                outputHeight;            //model output height
    int                outputChannel;           //model output channels
    int                model_MemLen_H;
    int                model_MemLen_V;
    bool               isEncrypt;               //is encrypt
    int                reservedLocation;        //reserved district
}Model_PropDes;

#ifdef __cplusplus
}
#endif


#endif