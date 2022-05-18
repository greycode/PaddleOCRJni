#ifdef __JNI__
#ifndef __OCR_RESULT_UTILS_H__
#define __OCR_RESULT_UTILS_H__

#include <jni.h>
#include "ocr_lite_port.h"
#include "utility.h"

class OcrResultUtils {
public:

    OcrResultUtils(JNIEnv *env, std::vector<std::vector<PaddleOCR::OCRPredictResult>> &results);

    ~OcrResultUtils();

    jobject getJPdObject();

private:
    JNIEnv *jniEnv;

    jobject jPdResult;

    jclass newJListClass();

    jmethodID getListConstructor(jclass clazz);

    jintArray newPdbox(std::vector<int> &box);

    jobject newPdBoxes(std::vector<std::vector<int>> &boxes);

    jobject getOCRPredictResult(PaddleOCR::OCRPredictResult &result);

    jobject getOCRPredictResultList(std::vector<PaddleOCR::OCRPredictResult> &result);

    jobject getAllOCRPredictResultList(std::vector<std::vector<PaddleOCR::OCRPredictResult>> &results);

};

#endif //__OCR_RESULT_UTILS_H__
#endif
