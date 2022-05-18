#ifdef __JNI__

#include <jni.h>
#include "ocr_result.h"
#include <chrono>

#include <include/args.h>
#include <include/paddleocr.h>

#include "utility.h"

using namespace PaddleOCR;

static PPOCR *ppocr;

void throwJavaException(JNIEnv *env);

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    int argc = 2;
    char arg0[] = "ppocr";
    char arg1[] = ".";
    char **argv = new char *[argc + 1]{arg0, arg1};

    google::ParseCommandLineFlags(&argc, &argv, true);

    delete[] argv;

    return JNI_VERSION_1_4;
}

JNIEXPORT void JNICALL
JNI_OnUnload(JavaVM *vm, void *reserved) {
    // printf("JNI_OnUnload\n");
    if (ppocr) {
        delete ppocr;
    }
}

#ifdef _WIN32
char *jstringToChar(JNIEnv *env, jstring jstr)
{
    char *rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("gbk");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0)
    {
        rtn = (char *)malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}
#else

char *jstringToChar(JNIEnv *env, jstring input) {
    char *str = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(input, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        str = (char *) malloc(alen + 1);
        memcpy(str, ba, alen);
        str[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return str;
}

#endif

extern "C" JNIEXPORT jboolean JNICALL
Java_io_github_greycode_PaddleOcr_initLite(JNIEnv *env, jobject thiz,
                                           jboolean use_det, jboolean use_cls, jboolean use_angle_cls, jboolean use_rec,
                                           jstring det_model_dir, jstring cls_model_dir, jstring rec_model_dir,
                                           jstring rec_char_dict_path) {
    std::string _det_model_dir = jstringToChar(env, det_model_dir);
    std::string _cls_model_dir = jstringToChar(env, cls_model_dir);
    std::string _rec_model_dir = jstringToChar(env, rec_model_dir);
    std::string _rec_char_dict_path = jstringToChar(env, rec_char_dict_path);

    ppocr = new PPOCR(
            use_det,
            use_cls,
            use_angle_cls,
            use_rec,
            _det_model_dir,
            _cls_model_dir,
            _rec_model_dir,
            _rec_char_dict_path
    );
    return JNI_TRUE;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_io_github_greycode_PaddleOcr_initFull(JNIEnv *env, jobject thiz,
                                           jboolean use_det, jboolean use_cls, jboolean use_angle_cls, jboolean use_rec,
                                           jstring det_model_dir, jstring cls_model_dir, jstring rec_model_dir,
                                           jstring image_dir,
                                           jboolean use_gpu, jint gpu_id, jint gpu_mem,
                                           jint cpu_math_library_num_threads, jboolean use_mkldnn,
                                           jint max_side_len, jdouble det_db_thresh, jdouble det_db_box_thresh,
                                           jdouble det_db_unclip_ratio, jstring det_db_score_mode,
                                           jboolean use_dilation, jdouble cls_thresh, jint cls_batch_num,
                                           jstring label_path, jint rec_batch_num, jint rec_img_h, jint rec_img_w,
                                           jboolean use_tensorrt, jstring precision) {
    std::string _det_model_dir = jstringToChar(env, det_model_dir);
    std::string _cls_model_dir = jstringToChar(env, cls_model_dir);
    std::string _rec_model_dir = jstringToChar(env, rec_model_dir);
    std::string _image_dir = jstringToChar(env, image_dir);
    std::string _det_db_score_mode = jstringToChar(env, det_db_score_mode);
    std::string _label_path = jstringToChar(env, label_path);
    std::string _precision = jstringToChar(env, precision);

    ppocr = new PPOCR(use_det, use_cls, use_angle_cls, use_rec,
                      _det_model_dir, _cls_model_dir, _rec_model_dir,
                      _image_dir,

                      use_gpu,
                      gpu_id, gpu_mem,
                      cpu_math_library_num_threads,
                      use_mkldnn,

                      max_side_len,
                      det_db_thresh,
                      det_db_box_thresh,
                      det_db_unclip_ratio,
                      _det_db_score_mode,
                      use_dilation,

                      cls_thresh,
                      cls_batch_num,

                      _label_path,
                      rec_batch_num, rec_img_h,
                      rec_img_w,

                      use_tensorrt, _precision);
    return JNI_TRUE;
}

extern "C" JNIEXPORT jobject
JNICALL Java_io_github_greycode_PaddleOcr_ocr(JNIEnv *env, jobject thiz,
                                              jstring image_dir, jboolean use_det, jboolean use_rec, jboolean use_cls) {
    try {
        std::string modelsDir = jstringToChar(env, image_dir);

        std::cout << "image path: " << modelsDir << endl;

        std::vector<cv::String> cv_all_img_names;
        cv::glob(modelsDir, cv_all_img_names);
        // std::cout << "total images num: " << cv_all_img_names.size() << endl;

        using clock = std::chrono::system_clock;
        //using sec = std::chrono::duration<double>;
        using ms = std::chrono::duration<double, std::milli>;
        const auto before = clock::now();

        std::vector<std::vector<OCRPredictResult>> ocr_results =
                ppocr->ocr(cv_all_img_names, use_det, use_rec, use_cls);
        const ms duration = clock::now() - before;
        std::cout << "image path: [" << modelsDir << "] tooks " << duration.count() << "ms" << std::endl;

        return OcrResultUtils(env, ocr_results).getJPdObject();
    }
    catch (...) {
        throwJavaException(env);
    }

}

void throwJavaException(JNIEnv *env) {
    jclass c = env->FindClass("io/github/greycode/OcrException");
    if (NULL == c) {
        c = env->FindClass("java/lang/NullPointerException");
    }
    env->ThrowNew(c, "unknown jni error!");
}

#endif