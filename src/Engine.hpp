#pragma once
#include "LogManager.hpp"
#include "FileSystemManager.hpp"
#include "Network.hpp"
#include "ResourceManager.hpp"
#include "InputManager.hpp"
#include "RenderSystem.hpp"
#include "RenderWindow.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Mesh.hpp"
#include "MeshEntity.hpp"
#include "Math.hpp"
#include "Camera.hpp"
#include "DX9RenderSystem.hpp"
#include "OGLRenderSystem.hpp"
#include "FrameListener.hpp"
#include "ScriptManager.hpp"
#include "OverlayManager.hpp"
#include "SceneTree.hpp"
#include "SceneNode.hpp"
#include "SkeletonNode.hpp"
#include "OverlayImage.hpp"
#include "OverlayButton.hpp"
#include "OverlayTextBox.hpp"
#include "OverlayListener.hpp"
#include "Font.hpp"

namespace Halia
{
	namespace RendererType
	{
		enum RendererType
		{
			DX9 = 1,
			OpenGL = 2,
			/* Not Implemented
			Software,
			DX10,
			*/
		};
	};

	class Engine : public EventListener
	{
	public:
		Engine( );
		~Engine( );

		bool Init( RendererType::RendererType renderer = RendererType::DX9, bool wndauto = true, std::string wndtitle = "Halia Engine", int width = 800, int height = 600 );
		void Destroy( );
		void StartRendering( );
		void Quit( );
		virtual bool EventHandler( EventBase* evt );

		void SetFrameListener( FrameListener* fm );
		void SetEventListener( EventListener* em );
		void SetScriptListener( ScriptListener* sm );
		
		RendererType::RendererType& GetRendererType( );
		RenderWindow*& GetWindow( );
		RenderSystem*& GetRenderer( );
		OverlayManager*& GetOverlayer( );
		ScriptManager*& GetScripter( );
		InputManager*& GetInputer( );
		ResourceManager*& GetResourceManager( );

	private:
		RendererType::RendererType mRType;
		RenderSystem* mRenderer;
		RenderWindow* mWindow;
		InputManager* mInputer;
		ScriptManager* mScripter;
		OverlayManager* mOverlayer;
		OverlayRenderer* mORenderer;
		ResourceManager* mRManager;
		FrameListener* mFrameListener;
		EventListener* mEventListener;
		int mFrameCount;
		int mBatchCount;
		int mPrimCount;
		int mStartTime;
		char mSFps[6][60];
	};

	extern Engine* EInstance;
};