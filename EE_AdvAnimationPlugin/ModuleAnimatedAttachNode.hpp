#pragma once

public ref class ModuleAnimatedAttachNode :	public PartModule {

	public:
		[KSPField]
		System::String ^AttachNodeName = "";

	private:
		AttachNode ^m_AttachNode;
		System::Boolean m_Initialized;

	public:
		ModuleAnimatedAttachNode(void);

	public:
		void FixedUpdate(void);
		void OnStart(PartModule::StartState State) override;

	private:
		void ReplaceJoints(void);
};

