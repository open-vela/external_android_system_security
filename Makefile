#
# Copyright (C) 2023 Xiaomi Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

include $(APPDIR)/Make.defs

CXXEXT = .cpp

VPATH   += :keystore
CXXSRCS += auth_token_table.cpp
CXXSRCS += blob.cpp
CXXSRCS += confirmation_manager.cpp
CXXSRCS += grant_store.cpp
CXXSRCS += key_store_service.cpp
CXXSRCS += keyblob_utils.cpp
CXXSRCS += keymaster_enforcement.cpp
CXXSRCS += keymaster_worker.cpp
CXXSRCS += keystore_utils.cpp
CXXSRCS += keystore_aidl_hidl_marshalling_utils.cpp
CXXSRCS += KeystoreResponse.cpp
CXXSRCS += key_creation_log_handler.cpp
CXXSRCS += key_operation_log_handler.cpp
CXXSRCS += key_attestation_log_handler.cpp
CXXSRCS += keystore_attestation_id.cpp
CXXSRCS += KeymasterArguments.cpp
CXXSRCS += KeyStore.cpp
CXXSRCS += legacy_keymaster_device_wrapper.cpp
CXXSRCS += operation.cpp
CXXSRCS += OperationResult.cpp
CXXSRCS += permissions.cpp
CXXSRCS += user_state.cpp

AIDLSRCS += $(shell find keystore -name *.aidl)
AIDLFLAGS = --lang=cpp -Ikeystore/binder -hkeystore/binder -okeystore/binder
CXXSRCS  += $(patsubst %$(AIDLEXT),%$(CXXEXT),$(AIDLSRCS))

MODULE    = $(CONFIG_ANDROID_KEYSTORE)
PRIORITY  = SCHED_PRIORITY_DEFAULT
STACKSIZE = $(CONFIG_ANDROID_KEYSTORE_STACKSIZE)

MAINSRC = keystore_main.cpp
PROGNAME += keystore

include $(APPDIR)/Application.mk