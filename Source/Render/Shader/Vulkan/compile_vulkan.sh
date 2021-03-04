#!/bin/bash

VULKAN_SHADER_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")"; pwd)
OUTPUT_DIR=$1

echo "vulkan shader dir: ${VULKAN_SHADER_DIR}"
echo "output dir: ${OUTPUT_DIR}"

function compile() {
    package_name=$1
    vs_list=$2
    ps_list=$3
    package_list=()

    echo "####################### compiling ${package_name} lib... ############################"

    for vs in ${vs_list[*]}; do

      echo "compiling ${vs}..."

      file_name=${vs%.*}
      src_file="${VULKAN_SHADER_DIR}/${vs}"
      output_file="${VULKAN_SHADER_DIR}/${file_name}VS.spv"

      echo -n "source file: "
      glslangValidator ${src_file} -V -o ${output_file}
      if [ $? -eq 0 ];then
        package_list+=(${output_file})
        echo "compile success!!!"
        echo "output file: ${output_file}"
      else 
        echo "compile fail!!!"
        return 1
      fi
      
    done

    for ps in ${ps_list[*]}; do

      echo ""
      echo "compiling ${ps}..."

      file_name=${ps%.*}
      src_file="${VULKAN_SHADER_DIR}/${ps}"
      output_file="${VULKAN_SHADER_DIR}/${file_name}PS.spv"

      echo -n "source file: "
      glslangValidator ${src_file} -V -o ${output_file}
      if [ $? -eq 0 ];then  
        package_list+=(${output_file})      
        echo "compile success!!!"
        echo "output file: ${output_file}"
      else 
        echo "compile fail!!!"
        return 1
      fi
      
    done

    output_lib="${OUTPUT_DIR}/${package_name}.lib"
    echo ""
    echo "output lib: ${output_lib}"

    zip -j ${output_lib} ${package_list[*]}

    rm ${VULKAN_SHADER_DIR}/*.spv

    echo "################################################################################"
 }
    
#Sprite3D
################################################################################
Sprite3DVS=(
  Sprite3DColor.vert 
  Sprite3DNormal.vert 
  Sprite3DTexture.vert 
  Sprite3DTextureWithColor.vert 
  Sprite3DTextureWithNormal.vert
)
    
Sprite3DPS=(
  Sprite3DColor.frag
  Sprite3DTexture.frag
)

compile "Sprite3D" "${Sprite3DVS[*]}" "${Sprite3DPS[*]}"
################################################################################

#Skeletal
################################################################################
SkeletalVS=(
  Skeletal.vert
)

SkeletalPS=(
  Skeletal.frag
)

compile "Skeletal" "${SkeletalVS[*]}" "${SkeletalPS[*]}"
################################################################################

#Sprite2D
################################################################################
Sprite2DVS=(
  Sprite2D.vert
)

Sprite2DPS=(
  Sprite2DColor.frag
  Sprite2DTexture.frag
)

compile "Sprite2D" "${Sprite2DVS[*]}" "${Sprite2DPS[*]}"
################################################################################

#Font
################################################################################
FontVS=(
  Font.vert
)

FontPS=(
  Font.frag
)

compile "Font" "${FontVS[*]}" "${FontPS[*]}"
################################################################################

#Copy
################################################################################
CopyVS=(
  Copy.vert
)

CopyPS=(
  Copy.frag
)

compile "Copy" "${CopyVS[*]}" "${CopyPS[*]}"
################################################################################

#BorderShine
################################################################################
BorderShineVS=(
  BorderShine.vert
)

BorderShinePS=(
  BorderShine.frag
)

compile "BorderShine" "${BorderShineVS[*]}" "${BorderShinePS[*]}"
################################################################################

#Lambert
################################################################################
LambertVS=(
  Lambert.vert
)

LambertPS=(
  Lambert.frag
)

compile "Lambert" "${LambertVS[*]}" "${LambertPS[*]}"
################################################################################

#Blinn
################################################################################
BlinnVS=(
  Blinn.vert
)

BlinnPS=(
  Blinn.frag
)

compile "Blinn" "${BlinnVS[*]}" "${BlinnPS[*]}"
################################################################################