#!/usr/bin/env bash

set -e

if [ $MOD_BUILD_DIR ]; then
    cd $MOD_BUILD_DIR
else
    cd $(dirname ${0})
fi

DESTDIR=${1:-build}
INSTALL_PATH=${DESTDIR}/usr/local/lib/lv2/audio-to-midi.lv2

mkdir -p ${INSTALL_PATH}

if [ x"${MOD_BUILD_DIR}" == x"" ]; then
    CFLAGS="-I. -fPIC -DPIC"
    LDFLAGS="-shared -lm"
else
    CFLAGS="${MOD_CFLAGS}"
    LDFLAGS="${MOD_LDFLAGS}"
fi

# Build the plugin
${CC} ${CFLAGS} src/audio-to-midi.c -o ${INSTALL_PATH}/audio-to-midi.so ${LDFLAGS}

# Copy all the plugin resources
cp audio-to-midi.lv2/*.ttl ${INSTALL_PATH}/
cp -r audio-to-midi.lv2/modgui ${INSTALL_PATH}/