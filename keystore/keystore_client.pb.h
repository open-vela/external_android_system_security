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

#ifndef KEYSTORE_KEYSTORE_CLIENT_PB_H_
#define KEYSTORE_KEYSTORE_CLIENT_PB_H_

#include <string>

class EncryptedData {
public:
    EncryptedData();
    void set_init_vector(const std::string& vt);
    const std::string &init_vector() const
    {
        return init_vector_;
    }

    void set_authentication_data(const std::string& au);
    const std::string &authentication_data() const
    {
        return authentication_data_;
    }

    void set_encrypted_data(const std::string& en);
    const std::string &encrypted_data()
    {
        return encrypted_data_;
    }

    bool SerializeToString(std::string* out);
    bool ParseFromString(const std::string& str);

private:
    std::string init_vector_;
    std::string authentication_data_;
    std::string encrypted_data_;
};
#endif // KEYSTORE_KEYSTORE_CLIENT_PB_H_