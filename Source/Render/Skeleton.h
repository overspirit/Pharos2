#pragma once

namespace Pharos
{
	namespace Render
	{
		struct BoneInfo
		{
			string				name;
			int32				id;
			int32				parentId;			
			Matrix4				bindPose;
			vector<BoneInfo>	children;
		};

		class Skeleton
		{
		public:
			Skeleton();
			virtual ~Skeleton();

		private:

		private:
		};
	}
}