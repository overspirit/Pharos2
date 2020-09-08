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

    echo "compile Sprite3D success!!!"
}

function compileSkeletal() {
    output_lib=$1/Skeletal.lib
    echo "compiling Skeletal lib..."
    echo "output lib: ${output_lib}"

    glslangValidator ${COMPILE_DIR}/Skeletal.vert -V -o ${COMPILE_DIR}/SkeletalVS.spv
    if [ $? -eq 0 ];then
        echo "compile success!!!"
    else 
        echo "compile fail!!!"
        return 1
    fi

    glslangValidator ${COMPILE_DIR}/Skeletal.frag -V -o ${COMPILE_DIR}/SkeletalPS.spv
    if [ $? -eq 0 ];then
        echo "compile success!!!"
    else 
        echo "compile fail!!!"
        return 1
    fi

    zip -j ${output_lib} \
        ${COMPILE_DIR}/SkeletalVS.spv \
        ${COMPILE_DIR}/SkeletalPS.spv

    rm ${COMPILE_DIR}/*.spv

    echo "compile Skeletal success!!!"
}

function compileSprite2D() {
    output_lib=$1/Sprite2D.lib
    echo "compiling Sprite2D lib..."
    echo "output lib: ${output_lib}"

    echo "compile Sprite2D vert..."
    glslangValidator ${COMPILE_DIR}/Sprite2D.vert -V -o ${COMPILE_DIR}/Sprite2DVS.spv
    if [ $? -eq 0 ];then
        echo "compile success!!!"
    else 
        echo "compile fail!!!"
        return 1
    fi

    echo "compile Sprite2DColor frag..."
    glslangValidator ${COMPILE_DIR}/Sprite2DColor.frag -V -o ${COMPILE_DIR}/Sprite2DColorPS.spv
    if [ $? -eq 0 ];then
        echo "compile success!!!"
    else
        echo "compile fail!!!"
        return 1
    fi

    echo "compile Sprite2DTexture frag..."
    glslangValidator ${COMPILE_DIR}/Sprite2DTexture.frag -V -o ${COMPILE_DIR}/Sprite2DTexturePS.spv
    if [ $? -eq 0 ];then
        echo "compile success!!!"
    else
        echo "compile fail!!!"
        return 1
    fi

    zip -j ${output_lib} \
        ${COMPILE_DIR}/Sprite2DVS.spv \
        ${COMPILE_DIR}/Sprite2DColorPS.spv \
        ${COMPILE_DIR}/Sprite2DTexturePS.spv \

    rm ${COMPILE_DIR}/*.spv

    echo "compile Sprite2D success!!!"
}

function compileFont() {
    output_lib=$1/Font.lib
    echo "compiling Font lib..."
    echo "output lib: ${output_lib}"

    echo "compile FontVS..."
    glslangValidator ${COMPILE_DIR}/Font.vert -V -o ${COMPILE_DIR}/FontVS.spv
    if [ $? -eq 0 ];then
        echo "compile success!!!"
    else 
        echo "compile fail!!!"
        return 1
    fi

    echo "compile FontPS..."
    glslangValidator ${COMPILE_DIR}/Font.frag -V -o ${COMPILE_DIR}/FontPS.spv
    if [ $? -eq 0 ];then
        echo "compile success!!!"
    else 
        echo "compile fail!!!"
        return 1
    fi

    zip -j ${output_lib} \
        ${COMPILE_DIR}/FontVS.spv \
        ${COMPILE_DIR}/FontPS.spv

    rm ${COMPILE_DIR}/*.spv

    echo "compile Font success!!!"
}

function main() {
    output_dir=$1

    echo "compile all shader..."    
    echo "vulkan shader resource dir: ${COMPILE_DIR}"
    echo "output dir: ${output_dir}"

    compileSprite3D ${output_dir}
    compileSkeletal ${output_dir}
    compileSprite2D ${output_dir}
    compileFont ${output_dir}
}

# main entry
main $@
