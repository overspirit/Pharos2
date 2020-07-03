#!/usr/bin/env bash

COMPILE_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")"; pwd)

function compileSprite3D() {
    output_lib=$1/Sprite3D.lib
    echo "compiling Sprite3D lib..."
    echo "output dir: ${output_lib}"

    glslangValidator ${COMPILE_DIR}/Sprite3DColor.vert -V -o ${COMPILE_DIR}/Sprite3DColorVS.spv
    glslangValidator ${COMPILE_DIR}/Sprite3DNormal.vert -V -o ${COMPILE_DIR}/Sprite3DNormalVS.spv
    glslangValidator ${COMPILE_DIR}/Sprite3DTexture.vert -V -o ${COMPILE_DIR}/Sprite3DTextureVS.spv
    glslangValidator ${COMPILE_DIR}/Sprite3DTextureWithColor.vert -V -o ${COMPILE_DIR}/Sprite3DTextureWithColorVS.spv
    glslangValidator ${COMPILE_DIR}/Sprite3DTextureWithNormal.vert -V -o ${COMPILE_DIR}/Sprite3DTextureWithNormalVS.spv
    glslangValidator ${COMPILE_DIR}/Sprite3DColor.frag -V -o ${COMPILE_DIR}/Sprite3DColorPS.spv
    glslangValidator ${COMPILE_DIR}/Sprite3DTexture.frag -V -o ${COMPILE_DIR}/Sprite3DTexturePS.spv

    zip -j ${output_lib} \
        ${COMPILE_DIR}/Sprite3DColorVS.spv \
        ${COMPILE_DIR}/Sprite3DNormalVS.spv \
        ${COMPILE_DIR}/Sprite3DTextureVS.spv \
        ${COMPILE_DIR}/Sprite3DTextureWithColorVS.spv \
        ${COMPILE_DIR}/Sprite3DTextureWithNormalVS.spv \
        ${COMPILE_DIR}/Sprite3DColorPS.spv \
        ${COMPILE_DIR}/Sprite3DTexturePS.spv

    rm ${COMPILE_DIR}/*.spv
    cp ${COMPILE_DIR}/Sprite3D.fxml $1

    echo "compile Sprite3D success!!!"
}

function compileSkeletal() {
    output_lib=$1/Skeletal.lib
    echo "compiling Skeletal lib..."
    echo "output lib: ${output_lib}"

    glslangValidator ${COMPILE_DIR}/Skeletal.vert -V -o ${COMPILE_DIR}/SkeletalVS.spv
    glslangValidator ${COMPILE_DIR}/Skeletal.frag -V -o ${COMPILE_DIR}/SkeletalPS.spv

    zip -j ${output_lib} \
        ${COMPILE_DIR}/SkeletalVS.spv \
        ${COMPILE_DIR}/SkeletalPS.spv

    rm ${COMPILE_DIR}/*.spv
    cp ${COMPILE_DIR}/Skeletal.fxml $1

    echo "compile Skeletal success!!!"
}

function main() {
    output_dir=$1

    echo "compile_all"    
    echo "shader resource dir: ${COMPILE_DIR}"
    echo "output dir: ${output_dir}"

    compileSprite3D ${output_dir}
    compileSkeletal ${output_dir}
}

# main entry
main $@
