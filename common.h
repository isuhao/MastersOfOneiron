#pragma once

//Masters of Oneiron headers

#include "mastercontrol.h"
#include "platform.h"
#include "imp.h"
#include "tile.h"
#include "slot.h"

//#define URHO3D_API

#include <Urho3D/Urho3D.h>
#include <Urho3D/Engine/Application.h>

// Engine 
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/UI/Text.h> 
#include <Urho3D/UI/UI.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/UI/Sprite.h>


#include <Urho3D/DebugNew.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/IO/FileSystem.h>

#include <Urho3D/Engine/Console.h>
#include <Urho3D/Engine/DebugHud.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Graphics/AnimationState.h>
#include <Urho3D/Graphics/Skeleton.h> 
#include <Urho3D/Graphics/Light.h> 
#include <Urho3D/Graphics/Model.h> 
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Math/Plane.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Core/Variant.h> 

#include <Urho3D/Input/Controls.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/IO/MemoryBuffer.h>

#include <Urho3D/Scene/ValueAnimation.h>
#include <Urho3D/Scene/ValueAnimationInfo.h>
#include <Urho3D/Graphics/DecalSet.h> 
#include <Urho3D/IO/PackageFile.h>
#include <Urho3D/Navigation/NavigationMesh.h>
#include <Urho3D/Navigation/Navigable.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Drawable.h>
#include <Urho3D/UI/Text.h> 
#include <Urho3D/UI/Text3D.h> 
#include <Urho3D/Graphics/Geometry.h> 
#include <Urho3D/Graphics/IndexBuffer.h>
#include <Urho3D/Graphics/VertexBuffer.h>

#include <Urho3D/Scene/SplinePath.h>
#include <Urho3D/IO/File.h>

//post FX
#include <Urho3D/Graphics/RenderPath.h> 

//UI
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIElement.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/Graphics/DrawableEvents.h>
#include <Urho3D/Script/ScriptInstance.h>


// SOUNDS
#include <Urho3D/Audio/Audio.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/SoundSource.h>
#include <Urho3D/Audio/SoundSource3D.h>
#include <Urho3D/Audio/SoundListener.h>

//namespace Urho3D
//{
//	class Node;
//	class Scene;
//	class Plane;
//	class RigidBody;
//	class LogicComponent;
//	class Controls;
//	class Drawable;
//	class ValueAnimation;
//	class Material;
//	class StaticModel;
//	class Geometry;
//	class AnimatedModel;
//	class AnimationState;
//	class SplinePath;
//	class Text;
//	class Viewport;
//	class RenderPath;
//	class Camera;
//	class DebugHud;
//	class ScriptInstance;
//
//	class Sound;
//	class Audio;
//	class SoundSource;
//	class SoundSource3D;
//	class SoundListener;
//};

using namespace Urho3D;




