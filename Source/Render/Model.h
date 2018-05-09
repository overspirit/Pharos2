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
			map<string, SkelAnimation>		m_animList;
			
			const SkelAnimation*		m_currAnim;			//当前播放动画数据
			float32						m_animPlayTime;		//当前动画总的播放时间
			bool						m_stopAnim;			//是否停止动画播放
			bool						m_playLoop;			//动画是否循环播放
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

			virtual void UpdateAnimation(float32 elapsed);

			virtual void Draw();
		};
	}
}