#!/usr/bin/env bash

set -euo pipefail

APP_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
WORKSPACE_DIR="$(cd "${APP_DIR}/.." && pwd)"
VENV_DIR="${WORKSPACE_DIR}/.venv"
ZEPHYR_BASE="${WORKSPACE_DIR}/deps/zephyr"
SDK_VERSION="0.16.8"
SDK_DIR="${WORKSPACE_DIR}/deps/zephyr-sdk-${SDK_VERSION}"
SDK_RELEASE="https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v${SDK_VERSION}"
TOOLCHAIN="xtensa-espressif_esp32_zephyr-elf"

case "$(uname -s)-$(uname -m)" in
  Darwin-arm64) SDK_HOST="macos-aarch64" ;;
  Darwin-x86_64) SDK_HOST="macos-x86_64" ;;
  Linux-aarch64) SDK_HOST="linux-aarch64" ;;
  Linux-x86_64) SDK_HOST="linux-x86_64" ;;
  *)
    echo "Unsupported host: $(uname -s) $(uname -m)" >&2
    exit 1
    ;;
esac

download_and_verify() {
  local filename="$1"
  local destination="$2"
  local temporary="${destination}.download"
  local checksums
  local expected
  local actual

  checksums="$(curl -fsSL "${SDK_RELEASE}/sha256.sum")"
  expected="$(printf '%s\n' "${checksums}" | awk -v name="${filename}" '$2 == name { print $1 }')"
  if [[ -z "${expected}" ]]; then
    echo "No published checksum found for ${filename}" >&2
    exit 1
  fi

  curl -fL --retry 3 -o "${temporary}" "${SDK_RELEASE}/${filename}"
  if command -v shasum >/dev/null 2>&1; then
    actual="$(shasum -a 256 "${temporary}" | awk '{ print $1 }')"
  else
    actual="$(sha256sum "${temporary}" | awk '{ print $1 }')"
  fi

  if [[ "${actual}" != "${expected}" ]]; then
    echo "Checksum mismatch for ${filename}" >&2
    rm -f "${temporary}"
    exit 1
  fi
  mv "${temporary}" "${destination}"
}

if [[ ! -x "${VENV_DIR}/bin/python" ]]; then
  python3 -m venv "${VENV_DIR}"
fi

"${VENV_DIR}/bin/python" -m pip install --upgrade pip
"${VENV_DIR}/bin/python" -m pip install 'cmake<4' west

if [[ ! -f "${WORKSPACE_DIR}/.west/config" ]]; then
  "${VENV_DIR}/bin/west" init -l "${APP_DIR}" "${WORKSPACE_DIR}"
fi
"${VENV_DIR}/bin/west" update
"${VENV_DIR}/bin/west" blobs fetch hal_espressif
"${VENV_DIR}/bin/python" -m pip install -r "${ZEPHYR_BASE}/scripts/requirements.txt"

mkdir -p "${WORKSPACE_DIR}/deps"
mkdir -p "${WORKSPACE_DIR}/.cache/ccache"
if [[ ! -f "${SDK_DIR}/sdk_version" ]]; then
  sdk_archive="zephyr-sdk-${SDK_VERSION}_${SDK_HOST}_minimal.tar.xz"
  sdk_download="${WORKSPACE_DIR}/deps/${sdk_archive}"
  download_and_verify "${sdk_archive}" "${sdk_download}"
  tar -xf "${sdk_download}" -C "${WORKSPACE_DIR}/deps"
  rm -f "${sdk_download}"
fi

if [[ ! -d "${SDK_DIR}/${TOOLCHAIN}" ]]; then
  toolchain_archive="toolchain_${SDK_HOST}_${TOOLCHAIN}.tar.xz"
  toolchain_download="${WORKSPACE_DIR}/deps/${toolchain_archive}"
  download_and_verify "${toolchain_archive}" "${toolchain_download}"
  tar -xf "${toolchain_download}" -C "${SDK_DIR}"
  rm -f "${toolchain_download}"
fi

if ! command -v task >/dev/null 2>&1; then
  echo "Task is required to start the local MQTT broker." >&2
  exit 1
fi

(cd "${APP_DIR}" && task broker-start)

echo
echo "Zephyr is ready and the MQTT broker is running. Start a shell with:"
echo "  source \"${APP_DIR}/zephyr-env.sh\""
echo
echo "Then build with:"
echo "  west build -d \"${APP_DIR}/build\" -p auto \"${APP_DIR}\""
