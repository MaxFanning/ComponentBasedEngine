#ifndef INCLUDED_ENGINE_H
#define INCLUDED_ENGINE_H

//====================================================================================================
// Filename:	Engine.h
// Created by:	Peter Chan
// Description:	3D Engine.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Common.h"

// Core headers
#include "Application.h"
#include "Delegate.h"
#include "Random.h"

// GameObject headers
#include "Component.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "World.h"

// GameObject component headers
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "FPSCameraControllerComponent.h"
#include "MinionComponent.h"
#include "SpawnerComponent.h"
#include "TowerComponent.h"
#include "BulletComponent.h"
#include "ModelComponent.h"
#include "BasicBulletComponent.h"
#include "TurretComponent.h"

// Service headers
#include "Service.h"
#include "MinionLocationService.h"
#include "ServiceFactory.h"

// Graphics headers
#include "AnimationClip.h"
#include "AnimationController.h"
#include "Bone.h"
#include "BoneAnimation.h"
#include "Camera.h"
#include "Color.h"
#include "ConstantBuffer.h"
#include "Font.h"
#include "GraphicsSystem.h"
#include "Heightmap.h"
#include "Keyframe.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "MeshBuilder.h"
#include "MeshRenderer.h"
#include "MeshUtil.h"
#include "Model.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "SimpleDraw.h"
#include "SpriteRenderer.h"
#include "Terrain.h"
#include "Texture.h"
#include "VertexShader.h"
#include "ModelManager.h"
#include "TextureManager.h"

// Input heades
#include "InputEvent.h"
#include "InputSystem.h"
#include "InputTypes.h"

// Math headers
#include "EngineMath.h"

// Memory headers
#include "BlockAllocator.h"
#include "TypedAllocator.h"
#include "ArenaAllocator.h"
#include "HandlePool.h"

// Physics headers
#include "Constraints.h"
#include "Particle.h"
#include "PhysicsWorld.h"

// Platform headers
#include "Timer.h"
#include "Window.h"

#endif // #ifndef INCLUDED_ENGINE_H