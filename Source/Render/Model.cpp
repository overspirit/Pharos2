#include "PreCompile.h"
#include "Pharos.h"

Model::Model()
{
	m_currAnim = nullptr;
	m_animPlayTime = 0;
	m_stopAnim = true;
	m_playSpeed = 1.0f;
	m_currAnimFrame = 0;
}

Model::~Model()
{
	for (uint32 i = 0; i < m_subModelList.size(); i++)
	{
		SAFE_DELETE(m_subModelList[i].mesh);

		for (auto* material : m_subModelList[i].materialList)
		{
			SAFE_DELETE(material);
		}
	}
}

uint32 Model::AddSubModelMesh(Mesh* mesh)
{
	uint32 index = m_subModelList.size();

	m_subModelList.resize(index + 1);

	m_subModelList[index].mesh = mesh;

	return index;
}

void Model::AddSubModelMaterial(uint32 index, Material* material)
{
	if (index >= m_subModelList.size())
	{
		m_subModelList.resize(index + 1);
	}

	m_subModelList[index].materialList.push_back(material);
}

void Model::SetBoneInfo(const char8* name, int32 id, int32 parentId, const Matrix4& bindPose)
{
	if (id >= m_boneInfoList.size())
	{
		m_boneInfoList.resize(id + 1);
		m_animBoneTrans.resize(id + 1);
	}

	m_boneInfoList[id] = { name, id, parentId, bindPose };
}

SkelAnimation& Model::AddSkelAnimation(const char8* name)
{
	uint32 slot = (uint32)m_animList.size();
	m_animList.resize(slot + 1);
	m_animSlotMap[name] = slot;
	return *m_animList.rbegin();
}

//数据的传递要用引用
//数据的顺序要和id对应
void Model::UpdateAnimation(float32 elapsed)
{
	if (m_currAnim != nullptr && !m_stopAnim)
	{
		uint32 frameNum = (uint32)m_currAnim->frameList.size();
		if (frameNum < 2) return;	//只有1帧的动画是无法计算的....

		uint32 lastFrame = frameNum - 1;

		//最后一帧动画的时间戳，超过这个时间戳就意味这个动画播放结束
		float32 lastFrameTime = m_currAnim->frameList[lastFrame].time;
		if (m_animPlayTime >= lastFrameTime)
		{
			m_animPlayTime = lastFrameTime;
			m_currAnimFrame = lastFrame;

			m_stopAnim = true;

			CalcSkelAnimMatrix(m_currAnim, lastFrame, lastFrame, 0.0f);

			return;
		}

		//插值比例，取值0-1之间
		float32 lerp = 0;

		for (uint32 i = m_currAnimFrame; i < lastFrame; i++)
		{
			float32 currFrameTime = m_currAnim->frameList[i].time;
			float32 nextFrameTime = m_currAnim->frameList[i + 1].time;
			if (m_animPlayTime >= currFrameTime && m_animPlayTime < nextFrameTime)
			{
				m_currAnimFrame = i;
				lerp = (m_animPlayTime - currFrameTime) / (nextFrameTime - currFrameTime); //注意lerp取值0-1之间，所以除以时间差以保证在0-1之间
				break;
			}
		}

		CalcSkelAnimMatrix(m_currAnim, m_currAnimFrame, m_currAnimFrame + 1, lerp);

		m_animPlayTime += (elapsed * m_playSpeed);
	}
}

void Model::CalcSkelAnimMatrix(const SkelAnimation* anim, uint32 currFrameIndex, uint32 nextFrameIndex, float32 lerp)
{
	//取当前帧和下一帧的数据，根据lerp进行插值
	if (anim == nullptr) return;

	const SkelFrame& currFrame = anim->frameList[currFrameIndex];
	const SkelFrame& nextFrame = anim->frameList[nextFrameIndex];

	for (uint32 i = 0; i < currFrame.tranList.size(); i++)
	{
		const BoneTran& currBoneTran = currFrame.tranList[i];
		const BoneTran& nextBoneTran = nextFrame.tranList[i];

		//旋转要进行球面插值
		Quaternion rota;
		rota.Slerp(currBoneTran.rota, nextBoneTran.rota, lerp);

		//位移要进行线性插值
		Vector3Df tran;
		tran.Interpolate(currBoneTran.tran, nextBoneTran.tran, lerp);

		Matrix4 matTran = rota.GetMatrix();
		matTran[12] = tran.x;
		matTran[13] = tran.y;
		matTran[14] = tran.z;

		BoneInfo& boneInfo = m_boneInfoList[currBoneTran.bone_id];
		if (boneInfo.parentId != 0xFFFFFFFF)
		{
			BoneInfo& parentBoneInfo = m_boneInfoList[boneInfo.parentId];
			m_animBoneTrans[boneInfo.id] = matTran * m_animBoneTrans[parentBoneInfo.id];
		}
		else
		{
			m_animBoneTrans[boneInfo.id] = matTran;
		}
	}

	for (uint32 i = 0; i < m_boneInfoList.size(); i++)
	{
		m_animBoneTrans[i] = m_boneInfoList[i].bindPose * m_animBoneTrans[i];
	}
}

void Model::SetCurrentAnimation(const char8* animName)
{
	auto iter = m_animSlotMap.find(animName);
	uint32 slot = iter->second;
	if (slot >= m_animList.size()) return;

	m_currAnim = (iter != m_animSlotMap.end()) ? &m_animList[slot] : nullptr;

	m_animPlayTime = 0;
	m_currAnimFrame = 0;

	CalcSkelAnimMatrix(m_currAnim, 0, 0, 0.0f);
}

void Model::PlayAnimation()
{
	m_stopAnim = false;
}

void Model::PauseAnimation()
{
	m_stopAnim = true;
}

void Model::StopAnimation()
{
	m_stopAnim = true;

	m_animPlayTime = 0;
	m_currAnimFrame = 0;

	CalcSkelAnimMatrix(m_currAnim, 0, 0, 0.0f);
}

void Model::SetCurrentAnimationFrame(uint32 frame)
{
	if (m_currAnim == nullptr) return;

	uint32 maxFrame = (m_currAnim != nullptr) ? (uint32)m_currAnim->frameList.size() : 0;
	m_currAnimFrame = Math::minimum(frame, maxFrame);
	m_animPlayTime = m_currAnim->frameList[m_currAnimFrame].time;

	CalcSkelAnimMatrix(m_currAnim, m_currAnimFrame, m_currAnimFrame, 0.0f);
}

uint32 Model::GetCurrentAnimationFrameNum()
{
	return (m_currAnim != nullptr) ? (uint32)m_currAnim->frameList.size() : 0;
}

const char8* Model::GetAnimationName(uint32 index)
{
	if (index >= m_animList.size()) return "";
	return m_animList[index].name.c_str();
}

const char8* Model::GetCurrentAnimationName()
{
	return (m_currAnim != nullptr) ? m_currAnim->name.c_str() : "";
}

void Model::TransformWorld(const Matrix4& world)
{
	m_world = world * m_offset;
}

void Model::Prepare(RenderObject* renderObj)
{
	if (m_animBoneTrans.size() > 0)
	{
		//m_renderBlock->SetBlockDataBoneMatrix(&*m_animBoneTrans.begin(), (uint32)m_animBoneTrans.size());
	}

	for (uint32 i = 0; i < m_subModelList.size(); i++)
	{
		SubModel& subModel = m_subModelList[i];
		Mesh* mesh = subModel.mesh;
		
		RenderBuffer* vertBuffer = mesh->GetMeshVertexBuffer();
		vector<VertLayoutDesc> vertDesc = mesh->GetMeshVertexDesc();

		RenderBuffer* indexBuffer = mesh->GetMeshIndexBuffer();
		DrawType drawType = mesh->GetDrawType();
		uint32 vertNum = mesh->GetVertNum();
		uint32 faceNum = mesh->GetFaceNum();

		vector<Material*>& materialList = subModel.materialList;

		for (Material* material : materialList)
		{
			material->SetWorldParamValue(m_world);

			RenderPipeline* pipeline = material->GetRenderPipeline();
			pipeline->SetInputLayoutDesc(vertDesc.data(), vertDesc.size());

			RenderResourceSet* resSet = material->GetRenderResourceSet();

			uint32 blockIndex = renderObj->AddRenderBlock(vertBuffer, pipeline);
			renderObj->SetBlockResourceSet(blockIndex, resSet);

			if (indexBuffer == nullptr)	
			{
				renderObj->SetBlockDrawInfo(blockIndex, drawType, vertNum);
			}
			else
			{
				renderObj->SetBlockIndexBuffer(blockIndex, indexBuffer);

				renderObj->SetBlockDrawInfo(blockIndex, drawType, faceNum * 3);				
			}			
		}
	}
}
