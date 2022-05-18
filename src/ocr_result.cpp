#ifdef __JNI__

#include "ocr_lite_port.h"
#include "ocr_result.h"
#include "utility.h"

using namespace PaddleOCR;

OcrResultUtils::OcrResultUtils(JNIEnv *env, std::vector<std::vector<PaddleOCR::OCRPredictResult>> &results) {
    jniEnv = env;
    jPdResult = getAllOCRPredictResultList(results);
}

OcrResultUtils::~OcrResultUtils() {
    jniEnv = NULL;
}

jobject OcrResultUtils::getJPdObject() {
    return jPdResult;
}

jclass OcrResultUtils::newJListClass() {
    jclass clazz = jniEnv->FindClass("java/util/ArrayList");
    if (clazz == NULL) {
        printf("ArrayList class is null\n");
        return NULL;
    }
    return clazz;
}

jmethodID OcrResultUtils::getListConstructor(jclass clazz) {
    jmethodID constructor = jniEnv->GetMethodID(clazz, "<init>", "()V");
    return constructor;
}


jintArray OcrResultUtils::newPdbox(std::vector<int> &box) {
    jintArray jBox = jniEnv->NewIntArray(box.size());
    jniEnv->SetIntArrayRegion(jBox, 0, box.size(), (jint *) box.data());
    return jBox;
}

jobject OcrResultUtils::newPdBoxes(std::vector<std::vector<int>> &boxes) {
    jclass jListClass = newJListClass();
    jmethodID jListConstructor = getListConstructor(jListClass);
    jobject jList = jniEnv->NewObject(jListClass, jListConstructor);
    jmethodID jListAdd = jniEnv->GetMethodID(jListClass, "add", "(Ljava/lang/Object;)Z");

    for (int i = 0; i < boxes.size(); ++i) {
        auto box = boxes[i];
        jobject jBox = newPdbox(box);
        jniEnv->CallBooleanMethod(jList, jListAdd, jBox);
    }
    return jList;
}

jobject OcrResultUtils::getOCRPredictResult(PaddleOCR::OCRPredictResult &result) {
    jobject jBox = newPdBoxes(result.box);
    jstring jText = jniEnv->NewStringUTF(result.text.c_str());
    jfloat jScore = (jfloat) result.score;
    jfloat jClsScore = (jfloat) result.cls_score;
    jdouble jClsLabel = (jint) result.cls_label;

    jclass clazz = jniEnv->FindClass("io/github/greycode/OCRPredictResult");
    if (clazz == NULL) {
        printf("OCRPredictResult class is null\n");
        return NULL;
    }
    jmethodID constructor = jniEnv->GetMethodID(clazz, "<init>",
                                                "(Ljava/util/List;Ljava/lang/String;FFI)V");
    jobject obj = jniEnv->NewObject(clazz, constructor, jBox, jText,
                                    jScore, jClsScore, jClsLabel);
    return obj;
}

jobject OcrResultUtils::getOCRPredictResultList(std::vector<PaddleOCR::OCRPredictResult> &result) {
    jclass jListClass = newJListClass();
    jmethodID jListConstructor = getListConstructor(jListClass);
    jobject jList = jniEnv->NewObject(jListClass, jListConstructor);
    jmethodID jListAdd = jniEnv->GetMethodID(jListClass, "add", "(Ljava/lang/Object;)Z");

    for (int i = 0; i < result.size(); ++i) {
        auto rst = result[i];
        jobject jRst = getOCRPredictResult(rst);
        jniEnv->CallBooleanMethod(jList, jListAdd, jRst);
    }
    return jList;
}

jobject OcrResultUtils::getAllOCRPredictResultList(std::vector<std::vector<OCRPredictResult>> &results) {
    jclass jListClass = newJListClass();
    jmethodID jListConstructor = getListConstructor(jListClass);
    jobject jList = jniEnv->NewObject(jListClass, jListConstructor);
    jmethodID jListAdd = jniEnv->GetMethodID(jListClass, "add", "(Ljava/lang/Object;)Z");

    for (int i = 0; i < results.size(); ++i) {
        auto result = results[i];
        jobject jResult = getOCRPredictResultList(result);
        jniEnv->CallBooleanMethod(jList, jListAdd, jResult);
    }
    return jList;
}

#endif