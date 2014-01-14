#pragma once

[KSPAddon(KSPAddon::Startup::Flight, false)]
ref class Events : public UnityEngine::MonoBehaviour {

	public:
		static Events ^ActiveRef;

	public:
		Events(void);

		void Start(void);
		void OnDestroy(void);

	private:
		void OnVesselWasModified(Vessel ^v);
		void OnVesselOffRails(Vessel ^v);

	public:
		ref class VesselEventArgs : System::EventArgs {
			private:
				::Vessel ^m_Vessel;
			public:
				VesselEventArgs(::Vessel ^Vessel);

				property ::Vessel ^Vessel {
					::Vessel ^get(void);
				}

		};

		event System::EventHandler<VesselEventArgs ^> ^VesselWasModified;
		event System::EventHandler<VesselEventArgs ^> ^VesselWasOffRails;

};

