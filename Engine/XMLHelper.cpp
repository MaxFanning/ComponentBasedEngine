#include "Precompiled.h"
#include "XMLHelper.h"

f32 XMLHelper::ReadFloat(const TiXmlNode* node)
{
	return (f32)atof(node->FirstChild()->Value());
}

Math::Vector3 XMLHelper::ReadVector(const TiXmlNode* vectorNode)
{
	const TiXmlNode* xNode = vectorNode->FirstChild();
	const TiXmlNode* yNode = vectorNode->FirstChild()->NextSibling();
	const TiXmlNode* zNode = vectorNode->FirstChild()->NextSibling()->NextSibling();
	f32 x = ReadFloat(xNode);
	f32 y = ReadFloat(yNode);
	f32 z = ReadFloat(zNode);
	return Math::Vector3(x, y, z);
}

Color XMLHelper::ReadColor(const TiXmlNode* colorNode)
{
	const TiXmlNode* rNode = colorNode->FirstChild();
	const TiXmlNode* gNode = colorNode->FirstChild()->NextSibling();
	const TiXmlNode* bNode = colorNode->FirstChild()->NextSibling()->NextSibling();
	const TiXmlNode* aNode = colorNode->FirstChild()->NextSibling()->NextSibling()->NextSibling();
	f32 r = ReadFloat(rNode);
	f32 g = ReadFloat(gNode);
	f32 b = ReadFloat(bNode);
	f32 a = ReadFloat(aNode);
	return Color(r, g, b, a);
}