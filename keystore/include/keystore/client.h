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

/****************************************************************************
 * Name: keyStoreInsert
 *
 * Description:
 *   Add data to keystore
 *
 * Input Parameters:
 *   name:Alias of the data, it is unique for per application
 *   nameLength:Length of name
 *   item:Data to added to keystore
 *   itemLength:The length of item
 *
 * Returned Value:
 *   Success if KEYSTORE_NO_ERROR,error if other values
 *
 * Assumptions/Limitations:
 *   Name is unique for per application, it is used for keyStoreInsert,keyStoreGet,keyStoreDel,keyStoreExist
 *
 ****************************************************************************/
extern int keyStoreInsert(FAR const char* name, size_t nameLength,
    FAR const uint8_t* item, size_t itemLength);

/****************************************************************************
 * Name: keyStoreInsert
 *
 * Description:
 *   Get data from keystore
 *
 * Input Parameters:
 *   name:Alias of the data, it is unique for per application
 *   nameLength:Length of name
 *   item:Data from keystore, memory is alloc in keystore, but caller need call function free() to release memory
 *   itemLength:The length of item getted from keystore
 *
 * Returned Value:
 *   Success if KEYSTORE_NO_ERROR,error if other values
 *
 * Assumptions/Limitations:
 *   Item pointer should release by caller
 *   Name is unique for per application, it is used for keyStoreInsert,keyStoreGet,keyStoreDel,keyStoreExist
 *
 ****************************************************************************/
extern int keyStoreGet(FAR const char* name, size_t nameLength,
    FAR uint8_t** item, FAR size_t* itemLength);

/****************************************************************************
 * Name: keyStoreDel
 *
 * Description:
 *   Delete data from keystore
 *
 * Input Parameters:
 *   name:Alias of the data, it is unique for per application
 *   nameLength:Length of name
 *
 * Returned Value:
 *   Success if KEYSTORE_NO_ERROR,error if other values
 *
 * Assumptions/Limitations:
 *   Name is unique for per application, it is used for keyStoreInsert,keyStoreGet,keyStoreDel,keyStoreExist
 *
 ****************************************************************************/
extern int keyStoreDel(FAR const char* name, size_t nameLength);

/****************************************************************************
 * Name: keyStoreExist
 *
 * Description:
 *   If data exist in keystore
 *
 * Input Parameters:
 *   name:Alias of the data, it is unique for per application
 *   nameLength:Length of name
 *
 * Returned Value:
 *   Exist if KEYSTORE_NO_ERROR,error or not exist if other values
 *
 * Assumptions/Limitations:
 *   Name is unique for per application, it is used for keyStoreInsert,keyStoreGet,keyStoreDel,keyStoreExist
 *
 ****************************************************************************/
extern int keyStoreExist(FAR const char* name, size_t nameLength);

/****************************************************************************
 * Name: keyStoreReset
 *
 * Description:
 *   Detele all keystore datas of current Application
 *
 * Input Parameters:
 *
 * Returned Value:
 *   Success if KEYSTORE_NO_ERROR,error if other values
 *
 * Assumptions/Limitations:
 *   Name is unique for per application, it is used for keyStoreInsert,keyStoreGet,keyStoreDel,keyStoreExist
 *
 ****************************************************************************/
extern int keyStoreReset(void);

#if defined(__cplusplus)
}
#endif

#endif  // KEYSTORE_CLIENT_H
