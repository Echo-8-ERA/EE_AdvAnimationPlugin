#include "stdafx.h"
#include "Events.hpp"
#include "Utilities.hpp"

using namespace System;

Events::Events() {
}

void Events::Start(void) {
	
	// Seriously, why the heck do I have to resort to reflection to get this to work
	// I mean, why the heck do I get:
	//		error C2664: 'void EventData<Vessel ^>::Add(EventData<Vessel ^>::OnEvent ^)' : cannot convert argument 1 from 'EventData<Vessel ^>::OnEvent ^' to 'EventData<Vessel ^>::OnEvent ^'
	// If I uncomment the following:
	//		GameEvents::onVesselWasModified->Add(gcnew EventData<Vessel^>::OnEvent(this, &Events::OnVesselWasModified));

	auto GEVED_Add = EventData<Vessel^>::typeid->GetMethod("Add");
	GEVED_Add->Invoke(GameEvents::onVesselWasModified, gcnew array<System::Object ^>{ MakeDelegate<EventData<Vessel ^>::OnEvent ^>(this, "OnVesselWasModified") });
	GEVED_Add->Invoke(GameEvents::onVesselGoOffRails, gcnew array<System::Object ^>{ MakeDelegate<EventData<Vessel ^>::OnEvent ^>(this, "OnVesselOffRails") });
}

void Events::OnDestroy(void) {
	
	auto GEVED_Remove = EventData<Vessel^>::typeid->GetMethod("Remove");
	GEVED_Remove->Invoke(GameEvents::onVesselWasModified, gcnew array<System::Object ^>{ MakeDelegate<EventData<Vessel ^>::OnEvent ^>(this, "OnVesselWasModified") });
	GEVED_Remove->Invoke(GameEvents::onVesselGoOffRails, gcnew array<System::Object ^>{ MakeDelegate<EventData<Vessel ^>::OnEvent ^>(this, "OnVesselOffRails") });

}

void Events::OnVesselWasModified(Vessel ^v) {

	this->VesselWasModified(this, gcnew VesselEventArgs(v));

}

void Events::OnVesselOffRails(Vessel ^v) {

	this->VesselWasOffRails(this, gcnew VesselEventArgs(v));

}