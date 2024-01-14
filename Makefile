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
DEPPATH += --dep-path keystore
VPATH   += :keystore/client
DEPPATH += --dep-path keystore/client
CXXSRCS += auth_token_table.cpp
CXXSRCS += blob.cpp
CXXSRCS += client.cpp
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
CXXSRCS += keystore_client_impl.cpp
CXXSRCS += keystore_client.pb.cpp
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
CXXFLAGS += -DGRANT_ROOT_ALL_PERMISSIONS
CXXFLAGS += -Wno-shadow -Wno-unused-result -fpermissive

MAINSRC = keystore_main.cpp
PROGNAME += keystore

ifneq ($(CONFIG_ANDROID_KEYSTORE_TOOL),)
CXXFLAGS += ${INCDIR_PREFIX}$(APPDIR)/external/libchrome/libchrome
# KeyStore Client tool
MAINSRC += keystore_tool.cpp
PROGNAME += keystore_tool

# Android KeyStore Client tool
MAINSRC  += keystore_cli_v2.cpp
PROGNAME += keystore_client_v2
endif

ifneq ($(CONFIG_ANDROID_KEYSTORE_TEST),)
VPATH    += :keystore/tests
DEPPATH += --dep-path keystore/tests
PROGNAME += keystoreUnitTest
MAINSRC  += gtest_main_entry.cpp
CXXSRCS  += KeyAttestationApplicationId.cpp
CXXSRCS  += KeyAttestationPackageInfo.cpp
CXXSRCS  += Signature.cpp
endif

include $(APPDIR)/Application.mk