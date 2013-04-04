#ifndef __ExampleApp__AppOnMap__
#define __ExampleApp__AppOnMap__

#include "IAppOnMap.h"
#include "EegeoWorld.h"
#include "RenderContext.h"
#include "NewGlobeCamera.h"
#include "IExample.h"
#include "TerrainHeightProvider.h"
#include "DebugSphereExample.h"
#include "ScreenUnprojectExample.h"
#include "LoadModelExample.h"
#include "EnvironmentNotifierExample.h"

namespace ExampleTypes
{
enum Examples
{
	DebugSphere=0,
	ScreenUnproject,
	TerrainHeightQuery,
	LoadModel,
	EnvironmentNotifier
};
}

ExampleTypes::Examples selectedExample = ExampleTypes::EnvironmentNotifier;

class MyApp : public Eegeo::IAppOnMap
{
private:
	Examples::IExample *pExample;

public:

	~MyApp()
	{
		pExample->Suspend();
		delete pExample;
	}

	void OnStart ()
	{
		float interestPointLatitudeDegrees = 37.7858f;
		float interestPointLongitudeDegrees = -122.401f;
		float interestPointAltitudeMeters = 2.7;

		Eegeo::Space::LatLongAltitude location = Eegeo::Space::LatLongAltitude(interestPointLatitudeDegrees,
				interestPointLongitudeDegrees,
				interestPointAltitudeMeters,
				Eegeo::Space::LatLongUnits::Degrees);

		World().GetCameraModel().SetWorldPosition(location.ToECEF());

		float cameraControllerOrientationDegrees = 0.0f;
		float cameraControllerDistanceFromInterestPointMeters = 1781.0f;

		Eegeo::Camera::NewGlobeCamera& controller = ((Eegeo::Camera::NewGlobeCamera&)World().GetCameraController());

		controller.SetInterestHeadingDistance(location,
				cameraControllerOrientationDegrees,
				cameraControllerDistanceFromInterestPointMeters);

		pExample = CreateExample(selectedExample,
				World().GetRenderContext(),
				location,
				World().GetCameraModel(),
				controller,
				*controller.GetCamera(),
				World().GetTerrainHeightProvider(),
				World().GetTextureLoader(),
				World().GetFileIO(),
				World().GetTerrainStreaming());

		pExample->Start();
	}

	void Update (float dt)
	{
		World().Update(dt);
		pExample->Update();
	}

	void Draw (float dt)
	{
		Eegeo::Rendering::GLState& glState = World().GetRenderContext().GetGLState();
		glState.ClearColor(0.8f, 0.8f, 0.8f, 1.f);
		World().Draw(dt);
		pExample->Draw();
	}

	Examples::IExample* CreateExample(ExampleTypes::Examples example,
			Eegeo::Rendering::RenderContext& renderContext,
			Eegeo::Space::LatLongAltitude interestLocation,
			Eegeo::Camera::CameraModel& cameraModel,
			Eegeo::Camera::NewGlobeCamera& globeCamera,
			Eegeo::RenderCamera& renderCamera,
			Eegeo::Resources::Terrain::Heights::TerrainHeightProvider& terrainHeightProvider,
			Eegeo::Helpers::ITextureFileLoader& textureLoader,
			Eegeo::Helpers::IFileIO& fileIO,
			Eegeo::Resources::Terrain::TerrainStreaming& terrainStreaming)
	{
		switch(example)
		{
		case ExampleTypes::LoadModel:
			return new Examples::LoadModelExample(renderContext,
					interestLocation,
					cameraModel,
					renderCamera,
					fileIO,
					textureLoader);
		case ExampleTypes::ScreenUnproject:
		case ExampleTypes::TerrainHeightQuery:
			return new Examples::ScreenUnprojectExample(renderContext,
					cameraModel,
					renderCamera,
					terrainHeightProvider);
		case ExampleTypes::DebugSphere:
			return new Examples::DebugSphereExample(renderContext,
					interestLocation,
					cameraModel,
					renderCamera);
		case ExampleTypes::EnvironmentNotifier:
			return new Examples::EnvironmentNotifierExample(renderContext,
					cameraModel,
					renderCamera,
					terrainStreaming);

		}
	}

	void Event_TouchRotate      (const AppInterface::RotateData& data) { World().GetCameraController().Event_TouchRotate(data); }
	void Event_TouchRotate_Start  (const AppInterface::RotateData& data) { World().GetCameraController().Event_TouchRotate_Start(data); }
	void Event_TouchRotate_End    (const AppInterface::RotateData& data) { World().GetCameraController().Event_TouchRotate_End(data); }

	void Event_TouchPinch       (const AppInterface::PinchData& data) { World().GetCameraController().Event_TouchPinch(data); }
	void Event_TouchPinch_Start   (const AppInterface::PinchData& data) { World().GetCameraController().Event_TouchPinch_Start(data); }
	void Event_TouchPinch_End     (const AppInterface::PinchData& data) { World().GetCameraController().Event_TouchPinch_End(data); }

	void Event_TouchPan       (const AppInterface::PanData& data) { World().GetCameraController().Event_TouchPan(data); }
	void Event_TouchPan_Start   (const AppInterface::PanData& data) { World().GetCameraController().Event_TouchPan_Start(data); }
	void Event_TouchPan_End     (const AppInterface::PanData& data) { World().GetCameraController().Event_TouchPan_End(data); }

	void Event_TouchTap       (const AppInterface::TapData& data) { World().GetCameraController().Event_TouchTap(data); }
	void Event_TouchDoubleTap   (const AppInterface::TapData& data) { World().GetCameraController().Event_TouchDoubleTap(data); }

	void Event_TouchDown      (const AppInterface::TouchData& data) { World().GetCameraController().Event_TouchDown(data); }
	void Event_TouchMove      (const AppInterface::TouchData& data) { World().GetCameraController().Event_TouchMove(data); }
	void Event_TouchUp        (const AppInterface::TouchData& data) { World().GetCameraController().Event_TouchUp(data); }
};

#endif /* defined(__ExampleApp__AppOnMap__) */
