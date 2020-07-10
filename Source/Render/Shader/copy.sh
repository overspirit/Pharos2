#!/usr/bin/env bash

SHADER_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")"; pwd)

function main() {
    echo "copy shader fxml...."    
    echo "shader dir: ${SHADER_DIR}"
    echo "output dir: $1"

    cp ${SHADER_DIR}/Sprite3D.fxml $1
    cp ${SHADER_DIR}/Skeletal.fxml $1
}

# main entry
main $@
