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
			vector<BoneInfo>				m_boneInfoList;
			vector<SkelAnimation>			m_animList;
			map<string, uint32>				m_animSlotMap;
			
			const SkelAnimation*		m_currAnim;			//当前播放动画数据
			float32						m_animPlayTime;		//当前动画总的播放时间
			bool						m_stopAnim;			//是否停止动画播放
			float32						m_playSpeed;		//动画播放速度
			uint32						m_currAnimFrame;	//当前播放动画的播放帧索引
			
			RenderBlock*				m_renderBlock;
			vector<Matrix4>				m_animBoneTrans;

		private:
			void CalcSkelAnimMatrix(const SkelAnimation* anim, uint32 currFrameIndex, uint32 nextFrameIndex, float32 lerp);

		public:
			virtual void AddMesh(Mesh* mesh);
			virtual void SetBoneInfo(const char8* name, int32 id, int32 parentId, const Matrix4& bindPose);
			virtual SkelAnimation& AddSkelAnimation(const char8* name);

			virtual uint32 GetAnimationNum() { return (uint32)m_animList.size(); }
			virtual const char8* GetAnimationName(uint32 index);

			virtual void SetCurrentAnimation(const char8* animName);
			virtual const char8* GetCurrentAnimationName();
			virtual uint32 GetCurrentAnimationFrameNum();
			virtual void SetCurrentAnimationFrame(uint32 frame);
			virtual uint32 GetCurrentAnimationFrame() { return m_currAnimFrame; }

			virtual void PlayAnimation();
			virtual void PauseAnimation();
			virtual void StopAnimation();
			
			virtual void SetPlayAnimationSpeed(float32 scale) { m_playSpeed = scale; }
			virtual float32 GetPlayAnimationSpeed() { return m_playSpeed; }

			virtual void TransformWorld(const Matrix4& world);

			virtual void UpdateAnimation(float32 elapsed);

			virtual void Draw();
		};
	}
}