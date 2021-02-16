#pragma once

class Converter
{
public:
    Converter();
    ~Converter();
    
private:
    struct Vertex
    {
        Vector3Df pos;
        Vector3Df normal;
    };
    
    struct MeshInfo
    {
        string name;
        std::vector<Vertex> vertList;
        std::vector<int> indicesList;
    };
    
private:
    string      m_name;
    
    vector<MeshInfo>    m_meshInfos;
    
private:
    Matrix4 GetFbxNodeTransform(FbxNode* node);
    Vector3Df GetFbxMeshNormal(FbxGeometryElementNormal* normals, int ctrlPointIndex, int vertCounter);
    void ProcessFbxMesh(FbxMesh* mesh, std::vector<Vertex>& vertList, std::vector<int>& indicesList, Matrix4& transMatrix);
    
    XmlNode* AppendVectorNode(XmlNode* parent, const char8* name, const Vector3Df& v);
    
public:
    void SetName(const char8* name) { m_name = name; }
    
    bool OpenFbx(const char8* path);
    
    bool Save(const char8* path);
};
