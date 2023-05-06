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
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <binder/IPCThreadState.h>
#include <keystore/keymaster_types.h>
#include <keystore/keystore_client_impl.h>

#define KEYSTORE_SAVE_PATH "/data/keystore/"

using namespace keystore;
namespace {
const char kEncryptSuffix[] = "_ENC";
const char kAuthenticateSuffix[] = "_AUTH";

int createDir(FAR const char* path)
{
    int ret = 0;
    if ((ret = access(path, F_OK)) != 0) {
        ALOGV("to mkdir %s", path);
        if ((ret = mkdir(path, 0755)) == -1) {
            ALOGE("mkdir error:%s\n", path);
            return -1;
        }
    }
    return 0;
}

int removeDir(FAR const char* path)
{
    ALOGV("to remove dir %s", path);
    FAR DIR* pDir = opendir(path);
    size_t path_len = strlen(path);
    int result = -1;

    if (pDir) {
        FAR struct dirent* pdirent;
        result = 0;
        while (!result && (pdirent = readdir(pDir))) {
            int resul2 = -1;
            FAR char* buf;
            size_t len;

            /* Skip the names "." and ".." as we don't want to recurse on them. */
            if (!strcmp(pdirent->d_name, ".") || !strcmp(pdirent->d_name, ".."))
                continue;

            len = path_len + strlen(pdirent->d_name) + 2;
            buf = (char*)malloc(len);

            if (buf) {
                struct stat statbuf;

                snprintf(buf, len, "%s/%s", path, pdirent->d_name);
                if (!stat(buf, &statbuf)) {
                    if (S_ISDIR(statbuf.st_mode))
                        resul2 = removeDir(buf);
                    else
                        resul2 = unlink(buf);
                }
                free(buf);
            }
            result = resul2;
        }
        closedir(pDir);
    } else {
        ALOGW("dir %s not exist", path);
        return 0;
    }

    if (!result)
        result = rmdir(path);
    return result;
}

std::string getUidDir(int32_t uid)
{
    return std::string(KEYSTORE_SAVE_PATH) + std::to_string(uid);
}

std::string getSaveFilePath(const std::string& name)
{
    std::string dir = getUidDir(geteuid()) + "/" + name;
    ALOGV("getSaveFilePath %s", dir.c_str());
    return dir;
}

bool createAppSaveDirs()
{
    return createDir(KEYSTORE_SAVE_PATH) == 0
        && createDir(getUidDir(geteuid()).c_str()) == 0;
}

std::unique_ptr<KeystoreClient> CreateKeystoreInstance()
{
    return std::unique_ptr<KeystoreClient>(
        static_cast<KeystoreClient*>(new keystore::KeystoreClientImpl));
}

}; // namespace

int keyStoreInsert(FAR const char* name, size_t nameLength,
    FAR const uint8_t* item, size_t itemLength)
{
    if (!name || nameLength == 0 || !item || itemLength == 0) {
        ALOGE("keyStoreInsert error for key=%p, nameLength=%zu, item=%p, itemLength=%zu",
            name, nameLength, item, itemLength);
        return KEYSTORE_SYSTEM_ERROR;
    }

    std::string keyName(name, nameLength);

    ALOGV("keyStoreInsert key=%s, item length=%zu start", keyName.c_str(), itemLength);

    if (!createAppSaveDirs()) {
        ALOGE("keyStoreInsert createAppSaveDirs failed");
        return KEYSTORE_SYSTEM_ERROR;
    }

    std::unique_ptr<KeystoreClient> keystore = CreateKeystoreInstance();
    std::string input((const char*)item, itemLength);
    std::string output;

    android::ProcessState::self()->startThreadPool();
    if (!keystore->encryptWithAuthentication(keyName, input, KEYSTORE_FLAG_NONE, &output)) {
        ALOGE("EncryptWithAuthentication failed.\n");
        return KEYSTORE_SYSTEM_ERROR;
    }

    std::string path = getSaveFilePath(keyName);
    std::ofstream file(path.c_str(), std::ios::binary | std::ios::out); //add ios::noreplace if not override file
    if (!file.is_open()) {
        ALOGE("EncryptWithAuthentication open file %s failed.\n", path.c_str());
        return KEYSTORE_SYSTEM_ERROR;
    }

    file << output;
    file.close();
    return KEYSTORE_NO_ERROR;
}

int keyStoreGet(FAR const char* name, size_t nameLength,
    FAR uint8_t** item, FAR size_t* itemLength)
{
    if (!name || nameLength == 0) {
        ALOGE("keyStoreGet key or nameLength is wrong(key=%p, nameLength=%zu)", name, nameLength);
        return KEYSTORE_SYSTEM_ERROR;
    }

    *itemLength = 0;
    std::string keyName(name, nameLength);
    std::unique_ptr<KeystoreClient> keystore = CreateKeystoreInstance();
    std::string path(getSaveFilePath(keyName));
    std::string input;
    std::ifstream file(path.c_str(), std::ios::binary | std::ios::in);
    if (!file.is_open()) {
        ALOGE("DecryptWithAuthentication open file %s failed.", path.c_str());
        return KEYSTORE_KEY_NOT_FOUND;
    }
    std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), std::back_inserter(input));
    file.close();

    std::string output;
    android::ProcessState::self()->startThreadPool();
    if (!keystore->decryptWithAuthentication(keyName, input, &output)) {
        ALOGE("DecryptWithAuthentication failed.\n");
        return KEYSTORE_SYSTEM_ERROR;
    }

    uint8_t* pData = (uint8_t*)malloc(output.size());
    memcpy(pData, output.data(), output.size());
    *item = pData;
    *itemLength = output.size();
    return KEYSTORE_NO_ERROR;
}

int keyStoreDel(FAR const char* name, size_t nameLength)
{
    if (!name || nameLength == 0) {
        ALOGE("keyStoreDel key or nameLength is wrong(key=%p,nameLength=%zu)", name, nameLength);
        return KEYSTORE_SYSTEM_ERROR;
    }

    std::string keyName(name, nameLength);
    std::string keyNameEncrypt = keyName + kEncryptSuffix;
    std::string keyNameAuthenticate = keyName + kAuthenticateSuffix;
    std::unique_ptr<KeystoreClient> keystore = CreateKeystoreInstance();
    KeyStoreNativeReturnCode encryptReturnCode = keystore->deleteKey(keyNameEncrypt);
    KeyStoreNativeReturnCode authenticateReturnCode = keystore->deleteKey(keyNameAuthenticate);
    ALOGV("keyStoreDel keyNameEncrypt=%s, encryptReturnCode=%d, keyNameAuthenticate=%s, authenticateReturnCode=%d",
        keyNameEncrypt.c_str(), encryptReturnCode.getErrorCode(),
        keyNameAuthenticate.c_str(), authenticateReturnCode.getErrorCode());
    int ret = remove(getSaveFilePath(keyName).c_str());
    if (encryptReturnCode.isOk() && authenticateReturnCode.isOk() && ret == 0) {
        return KEYSTORE_NO_ERROR;
    } else if (!encryptReturnCode.isOk()) {
        return encryptReturnCode.getErrorCode();
    } else if (!authenticateReturnCode.isOk()) {
        return authenticateReturnCode.getErrorCode();
    }
    return KEYSTORE_SYSTEM_ERROR;
}

int keyStoreExist(FAR const char* name, size_t nameLength)
{
    if (!name || nameLength == 0) {
        ALOGE("keyStoreExist key or nameLength is wrong(key=%p,nameLength=%zu)", name, nameLength);
        return KEYSTORE_SYSTEM_ERROR;
    }

    std::string keyName(name, nameLength);
    std::string path(getSaveFilePath(keyName));
    if (access(path.c_str(), F_OK) != 0) {
        ALOGE("keyStoreExist can not access file %s", path.c_str());
        return KEYSTORE_KEY_NOT_FOUND;
    }

    std::string keyNameEncrypt = std::string(name, nameLength) + kEncryptSuffix;
    std::string keyNameAuthenticate = std::string(name, nameLength) + kAuthenticateSuffix;
    std::unique_ptr<KeystoreClient> keystore = CreateKeystoreInstance();
    bool encryptExist = keystore->doesKeyExist(keyNameEncrypt);
    bool authenticateExist = keystore->doesKeyExist(keyNameAuthenticate);
    ALOGV("keyStoreExist keyNameEncrypt=%s, encryptExist=%d, keyNameAuthenticate=%s, authenticateExist=%d",
        keyNameEncrypt.c_str(), encryptExist,
        keyNameAuthenticate.c_str(), authenticateExist);
    return encryptExist && authenticateExist ? KEYSTORE_NO_ERROR : KEYSTORE_KEY_NOT_FOUND;
}

int keyStoreReset(void)
{
    std::unique_ptr<KeystoreClient> keystore = CreateKeystoreInstance();
    KeyStoreNativeReturnCode returnCode = keystore->deleteAllKeys();
    ALOGV("keyStoreReset returnCode=%d end", returnCode.getErrorCode());
    int ret = removeDir(getUidDir(geteuid()).c_str());
    if (returnCode.isOk() && ret == 0) {
        return KEYSTORE_NO_ERROR;
    } else if (!returnCode.isOk()) {
        return returnCode.getErrorCode();
    }
    return KEYSTORE_SYSTEM_ERROR;
}