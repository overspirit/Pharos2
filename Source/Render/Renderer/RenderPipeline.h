#pragma once

namespace Pharos
{
    namespace Render
    {
        enum VertElementType
        {
            VET_FLOAT32,
            VET_FLOAT16,
            
            VET_SINT32,
            VET_SINT16,
            VET_SINT8,
            
            VET_UINT32,
            VET_UINT16,
            VET_UINT8,
            
            VET_SNORM16,
            VET_SNORM8,
            
            VET_UNORM16,
            VET_UNORM8,
        };
        
        enum IndexElementType
        {
            IET_UINT32,
            IET_UINT16,
        };
        
        enum VertAttrLocation
        {
            VAL_POSITION = 0,
            VAL_COLOR,
            VAL_NORMAL,
            VAL_TANGENT_QUAT,
            VAL_TANGENT,
            VAL_WEIGHT,
            VAL_INDICES,
            VAL_TEXCOORD0,
            VAL_TEXCOORD1,
            VAL_TEXCOORD2,
            VAL_TEXCOORD3
        };
        
        struct VertLayoutDesc
        {
            VertElementType         elementType;
            uint32                  elementNum;
            VertAttrLocation        elementLocation;
            uint32                  elementOffset;
            uint32                  layoutIndex;
        };
        
        class RenderPipeline
        {
            public:
            virtual ~RenderPipeline() {}
            
            protected:
            vector<VertLayoutDesc>    m_layoutDesc;
            
            public:
            virtual bool SetInputLayoutDesc(const VertLayoutDesc* desc, uint32 descNum) = 0;
            
            virtual bool SetVertexShader(RenderProgram* program, const char8* enter) = 0;
            virtual bool SetPixelShader(RenderProgram* program, const char8* enter) = 0;
            
            virtual void SetRenderTarget(RenderTarget* target) = 0;
            
            virtual void SetBlendState(RenderBlendState* state) = 0;
            virtual void SetRasterizerState(RenderRasterizerState* state) = 0;
            //virtual void SetDepthStencilState(RenderDepthStencilState* state) = 0;
        };
    }
}
