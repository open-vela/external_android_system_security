/*
 * Copyright (C) 2022 Xiaomi Corporation
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

#include "keystore_client.pb.h"
#include <stdint.h>
#include <log/log.h>

EncryptedData::EncryptedData()
    : init_vector_()
    , authentication_data_()
    , encrypted_data_()
{
}

void EncryptedData::set_init_vector(const std::string& vt)
{
    init_vector_ = vt;
}

void EncryptedData::set_authentication_data(const std::string& au)
{
    authentication_data_ = au;
}

void EncryptedData::set_encrypted_data(const std::string& en)
{
    encrypted_data_ = en;
}

bool EncryptedData::SerializeToString(std::string* out)
{
    if (!out) {
        return false;
    }

    out->clear();
    if (init_vector_.length() == 0 || encrypted_data_.length() == 0) {
        return false;
    }

    size_t init_vector_length = init_vector_.length();
    out->append((char*)&init_vector_length, sizeof(size_t));
    out->append(init_vector_.data(), init_vector_length);

    size_t authentication_data_legnth = authentication_data_.length();
    out->append((char*)&authentication_data_legnth, sizeof(size_t));
    out->append(authentication_data_.data(), authentication_data_legnth);

    size_t encrypted_data_length = encrypted_data_.length();
    out->append((char*)&encrypted_data_length, sizeof(size_t));
    out->append(encrypted_data_.data(), encrypted_data_length);
    return true;
}

bool EncryptedData::ParseFromString(const std::string& str)
{
    if (str.size() == 0) {
        ALOGE("ParseFromString failed: str size is 0");
        return false;
    }

    size_t startPos = 0;
    size_t init_vector_length;
    if (sizeof(size_t) >= str.size()) {
        ALOGE("ParseFromString failed:get init_vector_length failed");
        return false;
    }
    memcpy(&init_vector_length, str.data(), sizeof(size_t));
    startPos += sizeof(size_t);
    if (startPos + init_vector_length >= str.size()) {
        ALOGE("ParseFromString failed:get init_vector_ failed");
        return false;
    }
    init_vector_ = std::string(str.data() + startPos, init_vector_length);
    startPos += init_vector_length;

    if (startPos + sizeof(size_t) >= str.size()) {
        ALOGE("ParseFromString failed:get authentication_data_length failed");
        return false;
    }
    size_t authentication_data_length;
    memcpy(&authentication_data_length, str.data() + startPos, sizeof(size_t));
    startPos += sizeof(size_t);
    if (startPos + authentication_data_length >= str.size()) {
        ALOGE("ParseFromString failed:get authentication_data_ failed");
        return false;
    }
    authentication_data_ = std::string(str.data() + startPos, authentication_data_length);
    startPos += authentication_data_length;
    if (startPos + sizeof(size_t) >= str.size()) {
        ALOGE("ParseFromString failed:get encrypted_data_length failed");
        return false;
    }
    size_t encrypted_data_length;
    memcpy(&encrypted_data_length, str.data() + startPos, sizeof(size_t));
    startPos += sizeof(size_t);
    if (startPos + encrypted_data_length != str.size()) {
        ALOGE("ParseFromString failed:get encrypted_data_ failed");
        return false;
    }
    encrypted_data_ = std::string(str.data() + startPos, encrypted_data_length);
    return true;
}
