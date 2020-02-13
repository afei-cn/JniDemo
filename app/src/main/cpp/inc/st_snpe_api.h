
#ifndef _ST_SNPE_API_H_
#define _ST_SNPE_API_H_

#ifndef ST_VISIBILITY_PUBLIC
#define ST_VISIBILITY_PUBLIC __attribute__((visibility ("default")))
#endif

#include <vector>
#include "st_snpe_data.h"

#ifdef __cplusplus
extern "C"{
#endif

using namespace std;

class ST_VISIBILITY_PUBLIC ST_SNPE_API {
public:
    static ST_SNPE_API* create(
        Model_PropDes prop,
        const char* hModel_Path,
        const char* vModel_Path);

    void executeSNPE(std::vector<float>& seg_img_in,
                     std::vector<float>& seg_img_out,
                     Model_Screen_Type screenType);

    void destroy();
    
    const char* getSNPE_Version();
    
protected:
    ST_SNPE_API();
    virtual ~ST_SNPE_API();

private:
    ST_SNPE_API(const ST_SNPE_API&) = delete;               ///< Disallow the copy constructor
    ST_SNPE_API& operator=(const ST_SNPE_API&) = delete;    ///< Disallow assignment operator
};

#ifdef __cplusplus
}
#endif

#endif