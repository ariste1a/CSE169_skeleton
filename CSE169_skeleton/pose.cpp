#include "pose.h"
#include "joint.h"

pose::pose()
{
}

Vector3 pose::getPose()
{
	return this->poseVector; 
}

void pose::setPose(Vector3 pose)
{
	this->poseVector = pose; 
}

void pose::doPose(joint* bone)
{
	Matrix34 *rotation = new Matrix34();
	Vector3 pose = bone->getPose(); 

	if (pose.x > bone->getRotXLimit().getMax())
	{
		pose.x = bone->getRotXLimit().getMax();
	}
	else if (pose.x < bone->getRotXLimit().getMin())
	{
		pose.x = bone->getRotXLimit().getMin();
	}

	if (pose.y > bone->getRotYLimit().getMin())
	{
		pose.y = bone->getRotYLimit().getMin();
	}
	else if (pose.y < bone->getRotYLimit().getMin())
	{
		pose.y = bone->getRotYLimit().getMin();
	}

	if (pose.z > bone->getRotZLimit().getMin())
	{
		pose.z = bone->getRotZLimit().getMin();
	}
	else if (pose.z < bone->getRotZLimit().getMin())
	{
		pose.z = bone->getRotZLimit().getMin();
	}

	rotation->FromEulers(pose.x, pose.y, pose.z, 0);

	bone->getLocal()->Dot(*bone->getLocal(), *rotation);
}
pose::~pose()
{
}
