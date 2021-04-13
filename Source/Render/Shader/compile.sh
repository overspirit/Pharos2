#!/usr/bin/env bash

SHADER_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")"; pwd)
VULKAN_SHADER_DIR="${SHADER_DIR}/Vulkan"

function main() {

    OUTPUT_DIR=$1

    ${VULKAN_SHADER_DIR}/compile_vulkan.sh ${OUTPUT_DIR}
    if [ $? -eq 0 ];then
        echo "compile vulkan shader success!!!"
    else 
        echo "compile fail!!!"
        return 1
    fi

    echo "copy shader fxml...."    
    echo "shader dir: ${SHADER_DIR}"
    echo "output dir: ${OUTPUT_DIR}"

    cp ${SHADER_DIR}/Sprite3D.fxml ${OUTPUT_DIR}
    cp ${SHADER_DIR}/Skeletal.fxml ${OUTPUT_DIR}
    cp ${SHADER_DIR}/BorderShine.fxml ${OUTPUT_DIR}
    cp ${SHADER_DIR}/Phong.fxml ${OUTPUT_DIR}
    cp ${SHADER_DIR}/Blinn.fxml ${OUTPUT_DIR}
    cp ${SHADER_DIR}/Lambert.fxml ${OUTPUT_DIR}
}

# main entry
main $@

