#!/usr/bin/env bash

SHADER_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")"; pwd)

function main() {
    output_dir=$1
    echo "copy shader fxml...."    
    echo "shader dir: ${SHADER_DIR}"
    echo "output dir: ${output_dir}"

    cp ${SHADER_DIR}/Sprite3D.fxml ${output_dir}
    cp ${SHADER_DIR}/Skeletal.fxml ${output_dir}
}

# main entry
main $@
