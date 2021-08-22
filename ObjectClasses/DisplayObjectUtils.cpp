#include "DisplayObjectUtils.h"

const DirectX::XMVECTOR DAUtils::GetRotation(const DisplayObject & object)
{
	return DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(DirectX::XMConvertToRadians(object.m_orientation.y),
		DirectX::XMConvertToRadians(object.m_orientation.x),
		DirectX::XMConvertToRadians(object.m_orientation.z));
}

const DirectX::XMVECTORF32 DAUtils::GetScale(const DisplayObject & object)
{
	return  DirectX::XMVECTORF32{ object.m_scale.x, object.m_scale.y, object.m_scale.z };
}

const DirectX::XMVECTORF32 DAUtils::GetTranslation(const DisplayObject & object)
{
	return DirectX::XMVECTORF32{ object.m_position.x, object.m_position.y, object.m_position.z };
}

const DirectX::XMMATRIX DAUtils::GetLocalMatrix(DirectX::SimpleMath::Matrix world, const DisplayObject& object)
{
	return world * DirectX::XMMatrixTransformation(DirectX::XMVECTORF32{ { { 0.0f, 0.0f, 0.0f, 0.0f } } }, DirectX::SimpleMath::Quaternion::Identity, GetScale(object),
		DirectX::XMVECTORF32{ { { 0.0f, 0.0f, 0.0f, 0.0f } } }, GetRotation(object), GetTranslation(object));
}
