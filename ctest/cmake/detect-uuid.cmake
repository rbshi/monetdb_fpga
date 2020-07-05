#[[
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0.  If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Copyright 1997 - July 2008 CWI, August 2008 - 2020 MonetDB B.V.
#]]

if (${LINUX_DISTRO} STREQUAL "debian")
  if(${LINUX_DISTRO_VERSION} STREQUAL "9")
    assert_package_detected(
      detect FALSE
      legacyvariable HAVE_UUID
      variablename HAVE_UUID_GENERATE)
  endif()
  if(${LINUX_DISTRO_VERSION} STREQUAL "10")
    assert_package_detected(
      detect FALSE
      legacyvariable HAVE_UUID
      variablename HAVE_UUID_GENERATE)
  endif()
elseif (${LINUX_DISTRO} STREQUAL "ubuntu")
  if(${LINUX_DISTRO_VERSION} STREQUAL "18")
    assert_package_detected(
      detect FALSE
      legacyvariable HAVE_UUID
      variablename HAVE_UUID_GENERATE)
  endif()
  if(${LINUX_DISTRO_VERSION} STREQUAL "19")
    assert_package_detected(
      detect FALSE
      legacyvariable HAVE_UUID
      variablename HAVE_UUID_GENERATE)
  endif()
  if(${LINUX_DISTRO_VERSION} STREQUAL "20")
    assert_package_detected(
      detect FALSE
      legacyvariable HAVE_UUID
      variablename HAVE_UUID_GENERATE)
  endif()
elseif(${LINUX_DISTRO} STREQUAL "fedora")
  if(${LINUX_DISTRO_VERSION} STREQUAL "30")
    assert_package_detected(
      detect TRUE
      legacyvariable HAVE_UUID
      variablename HAVE_UUID_GENERATE)
  endif()
  if(${LINUX_DISTRO_VERSION} STREQUAL "31")
    assert_package_detected(
      detect TRUE
      legacyvariable HAVE_UUID
      variablename HAVE_UUID_GENERATE)
  endif()
  if(${LINUX_DISTRO_VERSION} STREQUAL "32")
    assert_package_detected(
      detect TRUE
      legacyvariable HAVE_UUID
      variablename HAVE_UUID_GENERATE)
  endif()
else()
  message(ERROR "Linux distro: ${LINUX_DISTRO} not known")
  message(ERROR "Linux distro version: ${LINUX_DISTRO_VERSION} not known")
endif()
