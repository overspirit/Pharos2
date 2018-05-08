#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

Model::Model()
{
	m_currAnim = nullptr;
	m_animPlayTime = 0;
	m_stopAnim = true;
	m_stopAnimFlag = false;
	m_playLoop = false;
	m_playSpeed = 1.0f;
	m_currAnimFrame = 0;
}

Model::~Model()
{
	for (uint32 i = 0; i < m_meshGroupList.size(); i++)
	{
		SAFE_DELETE(m_meshGroupList[i]);
		SAFE_DELETE(m_blockList[i]);
	}
}

void Model::AddMesh(Mesh* mesh)
{
	if (mesh != nullptr)
	{
		RenderBlock* block = sRenderMgr->GenerateRenderBlock();
		m_blockList.push_back(block);

		m_meshGroupList.push_back(mesh);
	}
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
	return m_animList[name];
}

//数据的传递要用引用
//数据的顺序要和id对应
void Model::UpdateAnimation(float32 elapsed)
{	
	if (m_currAnim != nullptr && !m_stopAnim)
	{
		m_stopAnim = m_stopAnimFlag;

		uint32 frameNum = (uint32)m_currAnim->frameList.size();
		if (frameNum < 2) return;	//只有1帧的动画是无法计算的....

		//最后一帧动画的时间戳，超过这个时间戳就意味这个动画播放结束
		float32 lastFrameTime = m_currAnim->frameList[frameNum - 1].time;
		if (m_animPlayTime >= lastFrameTime)
		{
			m_animPlayTime = m_animPlayTime - lastFrameTime;
			m_currAnimFrame = 0;
			if (!m_playLoop)
			{
				m_stopAnim = true;
				return;
			}
		}

		//插值比例，取值0-1之间
		float32 lerp = 0;

		for (uint32 i = m_currAnimFrame; i < frameNum - 1; i++)
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

		//取当前帧和下一帧的数据，根据lerp进行插值
		const SkelFrame& currFrame = m_currAnim->frameList[m_currAnimFrame];
		const SkelFrame& nextFrame = m_currAnim->frameList[m_currAnimFrame + 1];
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

		m_animPlayTime += (elapsed * m_playSpeed);
	}
}

void Model::SetCurrentAnimation(const char8* animName)
{
	auto iter = m_animList.find(animName);
	m_currAnim = (iter != m_animList.end()) ? &iter->second : nullptr;

	m_animPlayTime = 0;

	if (m_stopAnim)
	{
		m_stopAnim = false;
		m_stopAnimFlag = true;
	}

	m_currAnimFrame = 0;
}

void Model::PlayAnimation(bool loop)
{  
	m_stopAnim = false;
	m_stopAnimFlag = false;
	m_playLoop = loop;
}

void Model::PauseAnimation()
{
	m_stopAnim = true;
	m_stopAnimFlag = false;
}

void Model::StopAnimation()
{
	m_stopAnim = false;
	m_stopAnimFlag = true;
	m_playLoop = false;

	m_animPlayTime = 0;
	m_currAnimFrame = 0;
}

void Model::SetCurrentAnimationFrame(uint32 frame)
{
	uint32 maxFrame = (m_currAnim != nullptr) ? (uint32)m_currAnim->frameList.size() : 0;
	m_currAnimFrame = Math::minimum(frame, maxFrame);
	m_animPlayTime = m_currAnim->frameList[m_currAnimFrame].time;

	if (m_stopAnim)
	{
		m_stopAnim = false;
		m_stopAnimFlag = true;
	}
}

uint32 Model::GetAnimationFrameNum(const char8* animName)
{
	return (m_currAnim != nullptr) ? (uint32)m_currAnim->frameList.size() : 0;
}

const char8* Model::GetCurrentAnimationName()
{
	return (m_currAnim != nullptr) ? m_currAnim->name.c_str() : "";
}

void Model::TransformWorld(const Matrix4& world)
{
	m_world = world * m_offset;
}

void Model::Draw()
{
	for (uint32 i = 0; i < m_meshGroupList.size(); i++)
	{
		Material* material = m_meshGroupList[i]->GetAttachMaterial();
		if(material == nullptr) continue;

		RenderLayout* layout = m_meshGroupList[i]->GetRenderLayout();
		DrawType drawType = m_meshGroupList[i]->GetDrawType();
		RenderTechnique* tech = material->GetMaterialTechnique();
		
		RenderBlock* block = m_blockList[i];
		block->SetBlockDataWorldMatrix(m_world);

		if (m_animBoneTrans.size() > 0)
		{
			block->SetBlockDataBoneMatrix(&*m_animBoneTrans.begin(), (uint32)m_animBoneTrans.size());
		}

		block->BindLayout(layout);
		block->BindTechnique(tech);
		block->SetDrawType(drawType);
		
		sRenderMgr->DoRender(block);
	}
}