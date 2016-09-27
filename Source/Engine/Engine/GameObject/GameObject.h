#pragma once
#include "Engine\Component\Pointer\SharedPtrComponent.h"

class Scene;
class BaseComponent;

struct GameObjectData
{
	std::string myID;
	Quaternion myRotation;
	Vector3f myPosition;
	std::shared_ptr<Scene> myScene;
};

class GameObject
{
public:
	GameObject();
	GameObject(const GameObjectData& aData);
	~GameObject();

	Scene & GetScene();
	const Scene & GetScene() const;

	template <typename TStateType>
	TStateType & GetState();

	template <typename TStateType>
	const TStateType & GetState() const;


	template <typename TComponentType>
	void AddComponent(const SharedPtrComponent<TComponentType> & aComponent);

	template <typename TComponentType>
	SharedPtrComponent<TComponentType> GetComponent();

	template <typename TComponentType>
	const SharedPtrComponent<TComponentType> GetComponent()const;

	//ComponentFactory<std::shared_ptr<ModelComponent>> myModelComponentFactory;
	GrowingArray<SharedPtrComponent<BaseComponent>, size_t> myComponents;

	inline const Vector3f& GetPosition() const
	{
		return myPosition;
	}
	inline const Quaternion& GetRotation() const
	{
		return myRotation;
	}

	void SetData(const GameObjectData& aData);

private:
	std::string myID;
	Quaternion myRotation;
	Vector3f myPosition;
	std::shared_ptr<Scene> myScene;
};

inline Scene & GameObject::GetScene()
{
	return *myScene;
}

inline const Scene & GameObject::GetScene() const
{
	return *myScene;
}

template <typename TStateType>
TStateType & GameObject::GetState()
{

}

template <typename TStateType>
const TStateType & GameObject::GetState() const
{

}

template <typename TComponentType>
void GameObject::AddComponent(const SharedPtrComponent<TComponentType> & aComponent)
{
	size_t id = UniqeIdentifier<BaseComponent>::GetID<TComponentType>();
	size_t nextID = UniqeIdentifier<BaseComponent>::nextTypeIndex;
	if (myComponents.Size() < nextID)
	{
		myComponents.Resize(nextID);
	}
	myComponents[id] = SharedPtrComponent<BaseComponent>::CastFrom(aComponent);
}

template <typename TComponentType>
SharedPtrComponent<TComponentType>
GameObject::GetComponent()
{
	size_t id = UniqeIdentifier<BaseComponent>::GetID<TComponentType>();
	size_t nextID = UniqeIdentifier<BaseComponent>::nextTypeIndex;
	if (myComponents.Size() < nextID)
	{
		AddComponent<TComponentType>();
	}
	return SharedPtrComponent<TComponentType>::CastFrom(myComponents[id]);
}

template <typename TComponentType>
const SharedPtrComponent<TComponentType>
GameObject::GetComponent() const
{
	size_t id = UniqeIdentifier<BaseComponent>::GetID<TComponentType>();
	return SharedPtrComponent<TComponentType>::CastFrom(myComponents[id]);
}
