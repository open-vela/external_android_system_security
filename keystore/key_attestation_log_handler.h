/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef _KEY_ATTESTATION_LOG_HANDLER_H_
#define _KEY_ATTESTATION_LOG_HANDLER_H_

#include <stdint.h>

namespace keystore {

void logKeystoreKeyAttestationEvent(bool wasSuccessful, int32_t errorCode);

}

#endif  //_KEY_ATTESTATION_LOG_HANDLER_H_
