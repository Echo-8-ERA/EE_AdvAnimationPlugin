#include "stdafx.h"
#include "Events.hpp"
#include "Utilities.hpp"

using namespace System;

Events::VesselEventArgs::VesselEventArgs(::Vessel ^v) : m_Vessel(v) {

}

::Vessel ^Events::VesselEventArgs::Vessel::get(void) {

	return m_Vessel;

}