#pragma once
#include "Engine\LightConstantBufferData.h"

class GenericMesh;
template<typename T>
class VertexBuffer;
class ModelInstance;

template <typename T>
class ConstantBuffer;

class Effect;
class Model;
class MultiRenderTexture;

class BatchEntry
{
public:
	BatchEntry(const std::shared_ptr<const GenericMesh> & aMesh);
	void Schedule();
	bool IsScheduled();

	void AddInstance(const Matrix44f & aMatrix);
	const Matrix44f * GetInstances() const;
	int GetInstanceCount() const;
	void FinishedRendering();

	const GenericMesh & GetMesh() const;

private:
	std::weak_ptr<const GenericMesh> myMesh;
	std::shared_ptr<const GenericMesh> myMeshScheduleLock;
	std::vector<Matrix44f> myMatrices;
	int myMatrixCounter;
};

class ModelRenderer
{
public:
	ModelRenderer();
	~ModelRenderer();

	void InstantRender(const std::shared_ptr<GenericMesh> & myMeshes);
	void Render(const std::shared_ptr<GenericMesh> & aMesh, const Matrix44f & aMatrix);
	void RenderBuffer();

	void PrepareInstantRender(const Matrix44f & aWorldMatrix);

	void Schedule(BatchEntry & aBatch);

	size_t GenerateBatchIdentifier(const std::shared_ptr<GenericMesh> & aMesh);
	void ReturnBatchIdentifier(size_t aBatchIdentifier);

	void SetDirectionalLight(int aIndex, const Vector3f & aLightDirection, const Vector4f & aLightColor);

	void SetPointLight(int aIndex, const Vector3f & aLightPosition, const Vector3f & aLightColor, const float aLightRadius, const float aLightIntensity);

	const LightConstantBufferData & GetLightData() const;

	const std::shared_ptr<MultiRenderTexture> & GetDeferredTexture() const;
	const std::shared_ptr<RenderTexture> & GetLambertTexture() const;

	void RenderLights();

private:
	std::vector<BatchEntry*> myCurrentlyScheduledBatches;
	std::vector<std::unique_ptr<BatchEntry>> myMeshes;
	std::stack<size_t> myReturnedBatchIdentifiers;
	std::vector<std::shared_ptr<VertexBuffer<Matrix44f>>> myVertexBuffers;

	std::shared_ptr<ConstantBuffer<LightConstantBufferData>> myLightingBuffer;
	LightConstantBufferData myLightingData;

	std::shared_ptr<ConstantBuffer<PointLight>> myPointLightBuffer;

	std::shared_ptr<Effect> myEffect;
	std::shared_ptr<Effect> myLambertEffect;

	std::shared_ptr<MultiRenderTexture> myDeferredTextures;
	std::shared_ptr<RenderTexture> myLambertBuffer;

	bool myIsInstantRendering;
	void UpdateAndBindLightingBuffer();

	std::shared_ptr<Model> mySphereModel;

	Sprite myFullscreenQuad;
	std::shared_ptr<Effect> myLambertRenderingEffect;
};

