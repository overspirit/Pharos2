#!/usr/bin/env bash
PROJECT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")"; pwd)
echo "project dir: ${PROJECT_DIR}"

DATA_DIR=${PROJECT_DIR}/Data
echo "data dir: ${DATA_DIR}"

SHADER_RESOURCE_DIR=${PROJECT_DIR}/Source/Render/Shader/Vulkan
echo "shader resource dir: ${SHADER_RESOURCE_DIR}"

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

    #glslangValidator ${SHADER_RESOURCE_DIR}/shaders.vert -V -o ${SHADER_RESOURCE_DIR}/vertexShader.spv
    #glslangValidator ${SHADER_RESOURCE_DIR}/shaders.frag -V -o ${SHADER_RESOURCE_DIR}/fragmentShader.spv
    #zip ${DATA_DIR}/Shader/default.lib ${SHADER_RESOURCE_DIR}/vertexShader.spv ${SHADER_RESOURCE_DIR}/fragmentShader.spv
    #rm ${SHADER_RESOURCE_DIR}/vertexShader.spv
    #rm ${SHADER_RESOURCE_DIR}/fragmentShader.spv

    ${SHADER_RESOURCE_DIR}/compile.sh ${DATA_DIR}/Shader
    if [ $? -eq 0 ];then
        echo "build success!!!"
    else 
        echo "build fail!!!"
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
    cp -r ../Data/ ./
}

# main entry
main $@
