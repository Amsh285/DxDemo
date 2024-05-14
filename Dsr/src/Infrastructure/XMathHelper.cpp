#include "dsrpch.h"
#include "XMathHelper.h"

float dsr::Vector3Determinant(const DirectX::XMVECTOR& v0, const DirectX::XMVECTOR& v1, const DirectX::XMVECTOR& v2)
{
	using namespace DirectX;

	XMMATRIX m;
	m.r[0] = v0;
	m.r[1] = v1;
	m.r[2] = v2;

	return XMVectorGetX(XMMatrixDeterminant(m));
}
