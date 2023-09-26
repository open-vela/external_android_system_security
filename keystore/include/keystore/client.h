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

#ifndef KEYSTORE_CLIENT_H
#define KEYSTORE_CLIENT_H

#include <stddef.h>
#include <stdint.h>

//error code same as external/android/security/keystore/keystore/include/keystore/keystore.h
#define KEYSTORE_NO_ERROR                     1
#define KEYSTORE_LOCKED                       2
#define KEYSTORE_UNINITIALIZED                3
#define KEYSTORE_SYSTEM_ERROR                 4
#define KEYSTORE_PROTOCOL_ERROR               5
#define KEYSTORE_PERMISSION_DENIED            6
#define KEYSTORE_KEY_NOT_FOUND                7
#define KEYSTORE_VALUE_CORRUPTED              8
#define KEYSTORE_UNDEFINED_ACTION             9
#define KEYSTORE_WRONG_PASSWORD_0             10
#define KEYSTORE_WRONG_PASSWORD_1             11
#define KEYSTORE_WRONG_PASSWORD_2             12
#define KEYSTORE_WRONG_PASSWORD_3             13 // MAX_RETRY = 4
#define KEYSTORE_SIGNATURE_INVALID            14
#define KEYSTORE_OP_AUTH_NEEDED               15 // Auth is needed for this operation before it can be used.
#define KEYSTORE_KEY_ALREADY_EXISTS           16
#define KEYSTORE_KEY_PERMANENTLY_INVALIDATED  17
#define KEYSTORE_ABORT_CALLED                 18
#define KEYSTORE_PRUNED                       19
#define KEYSTORE_BINDER_DIED                  20

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Put an item to keystore
 *
 * the keyStoreInsert using to store an item with a unique name into the
 * keystore, and keystore internal will encrypt and manage this item
 * @param[in] name       the name of the item to insert, this name should
 *                       be unique, and the max length of the name are:
 *                       "CONFIG_NAME_MAX - 12". If the item name contains
 *                       special character(for example, that ascii code that
 *                       between '0' to '~', then the length of the character
 *                       are calculated as 2)
 * @param[in] nameLength the length of the name.
 * @param[in] item       the item data that need to be saved to keystore.
 * @param[in] itemLength the length of the item data.
 * @return KEYSTORE_NO_ERROR on success; a value larger than 1 on failure
 */
extern int keyStoreInsert(FAR const char* name, size_t nameLength,
    FAR const uint8_t* item, size_t itemLength);

/**
 * @brief Get data from keystore
 *
 * @param[in] name       the name of the item to fetch from keystore.
 * @param[in] nameLength the length of the item name.
 * @param[in] item       data from keystore, memory is alloc in keystore, but
 *                       caller need call function free() to release memory.
 * @param[in] itemLength the length of item fetched from keystore.
 * @return KEYSTORE_NO_ERROR on success; a value larger than 1 on failure
 */
extern int keyStoreGet(FAR const char* name, size_t nameLength,
    FAR uint8_t** item, FAR size_t* itemLength);

/**
 * @brief Delete data from keystore
 *
 * @param[in] name       the name of the item to delete from keystore.
 * @param[in] nameLength the length of the item name.
 * @return KEYSTORE_NO_ERROR on success; a value larger than 1 on failure
 */
extern int keyStoreDel(FAR const char* name, size_t nameLength);

/**
 * @brief To detect if an item is exist in keystore
 *
 * @param[in] name       the name of the item to detect if exist in keystore.
 * @param[in] nameLength the length of the item name.
 * @return KEYSTORE_NO_ERROR on success; a value larger than 1 on failure
 */
extern int keyStoreExist(FAR const char* name, size_t nameLength);

/**
 * @brief Detele all keystore datas of current Application
 *
 * @return KEYSTORE_NO_ERROR on success; a value larger than 1 on failure
 */
extern int keyStoreReset(void);

#if defined(__cplusplus)
}
#endif

#endif  // KEYSTORE_CLIENT_H
