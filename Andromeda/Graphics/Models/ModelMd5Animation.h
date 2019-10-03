#ifndef ModelMd5Animation_H
#define ModelMd5Animation_H

#include <string>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
//#include <glm/gtx/norm.hpp>

#include <vector>

namespace Andromeda
{
	namespace Graphics
	{
		class ModelMd5Animation
		{
		private:
			int m_iMD5Version;
			int m_iNumFrames;
			int m_iNumJoints;
			int m_iFramRate;
			int m_iNumAnimatedComponents;

			float m_fAnimDuration;
			float m_fFrameDuration;
			float m_fAnimTime;

		public:

			// The JointInfo stores the information necessary to build the 
			// skeletons for each frame
			struct JointInfo
			{
				std::string m_Name;
				int         m_ParentID;
				int         m_Flags;
				int         m_StartIndex;
			};
			typedef std::vector<JointInfo> JointInfoList;

			struct Bound
			{
				glm::vec3   m_Min;
				glm::vec3   m_Max;
			};
			typedef std::vector<Bound> BoundList;

			struct BaseFrame
			{
				glm::vec3   m_Pos;
				glm::quat   m_Orient;
			};
			typedef std::vector<BaseFrame> BaseFrameList;

			struct FrameData
			{
				int m_iFrameID;
				std::vector<float> m_FrameData;
			};
			typedef std::vector<FrameData> FrameDataList;

			// A Skeleton joint is a joint of the skeleton per frame
			struct SkeletonJoint
			{
				SkeletonJoint()
					: m_Parent(-1)
					, m_Pos(0)
				{}

				SkeletonJoint(const BaseFrame& copy)
					: m_Pos(copy.m_Pos)
					, m_Orient(copy.m_Orient)
				{}

				std::string	m_Name;
				int         m_Parent;
				glm::vec3   m_Pos;
				glm::quat   m_Orient;
			};
			typedef std::vector<SkeletonJoint> SkeletonJointList;
			typedef std::vector<glm::mat4x4> SkeletonMatrixList;

			// A frame skeleton stores the joints of the skeleton for a single frame.
			struct FrameSkeleton
			{
				SkeletonMatrixList  m_BoneMatrices;
				SkeletonJointList   m_Joints;
			};
			typedef std::vector<FrameSkeleton> FrameSkeletonList;

			JointInfoList       m_JointInfos;
			BoundList           m_Bounds;
			BaseFrameList       m_BaseFrames;
			FrameDataList       m_Frames;
			FrameSkeletonList   m_Skeletons;    // All the skeletons for all the frames

			FrameSkeleton       m_AnimatedSkeleton;

			// Build the frame skeleton for a particular frame
			void BuildFrameSkeleton(FrameSkeletonList& skeletons, const JointInfoList& jointInfo, const BaseFrameList& baseFrames, const FrameData& frameData);
			void InterpolateSkeletons(FrameSkeleton& finalSkeleton, const FrameSkeleton& skeleton0, const FrameSkeleton& skeleton1, float fInterpolate);

		public:
			ModelMd5Animation();
			virtual ~ModelMd5Animation();

			// Load an animation from the animation file
			bool LoadAnimation(const std::string& filename);

			// Update this animation's joint set.
			void Update(float fDeltaTime);

			// Draw the animated skeleton
			void Render();


			const FrameSkeleton& GetSkeleton() const
			{
				return m_AnimatedSkeleton;
			}

			const SkeletonMatrixList& GetSkeletonMatrixList() const
			{
				return m_AnimatedSkeleton.m_BoneMatrices;
			}

			int GetNumJoints() const
			{
				return m_iNumJoints;
			}

			const JointInfo& GetJointInfo(unsigned int index) const
			{
				assert(index < m_JointInfos.size());
				return m_JointInfos[index];
			}


		};
	}
}

#endif