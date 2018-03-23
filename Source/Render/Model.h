#pragma once

namespace Pharos
{
	namespace Render
	{
		struct BoneTran
		{
			int32	bone_id;
			Quaternion rota;
			Vector3Df tran;
			Vector3Df scale;
		};

		struct SkelFrame
		{
			float32 time;
			uint32 joint;
			vector<BoneTran> tranList;
		};

		struct SkelAnimation
		{
			string name;
			vector<SkelFrame> frameList;
		};

		struct BoneInfo
		{
			string				name;
			int32				id;
			int32				parentId;			
			Matrix4				bindPose;
			vector<BoneInfo>	children;
		};

		class Model
		{
		public:
			Model();
			virtual ~Model();

		private:
			Matrix4							m_offset;
			Matrix4							m_world;

			vector<Mesh*>					m_meshGroupList;
			vector<RenderBlock*>			m_blockList;
			vector<BoneInfo>				m_boneInfoList;
			map<string, SkelAnimation>		m_animList;
			
			const SkelAnimation*		m_currAnim;
			float32						m_animPlayTime;
			bool						m_stopAnim;
			bool						m_stopAnimFlag;
			bool						m_playLoop;
			float32						m_playSpeed;
			uint32						m_currAnimFrame;

			vector<Matrix4>				m_animBoneTrans;

		private:
// 			void CalcBoneSpace(const BoneTree& boneTree, const BoneTran* boneTrans);
// 
// 			void UpdateAnimMat(const BoneTree& boneTree);

		public:
			virtual void UpdateAnimation(float32 elapsed);

		public:
			virtual void AddMesh(Mesh* mesh);
			virtual void SetBoneInfo(const char8* name, int32 id, int32 parentId, const Matrix4& bindPose);
			virtual SkelAnimation& AddSkelAnimation(const char8* name);

			virtual void SetCurrentAnimation(const char8* animName);

			virtual void PlayAnimation(bool loop = false);
			virtual void PauseAnimation();
			virtual void StopAnimation();

			virtual void SetCurrentAnimationFrame(uint32 frame);
			virtual void SetPlayAnimationSpeed(float32 scale) { m_playSpeed = scale; }

			virtual uint32 GetAnimationFrameNum(const char8* animName);
			virtual uint32 GetAnimationNum() { return (uint32)m_animList.size(); }

			virtual const char8* GetCurrentAnimationName();
			virtual float32 GetPlayAnimationSpeed() { return m_playSpeed; }
			virtual uint32 GetCurrentAnimationFrame() { return m_currAnimFrame; }

			virtual void TransformWorld(const Matrix4& world);

			virtual void Draw();
		};
	}
}