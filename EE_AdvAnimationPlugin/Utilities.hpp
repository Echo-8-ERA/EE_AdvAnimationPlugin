#pragma once

template <class T, class U>
System::Boolean IsInstanceOf(U u) {
	return dynamic_cast<T>(u) != nullptr;
}

void Assert(System::Boolean Flag, System::String ^Message);

generic <class TRet>
TRet MakeDelegate(System::Object ^Instance, System::String ^Method);

generic <class TRet>
TRet MakeDelegate(System::Type ^Type, System::String ^Method);