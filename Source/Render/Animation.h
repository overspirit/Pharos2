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

		class Animation
		{
		public:
			Animation();
			virtual ~Animation();

		private:
		};
	}
}