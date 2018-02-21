#ifndef INCLUDED_ENGINE_RTTI_H
#define INCLUDED_ENGINE_RTTI_H

#define REGISTER_TYPE(TypeId)\
	static int StaticGetType() { return #@TypeId; }\
	virtual int GetType() const { return StaticGetType(); }

#endif