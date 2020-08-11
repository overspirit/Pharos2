#!/usr/bin/env bash
PROJECT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")"; pwd)
echo "project dir: ${PROJECT_DIR}"

DATA_DIR=${PROJECT_DIR}/Data
echo "data dir: ${DATA_DIR}"

SHADER_DIR=${PROJECT_DIR}/Source/Render/Shader
#echo "shader resource dir: ${SHADER_DIR}"

VULKAN_SHADER_DIR=${SHADER_DIR}/Vulkan
#echo "vulkan shader resource dir: ${VULKAN_SHADER_DIR}"

function clean() {
    echo "clean build dir..."
    rm -rf ${PROJECT_DIR}/build

    echo "clean output dir..."
    rm -rf ${PROJECT_DIR}/output

}

function main() {
    arg=$@
    while getopts "c" arg
    do
        case $arg in
            c)
                clean
                ;;
        esac
    done

    echo "build_all"

    ${SHADER_DIR}/copy.sh ${DATA_DIR}/Shader
    if [ $? -eq 0 ];then
        echo "copy success!!!"
    else 
        echo "copy fail!!!"
        return 1
    fi

    ${VULKAN_SHADER_DIR}/compile.sh ${DATA_DIR}/Shader
    if [ $? -eq 0 ];then
        echo "compile success!!!"
    else 
        echo "compile fail!!!"
        return 1
    fi

    cmake -H. -Bbuild

    cd build
    make -j8
    if [ $? -eq 0 ];then
        echo "build success!!!"
    else 
        echo "build fail!!!"
        return 1
    fi

    cd ..
    mkdir output 
    cd output
    mkdir Sample
    cp -r ../build/Sample/SceneDemo/SceneDemo ./Sample/
    cp -r ../build/Sample/ModelViewer/ModelViewer ./Sample/
    cp -r ../Data/ ./
}

# main entry
main $@
