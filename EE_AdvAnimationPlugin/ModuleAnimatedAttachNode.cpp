#include "stdafx.h"
#include "Utilities.hpp"
#include "ModuleAnimatedAttachNode.hpp"

using System::String;

ModuleAnimatedAttachNode::ModuleAnimatedAttachNode() : PartModule() {
}

void ModuleAnimatedAttachNode::OnStart(PartModule::StartState State) {
	
	PartModule::OnStart(State);

	m_Initialized = false;
	m_AttachNode = this->part->findAttachNode(this->AttachNodeName);

}

void ModuleAnimatedAttachNode::FixedUpdate(void) {

	if ((!HighLogic::LoadedSceneIsFlight) || (this->vessel == nullptr) || (this->vessel->packed)) {

		return;

	}

	if (!m_Initialized) {
		m_Initialized = true;
		PDebug::Log("AttachNode is connected to: " + m_AttachNode->attachedPart->ConstructID); 
		ReplaceJoints();
	}

	for each (auto anim in this->part->FindModelAnimators()) {

		if (anim->isPlaying) {

			if (this->vessel != nullptr) {

				// Update child parts
				auto pRoot = this->vessel->rootPart;

				this->part->UpdateOrgPosAndRot(pRoot);

				for each (auto pChild in this->part->FindChildParts<Part ^>(true)) {

					pChild->UpdateOrgPosAndRot(pRoot);

				}

				break;

			}

		}

	}

}

void ModuleAnimatedAttachNode::ReplaceJoints(void) {

	// Check if there are any animators associated with the part. No point going about replacing joints if it's not
	// going to be animated.
	if (this->part->FindModelAnimators()->Length == 0) {

		return;

	}

	// Find the associated attach node
	auto AttNode = this->part->findAttachNode(this->AttachNodeName);

	// Are we working on the parent part?
	if (static_cast<System::Object ^>(AttNode->attachedPart->parent) == this->part) {

		// Yes we are. Now we need to change it so the joint's connected body is an invisible Rigidbody
		// located at the attach node's transform instead of our part's Rigidbody.

		// Add a kinematic Rigidbody to the transform
		auto RB = AttNode->nodeTransform->gameObject->AddComponent<UnityEngine::Rigidbody ^>();
		RB->isKinematic = true;

		// Change the connected body
		AttNode->attachedPart->attachJoint->connectedBody = RB;

	} else {

		// No we aren't. Now we need to recreate the joint.
		auto OldJoint = this->part->attachJoint;
		if (IsInstanceOf<UnityEngine::HingeJoint ^, UnityEngine::Joint ^>(OldJoint)) {

			auto OldJointH = dynamic_cast<UnityEngine::HingeJoint ^>(OldJoint);
			auto NewJoint = AttNode->nodeTransform->gameObject->AddComponent<UnityEngine::HingeJoint ^>();
			NewJoint->axis = OldJointH->axis;
			NewJoint->limits = OldJointH->limits;
			NewJoint->useLimits = OldJointH->useLimits;
			NewJoint->connectedBody = OldJointH->connectedBody;
			this->part->attachJoint = NewJoint;

		} else if (IsInstanceOf<UnityEngine::FixedJoint ^>(OldJoint)) {

			auto OldJointF = dynamic_cast<UnityEngine::FixedJoint ^>(OldJoint);
			auto NewJoint = AttNode->nodeTransform->gameObject->AddComponent<UnityEngine::FixedJoint ^>();
			NewJoint->connectedBody = OldJoint->connectedBody;
			this->part->attachJoint = NewJoint;

		} else {

			// It's an unknown joint. Report to the user and ignore it
			PDebug::Warning(String::Format(
					"[ModuleAnimatedAttachNode] AttachNode \"{0}\" on Part \"{0}\" has an unknown joint type", m_AttachNode->id,
					this->part->ConstructID));
			goto AFTER_DESTROY;

		}

		UnityEngine::Object::Destroy(OldJoint);

AFTER_DESTROY:
		void(0);
	}
}