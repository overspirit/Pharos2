#pragma once

namespace Pharos
{
	namespace Scene
	{
		//////////////////////////////////////////////////////////////////////////
		/*
			desc:基本摄像机类
			*/
		//////////////////////////////////////////////////////////////////////////

		class SceneCamera //: public ICamera
		{
		public:
			SceneCamera();
			virtual ~SceneCamera();

		protected:
			Matrix4		m_mWorld;			// 摄像机的世界矩阵
			Matrix4		m_mView;              // 观察矩阵
			Matrix4		m_mProj;              // 投影矩阵

			Vector3Df	m_vEye;                 // 摄像机位置
			Vector3Df	m_vLookAt;              // 观察点位置
			Vector3Df	m_vUp;					//摄像机上向量
			float32		m_fEyeToLookAtLen;		// 摄像机和观察点的距离

			float32		m_fFOV;					// 垂直视场角
			float32		m_fAspect;              // 宽高比
			float32		m_fNearPlane;           // 近裁剪面
			float32		m_fFarPlane;            // 远裁剪面

			float32		m_fMinRadius;           // 最小半径
			float32		m_fMaxRadius;           // 最大半径

			Frustum		m_frustum;

		private:
			Vector3Df GetScreenVector(const Vector2Df& vtMove);

			void BuildFrustum(const Matrix4& view, const Matrix4& prj);

		public:
			//设定观察矩阵
			//vEyePt为世界空间里的摄像机位置，vLookAtPt为世界空间里的观察点位置
			virtual void BuildViewMatrix(const Vector3Df& vEyePt, const Vector3Df& vLookAtPt, const Vector3Df& vUp = Vector3Df(0, 1, 0));

			//设定投影矩阵
			//fFov为观察角，fAspect为投影长宽比值，fNearPlane为近平面，fFarPlane为远平面
			virtual void BuildProjMatrix(float32 fFOV, uint32 nWidth, uint32 nHeight, float32 fNearPlane, float32 fFarPlane);

			//摄像机平移，摄像机将根据自身坐标系X,Y,Z轴移动
			virtual void Move(const Vector3Df& vtMove);

			//以自身为中心点旋转，vtMove为屏幕移动距离,
			//摄像机将把vtMove映射到三维空间中，产生一个由
			//(0, 0, 1)到这个映射之后的向量的旋转
			virtual void Slither(const Vector2Df& vtMove);

			//绕观察坐标系的Z轴旋转, fRad为旋转角度，弧度制，取值0至PI * 2
			virtual void Turn(float32 fRad);

			//以观察点为中心点旋转，vtMove为屏幕移动距离,
			//摄像机将把vtMove映射到三维空间中，产生一个由
			//(0, 0, 1)到这个映射之后的向量的旋转
			virtual void Round(const Vector2Df& vtMove);

			//拉近或拉远，即改变摄像机和观察点的距离
			virtual void Stretch(float32 fMove);

			virtual void SetPosition(const Vector3Df& vPos);			//设置摄像机位置	
			virtual const Vector3Df& GetPosition() const { return m_vEye; }		//获取摄像机位置	
			virtual void SetLookAtPt(const Vector3Df& vTarget);			//设置观察点
			virtual const Vector3Df& GetLookAtPt() const { return m_vLookAt; }	//获取观察点

			virtual Vector2Df WorldToScreen(const Vector3Df& vtWorld, const Size2Di& wndSize);			//将三维空间的点映射到屏幕相对坐标系
			virtual Vector3Df WorldToScreenF(const Vector3Df& vtWorld);			//将三维空间的点映射到屏幕绝对坐标系
			virtual Vector3Df ScreenToWorld(int32 screenX, int32 screenY, const Size2Di& wndSize);		//将屏幕上的点映射到三维空间
			virtual Vector2Df ScreenFToScreen(const Vector3Df& vtScreen, const Size2Di& wndSize);		//将屏幕的相对坐标系点转换到绝对坐标系
			virtual Line3Df GetMouseRay(int32 cursorX, int32 cursorY, const Size2Di& wndSize);			//产生一个由摄像机指向指定点的射线
			virtual const Line3Df& GetCameraRay() const { return *(Line3Df*)&m_vEye; }	//获得摄像机位置到观察点的射线

			//得到摄像机的各个信息
			const Vector3Df& GetWorldRight() const { return *(Vector3Df*)&m_mWorld.m[0]; }	//摄像机右向量
			const Vector3Df& GetWorldUp() const { return *(Vector3Df*)&m_mWorld.m[4]; }		//获取摄像机上向量	
			const Vector3Df& GetWorldAhead() const { return *(Vector3Df*)&m_mWorld.m[8]; }	//获取摄像机的前向量
			const Matrix4& GetWorldMatrix() const { return m_mWorld; }						//摄像机的世界矩阵
			const Matrix4& GetViewMatrix() const { return m_mView; }						//摄像机的观察矩阵
			const Matrix4& GetProjMatrix() const { return m_mProj; }						//摄像机的投影矩阵
			const Frustum& GetViewFrustum() const { return m_frustum; }						//获取摄像机的观察平截头体

			virtual float32 GetAspectRatio() const { return m_fAspect; }	//摄像机位置
			virtual float32 GetFieldOfView() const { return m_fFOV; }		//获取垂直视场角
			virtual float32 GetNearClip() const { return m_fNearPlane; }	//近平面
			virtual float32 GetFarClip() const { return m_fFarPlane; }		//远平面	
		};
	}
}