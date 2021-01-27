#pragma once

#include <core/Window.h>
#include <core/Game.h>
#include <core/Cursor.h>
#include <core/Input.h>
#include <core/KeyCodes.h>
#include <core/Core.h>

#include <util/Image.h>
#include <util/Timer.h>
#include <util/OrthographicCamera.h>
#include <util/OrthographicCameraController.h>
#include <util/PerspectiveCamera.h>
#include <util/PerspectiveCameraController.h>
#include <util/Time.h>
#include <util/Transform.h>
#include <util/io/FileSystem.h>

#include <events/Event.h>
#include <events/EventDispatcher.h>

#include <renderer/Buffer.h>
#include <renderer/shader/Shader.h>
#include <renderer/Texture2D.h>
#include <renderer/VertexArray.h>
#include <renderer/Framebuffer.h>
#include <renderer/Renderer.h>
#include <renderer/RenderCommand.h>
#include <renderer/Renderer2D.h>
#include <renderer/Renderer3D.h>
#include <renderer/text/TrueTypeFont.h>
#include <renderer/Mesh.h>
#include <renderer/MeshFactory.h>
#include <renderer/Animation.h>
#include <renderer/Assets.h>
#include <renderer/Particles.h>
#include <renderer/Model.h>
#include <renderer/TextureCube.h>
#include <renderer/InstancedRenderer.h>

#include <maths/math.h>

#include <audio/AudioController.h>
#include <audio/AudioSource.h>
#include <audio/AudioBuffer.h>
#include <audio/AudioListener.h>

#include <scene/Scene.h>
#include <scene/Components.h>
#include <scene/GameComponent.h>
#include <scene/GameObject.h>
#include <scene/ScriptableObject.h>

#include <physics/PhysicsWorld.h>
#include <physics/Body.h>
#include <physics/BoxBody.h>

#include <script/mono/mono.h>