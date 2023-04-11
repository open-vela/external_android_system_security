/*
 * Copyright (C) 2023 Xiaomi Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "keystore/client.h"
#include <base/command_line.h>
#include <binder/IPCThreadState.h>

using base::CommandLine;

void printUsageAndExit()
{
    printf("useage:\n"
        "\tkeystore_tool keyStoreInsert --name=test-name --data=test-data\n"
        "\tkeystore_tool keyStoreGet --name=test-name\n"
        "\tkeystore_tool keyStoreExist --name=test-name\n"
        "\tkeystore_tool keyStoreDel --name=test-name\n"
        "\tkeystore_tool keyStoreReset\n");
}

int logReturnCode(int code)
{
    char buffer[128] = { 0 };
    switch (code) {
    case KEYSTORE_NO_ERROR:
        sprintf(buffer, "KEYSTORE_NO_ERROR");
        break;
    case KEYSTORE_LOCKED:
        sprintf(buffer, "KEYSTORE_LOCKED");
        break;
    case KEYSTORE_UNINITIALIZED:
        sprintf(buffer, "KEYSTORE_UNINITIALIZED");
        break;
    case KEYSTORE_SYSTEM_ERROR:
        sprintf(buffer, "KEYSTORE_SYSTEM_ERROR");
        break;
    case KEYSTORE_PROTOCOL_ERROR:
        sprintf(buffer, "KEYSTORE_PROTOCOL_ERROR");
        break;

    case KEYSTORE_PERMISSION_DENIED:
        sprintf(buffer, "KEYSTORE_PERMISSION_DENIED");
        break;

    case KEYSTORE_KEY_NOT_FOUND:
        sprintf(buffer, "KEYSTORE_KEY_NOT_FOUND");
        break;

    case KEYSTORE_VALUE_CORRUPTED:
        sprintf(buffer, "KEYSTORE_VALUE_CORRUPTED");
        break;

    case KEYSTORE_UNDEFINED_ACTION:
        sprintf(buffer, "KEYSTORE_UNDEFINED_ACTION");
        break;
    case KEYSTORE_WRONG_PASSWORD_0:
        sprintf(buffer, "KEYSTORE_WRONG_PASSWORD_0");
        break;
    case KEYSTORE_WRONG_PASSWORD_1:
        sprintf(buffer, "KEYSTORE_WRONG_PASSWORD_1");
        break;

    case KEYSTORE_WRONG_PASSWORD_2:
        sprintf(buffer, "KEYSTORE_WRONG_PASSWORD_2");
        break;
    case KEYSTORE_WRONG_PASSWORD_3:
        sprintf(buffer, "KEYSTORE_WRONG_PASSWORD_3");
        break;
    case KEYSTORE_SIGNATURE_INVALID:
        sprintf(buffer, "KEYSTORE_SIGNATURE_INVALID");
        break;
    case KEYSTORE_OP_AUTH_NEEDED:
        sprintf(buffer, "KEYSTORE_OP_AUTH_NEEDED");
        break;
    case KEYSTORE_KEY_ALREADY_EXISTS:
        sprintf(buffer, "KEYSTORE_KEY_ALREADY_EXISTS");
        break;
    case KEYSTORE_KEY_PERMANENTLY_INVALIDATED:
        sprintf(buffer, "KEYSTORE_KEY_PERMANENTLY_INVALIDATED");
        break;
    }
    ALOGI("Return=%s(%d)", buffer, code);
    return code;
}

extern "C" int main(int argc, char* argv[])
{
    CommandLine::Init(argc, argv);
    CommandLine* command_line = CommandLine::ForCurrentProcess();
    CommandLine::StringVector args = command_line->GetArgs();

    android::ProcessState::self()->startThreadPool();

    if (args.empty()) {
        printUsageAndExit();
        return -1;
    }

    if (args[0] == "keyStoreExist") {
        std::string name = command_line->GetSwitchValueASCII("name");
        return logReturnCode(keyStoreExist(name.c_str(), name.length()));
    } else if (args[0] == "keyStoreInsert") {
        std::string name = command_line->GetSwitchValueASCII("name");
        std::string data = command_line->GetSwitchValueASCII("data");
        return logReturnCode(keyStoreInsert(name.c_str(), name.length(),
            (const uint8_t*)data.c_str(), data.length()));
    } else if (args[0] == "keyStoreGet") {
        std::string name = command_line->GetSwitchValueASCII("name");
        FAR uint8_t* item = 0;
        size_t len = 0;
        int ret = keyStoreGet(name.c_str(), name.length(), &item, &len);
        if (len > 0) {
            ALOGI("keyStoreGet key=%s,data=%s,length=%zu", name.c_str(), std::string((char*)item, len).c_str(), len);
            free(item);
        } else {
            ALOGE("keyStoreGet %s failed=%d", name.c_str(), ret);
        }
        return logReturnCode(ret);
    } else if (args[0] == "keyStoreDel") {
        std::string name = command_line->GetSwitchValueASCII("name");
        return logReturnCode(keyStoreDel(name.c_str(), name.length()));
    } else if (args[0] == "keyStoreReset") {
        return logReturnCode(keyStoreReset());
    } else {
        ALOGE("unknown command:%s", args[0].c_str());
        printUsageAndExit();
        return -1;
    }
    return 0;
}