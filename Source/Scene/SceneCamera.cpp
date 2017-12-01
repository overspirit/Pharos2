#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "SceneGlobal.h"

SceneCamera::SceneCamera()
{
	m_vEye = Vector3Df(0, 0, 0);                 // 摄像机位置
	m_vLookAt= Vector3Df(0, 0, 1.0f);              // 观察点位置
	m_fEyeToLookAtLen = 1.0f;		// 摄像机和观察点的距离

	m_fFOV = 0;                // 垂直视场角
	m_fAspect = 0;              // 宽高比
	m_fNearPlane = 0;           // 近裁剪面
	m_fFarPlane = 0;            // 远裁剪面

	m_fMinRadius = 1.0f;
	m_fMaxRadius = FLT_MAX;
}

SceneCamera::~SceneCamera()
{

}

void SceneCamera::BuildViewMatrix(const Vector3Df& vEyePt, const Vector3Df& vLookAtPt, const Vector3Df& vUp)
{
	m_vEye = vEyePt;
	m_vLookAt = vLookAtPt;
	m_vUp = vUp;

	// 计算观察点和摄像机的距离
	m_fEyeToLookAtLen = m_vLookAt.GetDistanceFrom(m_vEye);

	// 得到观察矩阵
	m_mView.BuildCameraLookAtMatrixLH(vEyePt, vLookAtPt, vUp);

	//得到摄像机的世界矩阵，摄像机的世界矩阵为观察矩阵的逆矩阵
	m_mView.GetInverse(m_mWorld);

	BuildFrustum(m_mView, m_mProj);
}

void SceneCamera::BuildProjMatrix( float32 fFOV, uint32 nWidth, uint32 nHeight, float32 fNearPlane, float32 fFarPlane )
{
	if(nWidth == 0 && nHeight == 0) return;
	
	m_fAspect = (float32)nWidth/nHeight;
	m_fFOV = fFOV;	
	//m_fAOMV = atan(tan(fFOV / 2.0f) * squareroot(1 + m_fAspect * m_fAspect)) * 2.0f;
	m_fNearPlane = fNearPlane;
	m_fFarPlane = fFarPlane;

	//计算投影矩阵
	m_mProj.BuildProjectionMatrixPerspectiveFovLH(fFOV, m_fAspect, fNearPlane, fFarPlane);

	BuildFrustum(m_mView, m_mProj);
}

void SceneCamera::Move(const Vector3Df& vtMove)
{
	m_vEye = m_mWorld.TransformCoord(vtMove);

 	Vector3Df lookatMove = vtMove;
	lookatMove.z += m_fEyeToLookAtLen;
	m_vLookAt = m_mWorld.TransformCoord(lookatMove);

	const Vector3Df& xaxis = this->GetWorldRight();
	const Vector3Df& yaxis = this->GetWorldUp();
	const Vector3Df& zaxis = this->GetWorldAhead();

	m_mView.m[12] = -xaxis.DotProduct(m_vEye);
	m_mView.m[13] = -yaxis.DotProduct(m_vEye);
	m_mView.m[14] = -zaxis.DotProduct(m_vEye);

	//直接得到平移矩阵
// 	Matrix4 mCameraTrans;
// 	mCameraTrans.SetTranslation(vtMove);
// 
// 	//平移
// 	Matrix4 mWorld = mCameraTrans * m_mWorld;
// 
// 	//转换摄像机位置	
// 	mWorld.TransformCoord(Vector3Df(0, 0, 0), m_vEye);
// 
// 	//计算观察点的平移矩阵
// 	Matrix4 mLookAtTrans;
// 	mLookAtTrans.SetTranslation(Vector3Df(0, 0, m_fEyeToLookAtLen));
// 
// 	//平移
// 	mWorld = mLookAtTrans * mWorld;
// 
// 	//转换观察点
// 	mWorld.TransformCoord(Vector3Df(0, 0, 0), m_vLookAt);
// 
// 	//建立观察矩阵
// 	m_mView.BuildCameraLookAtMatrixLH(m_vEye, m_vLookAt, m_vUp);

// 	//得到摄像机的世界矩阵，摄像机的世界矩阵为观察矩阵的逆矩阵
 	m_mView.GetInverse(m_mWorld);	

	BuildFrustum(m_mView, m_mProj);
}

void SceneCamera::Slither(const Vector2Df& vtMove)
{	
	//通过轨迹球得到旋转矩阵
	Matrix4 mCameraRotation;
	mCameraRotation.BuildRotateFromTo(Vector3Df(0, 0, 1.0f), GetScreenVector(vtMove));

	//平移到观察点
	Matrix4 mLookAtTrans;
	mLookAtTrans.SetTranslation(Vector3Df(0, 0, m_fEyeToLookAtLen));

	//计算观察点的世界矩阵
	Matrix4 mWorld = mLookAtTrans * mCameraRotation * m_mWorld;	

	//从原点转换观察点	
	m_vLookAt = mWorld.TransformCoord(Vector3Df(0, 0, 0));

	//建立观察矩阵
	m_mView.BuildCameraLookAtMatrixLH(m_vEye, m_vLookAt, m_vUp);

	//得到摄像机的世界矩阵，摄像机的世界矩阵为观察矩阵的逆矩阵
	m_mView.GetInverse(m_mWorld);

	BuildFrustum(m_mView, m_mProj);
}

//绕观察坐标系的Z轴旋转, fRad为旋转角度，弧度制，取值0至PI * 2
void SceneCamera::Turn(float32 fRad)
{
	Vector3Df newUp(tan(fRad), 1.0f, 0);
	newUp.Normalize();

	Matrix4 mCameraRotation;
	mCameraRotation.BuildRotateFromTo(Vector3Df(0, 1.0f, 0), newUp);	
	m_vUp = mCameraRotation.TransformCoord(m_vUp);

	//建立观察矩阵
	m_mView.BuildCameraLookAtMatrixLH(m_vEye, m_vLookAt, m_vUp);

	//得到摄像机的世界矩阵，摄像机的世界矩阵为观察矩阵的逆矩阵
	m_mView.GetInverse(m_mWorld);

	BuildFrustum(m_mView, m_mProj);
}

void SceneCamera::Round(const Vector2Df& vtMove)
{
	//从轨迹球得到旋转矩阵
	Matrix4 mCameraRotation;
	mCameraRotation.BuildRotateFromTo(Vector3Df(0, 0, 1.0f), GetScreenVector(vtMove));

	//计算一个平移到摄像机点的平移矩阵，
	Matrix4 mCameraTrans;
	mCameraTrans.SetTranslation(Vector3Df(0, 0, -m_fEyeToLookAtLen));

	//首先计算一个平移到观察点的平移矩阵
	Matrix4 mLookAtTrans;
	mLookAtTrans.SetTranslation(Vector3Df(0, 0, m_fEyeToLookAtLen));

	//组合矩阵，
	Matrix4 mWorld = mCameraTrans * mCameraRotation * mLookAtTrans * m_mWorld;

	//变换摄像机位置	
	m_vEye = mWorld.TransformCoord(Vector3Df(0, 0, 0));

	// 得到观察矩阵
	m_mView.BuildCameraLookAtMatrixLH(m_vEye, m_vLookAt, m_vUp);

	//得到摄像机的世界矩阵，摄像机的世界矩阵为观察矩阵的逆矩阵
	m_mView.GetInverse(m_mWorld);	

	BuildFrustum(m_mView, m_mProj);
}

void SceneCamera::Stretch(float32 fMove)
{
	//计算移动距离和新的摄像机位置和观察点距离
	m_fEyeToLookAtLen += fMove;

	//重新计算摄像机位置
	m_vEye += ((m_vEye - m_vLookAt).Normalize() * fMove);

	// 得到观察矩阵
	m_mView.BuildCameraLookAtMatrixLH(m_vEye, m_vLookAt, m_vUp);

	//求摄像机的世界矩阵
 	m_mView.GetInverse(m_mWorld);

	BuildFrustum(m_mView, m_mProj);
}

void SceneCamera::SetPosition(const Vector3Df& vPos)
{
	BuildViewMatrix(vPos, m_vLookAt, m_vUp);
}

void SceneCamera::SetLookAtPt(const Vector3Df& vTarget)
{
	BuildViewMatrix(m_vEye, vTarget, m_vUp);
}

Vector2Df SceneCamera::WorldToScreen(const Vector3Df& vtWorld)
{
	//将给出的点依次进行观察变换和投影变换就得到屏幕点
// 	Vector3Df trans;
// 	m_mView.TransformCoord(vtWorld, trans);
// 	m_mProj.TransformCoord(trans);
// 
// 	const Size2Di& wndSize = g_pKernel->GetWindowSize();
// 	int32 nWidth = wndSize.width;
// 	int32 nHeight = wndSize.height;

	//这时的点是屏幕的相对坐标点，需要转换成绝对坐标点
	Vector2Df ret;
// 	ret.x = (int32)((1.0f + trans.x) * 0.5f * nWidth);
// 	ret.y = (int32)((1.0f - trans.y) * 0.5f * nHeight);
	return ret;
}

Vector3Df SceneCamera::WorldToScreenF(const Vector3Df& vtWorld)
{
	//将给出的点依次进行观察变换和投影变换就得到屏幕点
	Vector3Df trans;
// 	m_mView.TransformCoord(vtWorld, trans);
// 	m_mProj.TransformCoord(trans);

	return trans;
}

Vector3Df SceneCamera::ScreenToWorld(int32 screenX, int32 screenY)
{
	//求投影平面的半长和半宽
// 	float32 halfW = tanf(m_fFOV * 0.5f) * m_fNearPlane * m_fAspect;
// 	float32 halfH = halfW / m_fAspect;
// 
// 	//获得窗口大小
// 	const Size2Di& wndSize = g_pKernel->GetWindowSize();
// 	int32 nWidth = wndSize.width;
// 	int32 nHeight = wndSize.height;

	//这个点在近裁剪面上，求这个点的位置，这时计算的位置是观察坐标系的
	//所以需要转换到世界坐标系中
	Vector3Df r;
// 	r.x = (screenX / (nWidth * 0.5f) - 1.0f) * halfW;
// 	r.y = (1.0f - screenY / (nHeight * 0.5f)) * halfH;
// 	r.z = m_fNearPlane;
// 
// 	m_mWorld.TransformCoord(r);

	return r;
}

Vector2Df SceneCamera::ScreenFToScreen(const Vector3Df& vtScreen)
{
// 	const Size2Di& wndSize = g_pKernel->GetWindowSize();
// 	int32 nWidth = wndSize.width;
// 	int32 nHeight = wndSize.height;

	//将屏幕相对坐标点转换成绝对坐标点
	Vector2Df ret;
// 	ret.x = (int32)((1.0f + vtScreen.x) * 0.5f * nWidth);
// 	ret.y = (int32)((1.0f - vtScreen.y) * 0.5f * nHeight);
	return ret;
}

Line3Df SceneCamera::GetMouseRay(int32 cursorX, int32 cursorY)
{
	Line3Df out;
// 	out.start = m_vEye;
// 	out.end = ScreenToWorld(cursorX, cursorY);
	return out;
}

Vector3Df SceneCamera::GetScreenVector(const Vector2Df& vtMove)
{
// 	const Size2Di& wndSize = g_pKernel->GetWindowSize();
// 	int32 nHalfWidth = (int32)(wndSize.width / 2);
// 	int32 nHalfHeight = (int32)(wndSize.height / 2);

	int32 nHalfWidth = 0;
	int32 nHalfHeight = 0;

	//求映射向量,首先将屏幕上的点映射为0-1之间的数值，然后勾股定理计算另一条边	
	Vector3Df vTo = {0, 0, 1.0f};
	float32 x = vtMove.x / nHalfWidth;
	float32 y = -vtMove.y / nHalfHeight;
	float32 x2 = x * x;
	float32 y2 = y * y;
	if (x2 + y2 <= 1.0f) //需要检测是否不能开方，传过来的数据可能大于窗口大小
	{
		vTo.x = x;
		vTo.y = y;
		vTo.z = sqrtf(1.0f - x2 - y2);
		vTo.Normalize();
	}
	return vTo;
}

void SceneCamera::BuildFrustum(const Matrix4& view, const Matrix4& prj)
{
	Matrix4 comboMatrix = view * prj;

	// Left clipping plane
	m_frustum.m_Planes[0].normal.x = comboMatrix.m[3] + comboMatrix.m[0];
	m_frustum.m_Planes[0].normal.y = comboMatrix.m[7] + comboMatrix.m[4];
	m_frustum.m_Planes[0].normal.z = comboMatrix.m[11] + comboMatrix.m[8];
	m_frustum.m_Planes[0].d = comboMatrix.m[15] + comboMatrix.m[12];
	m_frustum.m_Planes[0].Normalize();

	// Right clipping plane
	m_frustum.m_Planes[1].normal.x = comboMatrix.m[3] - comboMatrix.m[0];
	m_frustum.m_Planes[1].normal.y = comboMatrix.m[7] - comboMatrix.m[4];
	m_frustum.m_Planes[1].normal.z = comboMatrix.m[11] - comboMatrix.m[8];
	m_frustum.m_Planes[1].d = comboMatrix.m[15] - comboMatrix.m[12];
	m_frustum.m_Planes[1].Normalize();

	// Top clipping plane
	m_frustum.m_Planes[2].normal.x = comboMatrix.m[3] - comboMatrix.m[1];
	m_frustum.m_Planes[2].normal.y = comboMatrix.m[7] - comboMatrix.m[5];
	m_frustum.m_Planes[2].normal.z = comboMatrix.m[11] - comboMatrix.m[9];
	m_frustum.m_Planes[2].d = comboMatrix.m[15] - comboMatrix.m[13];
	m_frustum.m_Planes[2].Normalize();

	// Bottom clipping plane
	m_frustum.m_Planes[3].normal.x = comboMatrix.m[3] + comboMatrix.m[1];
	m_frustum.m_Planes[3].normal.y = comboMatrix.m[7] + comboMatrix.m[5];
	m_frustum.m_Planes[3].normal.z = comboMatrix.m[11] + comboMatrix.m[9];
	m_frustum.m_Planes[3].d = comboMatrix.m[15] + comboMatrix.m[13];
	m_frustum.m_Planes[3].Normalize();

	// Near clipping plane
	m_frustum.m_Planes[4].normal.x = comboMatrix.m[3] + comboMatrix.m[2];
	m_frustum.m_Planes[4].normal.y = comboMatrix.m[7] + comboMatrix.m[6];
	m_frustum.m_Planes[4].normal.z = comboMatrix.m[11] + comboMatrix.m[10];
	m_frustum.m_Planes[4].d = comboMatrix.m[15] + comboMatrix.m[14];
	m_frustum.m_Planes[4].Normalize();

	// Far clipping plane
	m_frustum.m_Planes[5].normal.x = comboMatrix.m[3] - comboMatrix.m[2];
	m_frustum.m_Planes[5].normal.y = comboMatrix.m[7] - comboMatrix.m[6];
	m_frustum.m_Planes[5].normal.z = comboMatrix.m[11] - comboMatrix.m[10];
	m_frustum.m_Planes[5].d = comboMatrix.m[15] - comboMatrix.m[14];
	m_frustum.m_Planes[5].Normalize();
}

