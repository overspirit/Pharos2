#pragma once

namespace Pharos
{
	namespace Scene
	{
		class OctreeScene;

		class TreeNode
		{
		public:
			TreeNode(void);
			virtual ~TreeNode(void);

			friend class OctreeScene;

		private:
			OctreeScene*	m_pScene;

			TreeNode*		m_pParent;	//父节点
			int32			m_nIndex;	//本节点在父节点中的索引
			TreeNode*		m_children[8];	//子节点	
			Vector3Df		m_basePos[8];	//基本点
			//基本点的意义:
			//本节点的内接正方形，平均分成8份，基本点就是这8个
			//子正方形的中心点，每个子节点的中心点都是按照
			//其对应的基本点做的偏移，基本点的意义在于保证子节点
			//的不至于偏移出父节点

			//优化计算的标志位，总共8位，如果第n个子节点如果有效，n位就为1,
			//如果m_dwComputeFlag 等于 0xFF 意味着8个子节点都存在
			uint32			m_dwComputeFlag;

			Vector3Df		m_vtCenter;
			float32			m_fRadius;

			list<SceneNode*>	m_nodeList;	//管理的SceneNode列表

			int32			m_nDepth;	//本节点深度

			bool			m_bCulling;		//是否被视锥裁剪掉

		private:
			bool IsInSphere(const Vector3Df& vtCenter, float32 fRadius);

			void RemoveChild(int32 nIndex);

			//计算基本点
			void ComputeBasePos();

			//重新计算
			void Recompute(const Vector3Df& vtCenter, float32 fRadius);

		public:
			void Init(OctreeScene* pScene, TreeNode* pParent, int32 nIndex);

			void Update(float32 fElapsed);

			void FrustumCulling(const Frustum& frustum);
			bool IsCulling(){ return m_bCulling; }			

			void InsertItem(SceneNode* pNode);	//将一个节点插入到树节点中，

			void RemoveOwnItem(SceneNode* pNode);
			void RefreshOwnItem(SceneNode* pNode);

			bool IsVaild();

			void SetCenter(const Vector3Df& vtCenter){ m_vtCenter = vtCenter; }
			void SetRadius(float32 fRadius){ m_fRadius = fRadius; }

			const Vector3Df& GetCenter(){ return m_vtCenter; }
			float32 GetRadius(){ return m_fRadius; }
			int32 GetDepth(){ return m_nDepth; }
		};
	}
}
