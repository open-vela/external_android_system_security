#
# Copyright (C) 2023 Xiaomi Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.
#
if(CONFIG_ANDROID_KEYSTORE)

  # keystore global include path
  set_property(
    TARGET nuttx
    APPEND
    PROPERTY NUTTX_CXX_INCLUDE_DIRECTORIES
             ${CMAKE_CURRENT_LIST_DIR}/keystore/binder)

  set_property(
    TARGET nuttx
    APPEND
    PROPERTY NUTTX_INCLUDE_DIRECTORIES
             ${CMAKE_CURRENT_LIST_DIR}/keystore/include)

  nuttx_add_library(android_keystore STATIC)

  # sources

  set(SRCS
      keystore/auth_token_table.cpp
      keystore/blob.cpp
      keystore/client/client.cpp
      keystore/confirmation_manager.cpp
      keystore/grant_store.cpp
      keystore/key_store_service.cpp
      keystore/keyblob_utils.cpp
      keystore/keymaster_enforcement.cpp
      keystore/keymaster_worker.cpp
      keystore/keystore_utils.cpp
      keystore/keystore_aidl_hidl_marshalling_utils.cpp
      keystore/KeystoreResponse.cpp
      keystore/key_creation_log_handler.cpp
      keystore/key_operation_log_handler.cpp
      keystore/key_attestation_log_handler.cpp
      keystore/keystore_attestation_id.cpp
      keystore/KeymasterArguments.cpp
      keystore/KeyStore.cpp
      keystore/keystore_client_impl.cpp
      keystore/keystore_client.pb.cpp
      keystore/legacy_keymaster_device_wrapper.cpp
      keystore/operation.cpp
      keystore/OperationResult.cpp
      keystore/permissions.cpp
      keystore/user_state.cpp)

  # aidl sources
  file(GLOB_RECURSE AIDLS keystore/*.aidl)
  nuttx_add_aidl(
    TARGET
    android_keystore
    AIDL_BASE_DIR
    ${CMAKE_CURRENT_LIST_DIR}/keystore/binder
    AIDL_INCLUDE_DIR
    ${CMAKE_CURRENT_LIST_DIR}/keystore/binder
    AIDL_HEADER_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/keystore/binder
    AIDL_OUT_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/keystore/binder
    AIDL_FLAGS
    --lang=cpp
    AIDLS
    ${AIDLS})

  # flags
  set(FLAGS -DGRANT_ROOT_ALL_PERMISSIONS -Wno-shadow -Wno-unused-result
            -fpermissive)

  nuttx_add_application(
    MODULE
    ${CONFIG_ANDROID_KEYSTORE}
    NAME
    keystore
    STACKSIZE
    ${CONFIG_ANDROID_KEYSTORE_STACKSIZE}
    SRCS
    keystore/keystore_main.cpp
    COMPILE_FLAGS
    ${FLAGS}
    INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_BINARY_DIR}/keystore/binder
    DEPENDS
    android_keystore)

  if(CONFIG_ANDROID_KEYSTORE_TOOL)
    nuttx_add_application(
      MODULE
      ${CONFIG_ANDROID_KEYSTORE}
      NAME
      keystore_tool
      STACKSIZE
      ${CONFIG_ANDROID_KEYSTORE_STACKSIZE}
      SRCS
      keystore/client/keystore_tool.cpp
      COMPILE_FLAGS
      ${FLAGS}
      INCLUDE_DIRECTORIES
      ${NUTTX_APPS_DIR}/external/libchrome/libchrome
      ${CMAKE_CURRENT_BINARY_DIR}/keystore/binder
      DEPENDS
      android_keystore)

    nuttx_add_application(
      MODULE
      ${CONFIG_ANDROID_KEYSTORE}
      NAME
      keystore_client_v2
      STACKSIZE
      ${CONFIG_ANDROID_KEYSTORE_STACKSIZE}
      SRCS
      keystore/keystore_cli_v2.cpp
      COMPILE_FLAGS
      ${FLAGS}
      INCLUDE_DIRECTORIES
      ${NUTTX_APPS_DIR}/external/libchrome/libchrome
      DEPENDS
      android_keystore)
  endif()

  if(CONFIG_ANDROID_KEYSTORE_TEST)
    nuttx_add_application(
      MODULE
      ${CONFIG_ANDROID_KEYSTORE}
      NAME
      keystoreUnitTest
      STACKSIZE
      ${CONFIG_ANDROID_KEYSTORE_STACKSIZE}
      SRCS
      keystore/tests/gtest_main_entry.cpp
      keystore/KeyAttestationApplicationId.cpp
      keystore/KeyAttestationPackageInfo.cpp
      keystore/Signature.cpp
      COMPILE_FLAGS
      ${FLAGS}
      DEPENDS
      android_keystore)
  endif()

  add_dependencies(android_keystore android)
  target_sources(android_keystore PRIVATE ${SRCS})
  target_compile_options(android_keystore PRIVATE ${FLAGS})

endif()
