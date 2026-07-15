#!/usr/bin/env bash

_zephyr_app_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export ZEPHYR_WORKSPACE="$(cd "${_zephyr_app_dir}/.." && pwd)"
export ZEPHYR_BASE="${ZEPHYR_WORKSPACE}/deps/zephyr"
export ZEPHYR_SDK_INSTALL_DIR="${ZEPHYR_WORKSPACE}/deps/zephyr-sdk-0.16.8"
export CCACHE_DIR="${ZEPHYR_WORKSPACE}/.cache/ccache"

if [[ ! -x "${ZEPHYR_WORKSPACE}/.venv/bin/python" || ! -d "${ZEPHYR_SDK_INSTALL_DIR}/xtensa-espressif_esp32_zephyr-elf" ]]; then
  echo "Zephyr is not fully installed; run ${_zephyr_app_dir}/scripts/setup-zephyr.sh" >&2
  unset _zephyr_app_dir
  return 1 2>/dev/null || exit 1
fi

case ":${PATH}:" in
  *":${ZEPHYR_WORKSPACE}/.venv/bin:"*) ;;
  *) export PATH="${ZEPHYR_WORKSPACE}/.venv/bin:${PATH}" ;;
esac

unset _zephyr_app_dir
