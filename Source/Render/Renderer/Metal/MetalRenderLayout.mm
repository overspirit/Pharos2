#include "PreCompile.h"
#include "Pharos.h"

MetalRenderLayout::MetalRenderLayout()
{
}

MetalRenderLayout::~MetalRenderLayout()
{
}

bool MetalRenderLayout::CreateVertexBuffer(uint32 sizeInBytes, MemoryBuffer* data)
{
//    MTKMeshBufferAllocator *metalAllocator = [[MTKMeshBufferAllocator alloc]
//                                              initWithDevice: _device];
//    
//    MDLMesh *mdlMesh = [MDLMesh newBoxWithDimensions:(vector_float3){4, 4, 4}
//                                            segments:(vector_uint3){2, 2, 2}
//                                        geometryType:MDLGeometryTypeTriangles
//                                       inwardNormals:NO
//                                           allocator:metalAllocator];
//    
//    MDLVertexDescriptor *mdlVertexDescriptor =
//    MTKModelIOVertexDescriptorFromMetal(_mtlVertexDescriptor);
//    
//    mdlVertexDescriptor.attributes[VertexAttributePosition].name  = MDLVertexAttributePosition;
//    mdlVertexDescriptor.attributes[VertexAttributeTexcoord].name  = MDLVertexAttributeTextureCoordinate;
//    
//    mdlMesh.vertexDescriptor = mdlVertexDescriptor;
//    
//    _mesh = [[MTKMesh alloc] initWithMesh:mdlMesh
//                                   device:_device
//                                    error:&error];
//    
//    if(!_mesh || error)
//    {
//        NSLog(@"Error creating MetalKit mesh %@", error.localizedDescription);
//    }
    
	return true;
}

bool MetalRenderLayout::CreateIndexBuffer(uint32 indexNum, MemoryBuffer* data)
{
	return true;
}

void MetalRenderLayout::CopyVertexBuffer(MemoryBuffer* data, uint32 dataSize, uint32 nOffsetInBytes, bool bNoOverwrite)
{
}

void MetalRenderLayout::CopyIndexBuffer(MemoryBuffer* data, uint32 dataSize, uint32 nOffsetInBytes, bool bNoOverwrite)
{
}

bool MetalRenderLayout::SetInputLayoutDesc(const VertLayoutDesc* desc, uint32 descNum)
{
//    _mtlVertexDescriptor = [[MTLVertexDescriptor alloc] init];
//
//    _mtlVertexDescriptor.attributes[VertexAttributePosition].format = MTLVertexFormatFloat3;
//    _mtlVertexDescriptor.attributes[VertexAttributePosition].offset = 0;
//    _mtlVertexDescriptor.attributes[VertexAttributePosition].bufferIndex = BufferIndexMeshPositions;
//
//    _mtlVertexDescriptor.attributes[VertexAttributeTexcoord].format = MTLVertexFormatFloat2;
//    _mtlVertexDescriptor.attributes[VertexAttributeTexcoord].offset = 0;
//    _mtlVertexDescriptor.attributes[VertexAttributeTexcoord].bufferIndex = BufferIndexMeshGenerics;
//
//    _mtlVertexDescriptor.layouts[BufferIndexMeshPositions].stride = 12;
//    _mtlVertexDescriptor.layouts[BufferIndexMeshPositions].stepRate = 1;
//    _mtlVertexDescriptor.layouts[BufferIndexMeshPositions].stepFunction = MTLVertexStepFunctionPerVertex;
//
//    _mtlVertexDescriptor.layouts[BufferIndexMeshGenerics].stride = 8;
//    _mtlVertexDescriptor.layouts[BufferIndexMeshGenerics].stepRate = 1;
//    _mtlVertexDescriptor.layouts[BufferIndexMeshGenerics].stepFunction = MTLVertexStepFunctionPerVertex;
    
	return true;
}

void MetalRenderLayout::ApplyDevice()
{
}
