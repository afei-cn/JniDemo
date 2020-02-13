#include "com_afei_jnidemo_NativeLibrary.h"
#include "LogUtil.h"
#include "st_snpe_data.h"
#include "st_snpe_common.h"
#include "st_snpe_api.h"
#include "st_snpe_util.h"

JNIEXPORT void JNICALL
Java_com_afei_jnidemo_NativeLibrary_test(JNIEnv *env, jclass type) {
    const char *ADSP_LIBRARY_PATH = "/system/lib/rfsa/adsp;/system/vendor/lib/rfsa/adsp;/dsp/adsp;/dsp/cdsp";
    bool success = setenv("ADSP_LIBRARY_PATH", ADSP_LIBRARY_PATH, 1);
    LOGD("setenv: %d", success);

    const char *h_model_path = "/sdcard/large_dsp_481_289_h_encrypt.model";
    const char *v_model_path = "/sdcard/large_dsp_481_289_v_encrypt.model";
    LOGD("start create, h_model path: %s, v_model_path: %s", h_model_path, v_model_path);

    Model_PropDes snpeRunning_ModelProp = {(Snpe_Runtime_Type)1, 481, 289, 3,       241,       145, 2, 6834079, 6828847, true,  0};

    LOGD("start create handle\n");
    ST_SNPE_API* snpe_api_handle = ST_SNPE_API::create(
            snpeRunning_ModelProp,
            h_model_path,
            v_model_path
    );
    LOGD("end create handle: %p\n", snpe_api_handle);
    const char* version = snpe_api_handle->getSNPE_Version();
    LOGD("version: %s\n", version);

}
