//
// Created by yjs on 23-5-3.
//
#include <tencentcloud/core/Credential.h>
#include <tencentcloud/core/profile/ClientProfile.h>
#include <tencentcloud/core/profile/HttpProfile.h>
#include <tencentcloud/ocr/v20181119/OcrClient.h>
#include <tencentcloud/ocr/v20181119/model/GeneralAccurateOCRRequest.h>
#include <tencentcloud/ocr/v20181119/model/GeneralAccurateOCRResponse.h>
#include <iostream>
#include <string>
#include <fstream>
#include "include/base64.h"
#include "include/json.hpp"
#include <gflags/gflags.h>

#include <sys/stat.h>
#include "spdlog/cfg/env.h"
#include "spdlog/spdlog.h"
#include <cstdio>
#include <chrono>

using namespace std;
using json = nlohmann::json;
using namespace TencentCloud;
using namespace TencentCloud::Ocr::V20181119;
using namespace TencentCloud::Ocr::V20181119::Model;
using namespace gflags;

const string tencentKeysPath="/etc/keys/tencent_key.json";


inline int exist(const string name) {
    // if  exists return 1
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}


class OCRImages {
private:
    string secretId;
    string secretKey;
    string fileName;
    string content = "";

public:
    OCRImages(const string &name) : fileName(name) {}

    ~OCRImages() {}

    void Run() {
        this->loadCredentialKey();
        this->SendRequests();
    }

    int loadCredentialKey() {
        if (exist(tencentKeysPath)) {
            spdlog::info("{} exists ",tencentKeysPath);
            json tencent_key_json;
            ifstream tencent_key(tencentKeysPath);
            tencent_key >> tencent_key_json;
            this->secretId = tencent_key_json["secretId"].get<string>();
            this->secretKey = tencent_key_json["secretKey"].get<string>();
            spdlog::info("load secretId and secretKey success");

            return 0;
        } else {
            spdlog::error("{} not exists",tencentKeysPath);
            return 1;
        }
    }


    int SendRequests() {
        Credential cred = Credential(this->secretId, this->secretKey);
        HttpProfile httpProfile = HttpProfile();
        httpProfile.SetEndpoint("ocr.tencentcloudapi.com");
        ClientProfile clientProfile = ClientProfile();
        clientProfile.SetHttpProfile(httpProfile);
        OcrClient client = OcrClient(cred, "ap-beijing", clientProfile);
        FormulaOCRRequest req = FormulaOCRRequest();


        spdlog::info("this->fileName is {}", this->fileName);
        fstream f;
        f.open(this->fileName, ios::in | ios::binary);
        f.seekg(0, std::ios_base::end);
        std::streampos sp = f.tellg();
        int size = sp;
        char *buffer = (char *) malloc(sizeof(char) * size);
        f.seekg(0, std::ios_base::beg);
        f.read(buffer, size);
//        cout << "file size:" << size << endl;
        string imgBase64 = base64_encode(buffer, size);
//        cout << "img base64 encode size:" << imgBase64.size() << endl;
        req.SetImageBase64(imgBase64);
        auto outcome = client.FormulaOCR(req);
        if (!outcome.IsSuccess()) {
            cout << outcome.GetError().PrintAll() << endl;
            return -1;
        }
        FormulaOCRResponse resp = outcome.GetResult();
        string str = resp.ToJsonString();
        spdlog::info(str);



        auto j = json::parse(str);
//        for (auto  res:j["FormulaInfos"]){
//            spdlog::info(res[0]["DetectedText"].get<string>());
//        }

        this->content = j["FormulaInfos"][0]["DetectedText"].get<string>();



//        for (string res: DetectedTextList) {
//            this->content = this->content + res + '\n';
//        }

        f.close();
        spdlog::info("get info success");

        cout << endl;
        cout << endl;
        cout << this->content << endl;
        cout << endl;
        cout << endl;
        return 0;
    }


    string getCount() const {
        return this->content;
    }

};

DEFINE_string(i, "", "input the images name");
DEFINE_bool(f, false, "is output the images content to file");
DEFINE_bool(h, false, "get for help");

int main(int argc, char **argv) {
    spdlog::cfg::load_env_levels();


    ParseCommandLineFlags(&argc, &argv, true);
    if (FLAGS_h) {
        cout << endl;
        cout << "ocrimg -h 查看帮助" << endl;
        cout << "ocrimg -i=input.png 输入图片名字" << endl;
        cout << "ocrimg -i=input.png  -o=aa.txt 输入图片名字 输出文件识别内容到文件" << endl;
        cout << endl;
        return 1;
    }
    string filepath = string(FLAGS_i);
    if (FLAGS_i.empty()) {
        spdlog::error("mast input image name");
        return 1;
    }
    if (!exist(FLAGS_i)) {
        spdlog::error(" image name not exists");
        return 1;
    }
    spdlog::info("images name is {}", filepath);


    OCRImages *ocrImages = new OCRImages(filepath);
    ocrImages->Run();
    if (FLAGS_f) {
        string name;
        string fellname = string(FLAGS_i);
        string::size_type position = fellname.find('.');
        if (position != fellname.npos) {
            name = string(fellname, 0, position);
        }
        fstream f2;

        f2.open(name + ".txt", ios::out);
        f2 << ocrImages->getCount();
        f2.close();
    }
    delete ocrImages;

    ShutDownCommandLineFlags();
    return 0;
}
