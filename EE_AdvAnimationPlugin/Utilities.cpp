#include "stdafx.h"
#include "Utilities.hpp"

using namespace System;

void Assert(Boolean Flag, String ^Message) {
	if (!Flag) {
		PDebug::Log(Message);
	}
}

generic <class TRet>
TRet MakeDelegate(Object ^Instance, String ^Method) {

	return safe_cast<TRet>(Delegate::CreateDelegate(TRet::typeid, Instance, Method));

}

generic <class TRet>
TRet MakeDelegate(Type ^Type, String ^Method) {

	return safe_cast<TRet>(Delegate::CreateDelegate(TRet::typeid, Type, Method));

}