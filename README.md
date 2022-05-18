<!-- PROJECT SHIELDS -->

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]

# PaddleOCRJni

## 介绍

本项目是对 [PaddleOCR](https://github.com/PaddlePaddle/PaddleOCR/tree/release/2.5/deploy/cpp_infer) （C++ 部署版本）的一个简单 JNI 封装，主要是为了方便 Java 服务端部署，更多资料请参考 PaddleOCR 文档。
该项目编译后生成的动态库需要配合 [OcrLite](https://github.com/greycode/ocrlite) 一起使用。


## 使用

 1. 下载 Release 中编译好的平台相关的动态库，放到 Java 项目 resource 中（保证编译后在 classes 目录）
 2. 添加 Maven 依赖
    ```xml
    <dependency>
      <groupId>io.github.greycode</groupId>
      <artifactId>ocrlite</artifactId>
      <version>3.0.0</version>
    </dependency>
    ```
 3. 运行
    ```java
    // 注意请务必使用绝对路径
    String imagePath = "run-test/test_imgs/c.png";

    OcrDriver.initialize();
    List<List<OCRPredictResult>> ocrResult = OcrDriver.ocr(imagePath);
    ```

## 自定义编译

### 准备环境

#### 下载代码

```shell
git clone https://github.com/greycode/PaddleOCRJni.git
```

#### OpenCV

直接下载 Release 中编译好的库，解压到 opencv-static 目录

> 如果需要自行编译，请参考 [PaddleOCR-【服务器端C++预测】-【1.2 编译opencv库】](https://github.com/PaddlePaddle/PaddleOCR/blob/release/2.5/deploy/cpp_infer/readme_ch.md#12)部分

#### paddle_inference

直接下载 Release 中编译好的库，解压到 paddle_inference_install_dir 目录

> 如果需要自行编译，请参考 [PaddleOCR-【服务器端C++预测】-【1.3 下载或者编译Paddle预测库】](https://github.com/PaddlePaddle/PaddleOCR/blob/release/2.5/deploy/cpp_infer/readme_ch.md#13)部分

### 编译

根据环境运行脚本

**Linux 或 Mac OS**
```shell
./build.sh
```

**Window**

```shell
./build.bat
```

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/greycode/ocrlite.svg?style=for-the-badge
[contributors-url]: https://github.com/greycode/ocrlite/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/greycode/ocrlite.svg?style=for-the-badge
[forks-url]: https://github.com/greycode/ocrlite/network/members
[stars-shield]: https://img.shields.io/github/stars/greycode/ocrlite.svg?style=for-the-badge
[stars-url]: https://github.com/greycode/ocrlite/stargazers
[issues-shield]: https://img.shields.io/github/issues/greycode/ocrlite.svg?style=for-the-badge
[issues-url]: https://github.com/greycode/ocrlite/issues
[license-shield]: https://img.shields.io/github/license/greycode/ocrlite.svg?style=for-the-badge
[license-url]: https://github.com/greycode/ocrlite/blob/master/LICENSE
