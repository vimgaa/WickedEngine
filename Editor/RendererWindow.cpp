#include "stdafx.h"
#include "RendererWindow.h"
#include "Renderable3DComponent.h"


RendererWindow::RendererWindow(wiGUI* gui, Renderable3DComponent* component) : GUI(gui)
{
	assert(GUI && "Invalid GUI!");

	float screenW = (float)wiRenderer::GetDevice()->GetScreenWidth();
	float screenH = (float)wiRenderer::GetDevice()->GetScreenHeight();

	wiRenderer::SetToDrawDebugEnvProbes(true);
	wiRenderer::SetToDrawGridHelper(true);

	rendererWindow = new wiWindow(GUI, "Renderer Window");
	rendererWindow->SetSize(XMFLOAT2(640, 930));
	rendererWindow->SetEnabled(true);
	GUI->AddWidget(rendererWindow);

	float x = 260, y = 20, step = 30;

	vsyncCheckBox = new wiCheckBox("VSync: ");
	vsyncCheckBox->SetTooltip("Toggle vertical sync");
	vsyncCheckBox->SetScriptTip("SetVSyncEnabled(opt bool enabled)");
	vsyncCheckBox->SetPos(XMFLOAT2(x, y += step));
	vsyncCheckBox->OnClick([](wiEventArgs args) {
		wiRenderer::GetDevice()->SetVSyncEnabled(args.bValue);
	});
	vsyncCheckBox->SetCheck(wiRenderer::GetDevice()->GetVSyncEnabled());
	rendererWindow->AddWidget(vsyncCheckBox);

	occlusionCullingCheckBox = new wiCheckBox("Occlusion Culling: ");
	occlusionCullingCheckBox->SetTooltip("Toggle occlusion culling. This can boost framerate if many objects are occluded in the scene.");
	occlusionCullingCheckBox->SetScriptTip("SetOcclusionCullingEnabled(bool enabled)");
	occlusionCullingCheckBox->SetPos(XMFLOAT2(x, y += step));
	occlusionCullingCheckBox->OnClick([](wiEventArgs args) {
		wiRenderer::SetOcclusionCullingEnabled(args.bValue);
	});
	occlusionCullingCheckBox->SetCheck(wiRenderer::GetOcclusionCullingEnabled());
	rendererWindow->AddWidget(occlusionCullingCheckBox);

	resolutionScaleSlider = new wiSlider(0.25f, 2.0f, 1.0f, 7.0f, "Resolution Scale: ");
	resolutionScaleSlider->SetTooltip("Adjust the internal rendering resolution.");
	resolutionScaleSlider->SetSize(XMFLOAT2(100, 30));
	resolutionScaleSlider->SetPos(XMFLOAT2(x, y += 30));
	resolutionScaleSlider->SetValue(wiRenderer::GetResolutionScale());
	resolutionScaleSlider->OnSlide([&](wiEventArgs args) {
		wiRenderer::SetResolutionScale(args.fValue);
	});
	rendererWindow->AddWidget(resolutionScaleSlider);

	gammaSlider = new wiSlider(1.0f, 3.0f, 2.2f, 1000.0f, "Gamma: ");
	gammaSlider->SetTooltip("Adjust the gamma correction for the display device.");
	gammaSlider->SetSize(XMFLOAT2(100, 30));
	gammaSlider->SetPos(XMFLOAT2(x, y += 30));
	gammaSlider->SetValue(wiRenderer::GetGamma());
	gammaSlider->OnSlide([&](wiEventArgs args) {
		wiRenderer::SetGamma(args.fValue);
	});
	rendererWindow->AddWidget(gammaSlider);

	voxelRadianceCheckBox = new wiCheckBox("Voxel GI: ");
	voxelRadianceCheckBox->SetTooltip("Toggle voxel Global Illumination computation.");
	voxelRadianceCheckBox->SetPos(XMFLOAT2(x, y += step));
	voxelRadianceCheckBox->OnClick([](wiEventArgs args) {
		wiRenderer::SetVoxelRadianceEnabled(args.bValue);
	});
	voxelRadianceCheckBox->SetCheck(wiRenderer::GetVoxelRadianceEnabled());
	rendererWindow->AddWidget(voxelRadianceCheckBox);

	voxelRadianceDebugCheckBox = new wiCheckBox("DEBUG: ");
	voxelRadianceDebugCheckBox->SetTooltip("Toggle Voxel GI visualization.");
	voxelRadianceDebugCheckBox->SetPos(XMFLOAT2(x + 100, y));
	voxelRadianceDebugCheckBox->OnClick([](wiEventArgs args) {
		wiRenderer::SetToDrawVoxelHelper(args.bValue);
	});
	voxelRadianceDebugCheckBox->SetCheck(wiRenderer::GetToDrawVoxelHelper());
	rendererWindow->AddWidget(voxelRadianceDebugCheckBox);

	voxelRadianceSecondaryBounceCheckBox = new wiCheckBox("Voxel GI Secondary Light Bounce: ");
	voxelRadianceSecondaryBounceCheckBox->SetTooltip("Toggle secondary light bounce computation for Voxel GI.");
	voxelRadianceSecondaryBounceCheckBox->SetPos(XMFLOAT2(x, y += step));
	voxelRadianceSecondaryBounceCheckBox->OnClick([](wiEventArgs args) {
		wiRenderer::SetVoxelRadianceSecondaryBounceEnabled(args.bValue);
	});
	voxelRadianceSecondaryBounceCheckBox->SetCheck(wiRenderer::GetVoxelRadianceSecondaryBounceEnabled());
	rendererWindow->AddWidget(voxelRadianceSecondaryBounceCheckBox);

	voxelRadianceVoxelSizeSlider = new wiSlider(0.125, 10, 1, 79, "Voxel GI Voxel Size: ");
	voxelRadianceVoxelSizeSlider->SetTooltip("Adjust the voxel size for Voxel GI calculations.");
	voxelRadianceVoxelSizeSlider->SetSize(XMFLOAT2(100, 30));
	voxelRadianceVoxelSizeSlider->SetPos(XMFLOAT2(x, y += 30));
	voxelRadianceVoxelSizeSlider->SetValue(wiRenderer::GetVoxelRadianceVoxelSize());
	voxelRadianceVoxelSizeSlider->OnSlide([&](wiEventArgs args) {
		wiRenderer::SetVoxelRadianceVoxelSize(args.fValue);
	});
	rendererWindow->AddWidget(voxelRadianceVoxelSizeSlider);

	voxelRadianceConeTracingSlider = new wiSlider(1, 16, 8, 15, "Voxel GI Diffuse Cone Tracing Quality: ");
	voxelRadianceConeTracingSlider->SetTooltip("Adjust the number of cones sampled in the radiance gathering phase.");
	voxelRadianceConeTracingSlider->SetSize(XMFLOAT2(100, 30));
	voxelRadianceConeTracingSlider->SetPos(XMFLOAT2(x, y += 30));
	voxelRadianceConeTracingSlider->SetValue((float)wiRenderer::GetVoxelRadianceConeTracingQuality());
	voxelRadianceConeTracingSlider->OnSlide([&](wiEventArgs args) {
		wiRenderer::SetVoxelRadianceConeTracingQuality(args.iValue);
	});
	rendererWindow->AddWidget(voxelRadianceConeTracingSlider);

	voxelRadianceFalloffSlider = new wiSlider(0.01f, 2, 1, 10000, "Voxel AO Falloff: ");
	voxelRadianceFalloffSlider->SetTooltip("Adjust the falloff of the Voxel Ambient Occlusion.");
	voxelRadianceFalloffSlider->SetSize(XMFLOAT2(100, 30));
	voxelRadianceFalloffSlider->SetPos(XMFLOAT2(x, y += 30));
	voxelRadianceFalloffSlider->SetValue(wiRenderer::GetVoxelRadianceFalloff());
	voxelRadianceFalloffSlider->OnSlide([&](wiEventArgs args) {
		wiRenderer::SetVoxelRadianceFalloff(args.fValue);
	});
	rendererWindow->AddWidget(voxelRadianceFalloffSlider);

	specularAASlider = new wiSlider(0, 1, 1, 10000, "Specular Antialiasing: ");
	specularAASlider->SetTooltip("Adjust specular antialiasing strength. This can reduce flickering of specular highlights.");
	specularAASlider->SetSize(XMFLOAT2(100, 30));
	specularAASlider->SetPos(XMFLOAT2(x, y += 30));
	specularAASlider->SetValue(wiRenderer::GetSpecularAAParam());
	specularAASlider->OnSlide([&](wiEventArgs args) {
		wiRenderer::SetSpecularAAParam(args.fValue);
	});
	rendererWindow->AddWidget(specularAASlider);

	wireFrameCheckBox = new wiCheckBox("Render Wireframe: ");
	wireFrameCheckBox->SetTooltip("Visualize the scene as a wireframe");
	wireFrameCheckBox->SetPos(XMFLOAT2(x, y += step));
	wireFrameCheckBox->OnClick([](wiEventArgs args) {
		wiRenderer::SetWireRender(args.bValue);
	});
	wireFrameCheckBox->SetCheck(wiRenderer::IsWireRender());
	rendererWindow->AddWidget(wireFrameCheckBox);

	advancedLightCullingCheckBox = new wiCheckBox("2.5D Light Culling: ");
	advancedLightCullingCheckBox->SetTooltip("Enable a more aggressive light culling approach which can result in slower culling but faster rendering (Tiled renderer only)");
	advancedLightCullingCheckBox->SetPos(XMFLOAT2(x, y += step));
	advancedLightCullingCheckBox->OnClick([](wiEventArgs args) {
		wiRenderer::SetAdvancedLightCulling(args.bValue);
	});
	advancedLightCullingCheckBox->SetCheck(wiRenderer::GetAdvancedLightCulling());
	rendererWindow->AddWidget(advancedLightCullingCheckBox);

	debugLightCullingCheckBox = new wiCheckBox("DEBUG: ");
	debugLightCullingCheckBox->SetTooltip("Toggle visualization of the screen space light culling heatmap grid (Tiled renderer only)");
	debugLightCullingCheckBox->SetPos(XMFLOAT2(x + 100, y));
	debugLightCullingCheckBox->OnClick([](wiEventArgs args) {
		wiRenderer::SetDebugLightCulling(args.bValue);
	});
	debugLightCullingCheckBox->SetCheck(wiRenderer::GetDebugLightCulling());
	rendererWindow->AddWidget(debugLightCullingCheckBox);

	tessellationCheckBox = new wiCheckBox("Tessellation Enabled: ");
	tessellationCheckBox->SetTooltip("Enable tessellation feature. You also need to specify a tessellation factor for individual objects.");
	tessellationCheckBox->SetPos(XMFLOAT2(x, y += step));
	tessellationCheckBox->OnClick([=](wiEventArgs args) {
		component->setTessellationEnabled(args.bValue);
	});
	tessellationCheckBox->SetCheck(false);
	rendererWindow->AddWidget(tessellationCheckBox);
	tessellationCheckBox->SetEnabled(wiRenderer::GetDevice()->CheckCapability(wiGraphicsTypes::GraphicsDevice::GRAPHICSDEVICE_CAPABILITY_TESSELLATION));

	advancedRefractionsCheckBox = new wiCheckBox("Advanced Refractions: ");
	advancedRefractionsCheckBox->SetTooltip("Enable advanced refraction rendering: rough transparent materials will be more matte. This needs additional support from the graphics driver.");
	advancedRefractionsCheckBox->SetPos(XMFLOAT2(x, y += step));
	advancedRefractionsCheckBox->OnClick([=](wiEventArgs args) {
		wiRenderer::SetAdvancedRefractionsEnabled(args.bValue);
	});
	advancedRefractionsCheckBox->SetCheck(wiRenderer::GetAdvancedRefractionsEnabled());
	rendererWindow->AddWidget(advancedRefractionsCheckBox);
	advancedRefractionsCheckBox->SetEnabled(wiRenderer::GetDevice()->CheckCapability(wiGraphicsTypes::GraphicsDevice::GRAPHICSDEVICE_CAPABILITY_UNORDEREDACCESSTEXTURE_LOAD_FORMAT_EXT));


	pickTypeObjectCheckBox = new wiCheckBox("Pick Objects: ");
	pickTypeObjectCheckBox->SetTooltip("Enable if you want to pick objects with the pointer");
	pickTypeObjectCheckBox->SetPos(XMFLOAT2(x, y += step));
	pickTypeObjectCheckBox->SetCheck(true);
	rendererWindow->AddWidget(pickTypeObjectCheckBox);

	pickTypeEnvProbeCheckBox = new wiCheckBox("Pick EnvProbes: ");
	pickTypeEnvProbeCheckBox->SetTooltip("Enable if you want to pick environment probes with the pointer");
	pickTypeEnvProbeCheckBox->SetPos(XMFLOAT2(x, y += step));
	pickTypeEnvProbeCheckBox->SetCheck(true);
	rendererWindow->AddWidget(pickTypeEnvProbeCheckBox);

	pickTypeLightCheckBox = new wiCheckBox("Pick Lights: ");
	pickTypeLightCheckBox->SetTooltip("Enable if you want to pick lights with the pointer");
	pickTypeLightCheckBox->SetPos(XMFLOAT2(x, y += step));
	pickTypeLightCheckBox->SetCheck(true);
	rendererWindow->AddWidget(pickTypeLightCheckBox);

	pickTypeDecalCheckBox = new wiCheckBox("Pick Decals: ");
	pickTypeDecalCheckBox->SetTooltip("Enable if you want to pick decals with the pointer");
	pickTypeDecalCheckBox->SetPos(XMFLOAT2(x, y += step));
	pickTypeDecalCheckBox->SetCheck(true);
	rendererWindow->AddWidget(pickTypeDecalCheckBox);

	pickTypeForceFieldCheckBox = new wiCheckBox("Pick Force Fields: ");
	pickTypeForceFieldCheckBox->SetTooltip("Enable if you want to pick force fields with the pointer");
	pickTypeForceFieldCheckBox->SetPos(XMFLOAT2(x, y += step));
	pickTypeForceFieldCheckBox->SetCheck(true);
	rendererWindow->AddWidget(pickTypeForceFieldCheckBox);

	pickTypeEmitterCheckBox = new wiCheckBox("Pick Emitters: ");
	pickTypeEmitterCheckBox->SetTooltip("Enable if you want to pick emitters with the pointer");
	pickTypeEmitterCheckBox->SetPos(XMFLOAT2(x, y += step));
	pickTypeEmitterCheckBox->SetCheck(true);
	rendererWindow->AddWidget(pickTypeEmitterCheckBox);

	speedMultiplierSlider = new wiSlider(0, 4, 1, 100000, "Speed: ");
	speedMultiplierSlider->SetTooltip("Adjust the global speed (time multiplier)");
	speedMultiplierSlider->SetSize(XMFLOAT2(100, 30));
	speedMultiplierSlider->SetPos(XMFLOAT2(x, y += 30));
	speedMultiplierSlider->SetValue(wiRenderer::GetGameSpeed());
	speedMultiplierSlider->OnSlide([&](wiEventArgs args) {
		wiRenderer::SetGameSpeed(args.fValue);
	});
	rendererWindow->AddWidget(speedMultiplierSlider);

	transparentShadowsCheckBox = new wiCheckBox("Transparent Shadows: ");
	transparentShadowsCheckBox->SetTooltip("Enables color tinted shadows and refraction caustics effects for transparent objects and water.");
	transparentShadowsCheckBox->SetPos(XMFLOAT2(x, y += step));
	transparentShadowsCheckBox->SetCheck(wiRenderer::GetTransparentShadowsEnabled());
	transparentShadowsCheckBox->OnClick([=](wiEventArgs args) {
		wiRenderer::SetTransparentShadowsEnabled(args.bValue);
	});
	rendererWindow->AddWidget(transparentShadowsCheckBox);

	shadowProps2DComboBox = new wiComboBox("2D Shadowmap resolution:");
	shadowProps2DComboBox->SetSize(XMFLOAT2(100, 20));
	shadowProps2DComboBox->SetPos(XMFLOAT2(x, y += step));
	shadowProps2DComboBox->AddItem("Off");
	shadowProps2DComboBox->AddItem("128");
	shadowProps2DComboBox->AddItem("256");
	shadowProps2DComboBox->AddItem("512");
	shadowProps2DComboBox->AddItem("1024");
	shadowProps2DComboBox->AddItem("2048");
	shadowProps2DComboBox->AddItem("4096");
	shadowProps2DComboBox->OnSelect([&](wiEventArgs args) {
		wiRenderer::SHADOWCOUNT_2D = 11;
		switch (args.iValue)
		{
		case 0:
			wiRenderer::SetShadowProps2D(128, 0, wiRenderer::SOFTSHADOWQUALITY_2D);
			break;
		case 1:
			wiRenderer::SetShadowProps2D(128, wiRenderer::SHADOWCOUNT_2D, wiRenderer::SOFTSHADOWQUALITY_2D);
			break;
		case 2:
			wiRenderer::SetShadowProps2D(256, wiRenderer::SHADOWCOUNT_2D, wiRenderer::SOFTSHADOWQUALITY_2D);
			break;
		case 3:
			wiRenderer::SetShadowProps2D(512, wiRenderer::SHADOWCOUNT_2D, wiRenderer::SOFTSHADOWQUALITY_2D);
			break;
		case 4:
			wiRenderer::SetShadowProps2D(1024, wiRenderer::SHADOWCOUNT_2D, wiRenderer::SOFTSHADOWQUALITY_2D);
			break;
		case 5:
			wiRenderer::SetShadowProps2D(2048, wiRenderer::SHADOWCOUNT_2D, wiRenderer::SOFTSHADOWQUALITY_2D);
			break;
		case 6:
			wiRenderer::SetShadowProps2D(4096, wiRenderer::SHADOWCOUNT_2D, wiRenderer::SOFTSHADOWQUALITY_2D);
			break;
		default:
			break;
		}
	});
	shadowProps2DComboBox->SetSelected(4);
	shadowProps2DComboBox->SetEnabled(true);
	shadowProps2DComboBox->SetTooltip("Choose a shadow quality preset for 2D shadow maps (spotlights, directional lights)...");
	shadowProps2DComboBox->SetScriptTip("SetShadowProps2D(int resolution, int count, int softShadowQuality)");
	rendererWindow->AddWidget(shadowProps2DComboBox);

	shadowPropsCubeComboBox = new wiComboBox("Cube Shadowmap resolution:");
	shadowPropsCubeComboBox->SetSize(XMFLOAT2(100, 20));
	shadowPropsCubeComboBox->SetPos(XMFLOAT2(x, y += step));
	shadowPropsCubeComboBox->AddItem("Off");
	shadowPropsCubeComboBox->AddItem("128");
	shadowPropsCubeComboBox->AddItem("256");
	shadowPropsCubeComboBox->AddItem("512");
	shadowPropsCubeComboBox->AddItem("1024");
	shadowPropsCubeComboBox->AddItem("2048");
	shadowPropsCubeComboBox->AddItem("4096");
	shadowPropsCubeComboBox->OnSelect([&](wiEventArgs args) {
		wiRenderer::SHADOWCOUNT_CUBE = 5;
		switch (args.iValue)
		{
		case 0:
			wiRenderer::SetShadowPropsCube(128, 0);
			break;
		case 1:
			wiRenderer::SetShadowPropsCube(128, wiRenderer::SHADOWCOUNT_CUBE);
			break;
		case 2:
			wiRenderer::SetShadowPropsCube(256, wiRenderer::SHADOWCOUNT_CUBE);
			break;
		case 3:
			wiRenderer::SetShadowPropsCube(512, wiRenderer::SHADOWCOUNT_CUBE);
			break;
		case 4:
			wiRenderer::SetShadowPropsCube(1024, wiRenderer::SHADOWCOUNT_CUBE);
			break;
		case 5:
			wiRenderer::SetShadowPropsCube(2048, wiRenderer::SHADOWCOUNT_CUBE);
			break;
		case 6:
			wiRenderer::SetShadowPropsCube(4096, wiRenderer::SHADOWCOUNT_CUBE);
			break;
		default:
			break;
		}
	});
	shadowPropsCubeComboBox->SetSelected(2);
	shadowPropsCubeComboBox->SetEnabled(true);
	shadowPropsCubeComboBox->SetTooltip("Choose a shadow quality preset for cube shadow maps (pointlights, area lights)...");
	shadowPropsCubeComboBox->SetScriptTip("SetShadowPropsCube(int resolution, int count)");
	rendererWindow->AddWidget(shadowPropsCubeComboBox);

	MSAAComboBox = new wiComboBox("MSAA:");
	MSAAComboBox->SetSize(XMFLOAT2(100, 20));
	MSAAComboBox->SetPos(XMFLOAT2(x, y += step));
	MSAAComboBox->AddItem("Off");
	MSAAComboBox->AddItem("2");
	MSAAComboBox->AddItem("4");
	MSAAComboBox->AddItem("8");
	MSAAComboBox->OnSelect([=](wiEventArgs args) {
		switch (args.iValue)
		{
		case 0:
			component->setMSAASampleCount(1);
			break;
		case 1:
			component->setMSAASampleCount(2);
			break;
		case 2:
			component->setMSAASampleCount(4);
			break;
		case 3:
			component->setMSAASampleCount(8);
			break;
		default:
			break;
		}
	});
	MSAAComboBox->SetSelected(0);
	MSAAComboBox->SetEnabled(true);
	MSAAComboBox->SetTooltip("Multisampling Anti Aliasing quality. It is only available for Forward render paths.");
	rendererWindow->AddWidget(MSAAComboBox);

	temporalAACheckBox = new wiCheckBox("Temporal AA: ");
	temporalAACheckBox->SetTooltip("Toggle Temporal Anti Aliasing. It is a supersampling techique which is performed across multiple frames.");
	temporalAACheckBox->SetPos(XMFLOAT2(x, y += step));
	temporalAACheckBox->OnClick([](wiEventArgs args) {
		wiRenderer::SetTemporalAAEnabled(args.bValue);
	});
	temporalAACheckBox->SetCheck(wiRenderer::GetTemporalAAEnabled());
	rendererWindow->AddWidget(temporalAACheckBox);

	temporalAADebugCheckBox = new wiCheckBox("DEBUGJitter: ");
	temporalAADebugCheckBox->SetText("DEBUG: ");
	temporalAADebugCheckBox->SetTooltip("Disable blending of frame history. Camera Subpixel jitter will be visible.");
	temporalAADebugCheckBox->SetPos(XMFLOAT2(x + 100, y));
	temporalAADebugCheckBox->OnClick([](wiEventArgs args) {
		wiRenderer::SetTemporalAADebugEnabled(args.bValue);
	});
	temporalAADebugCheckBox->SetCheck(wiRenderer::GetTemporalAADebugEnabled());
	rendererWindow->AddWidget(temporalAADebugCheckBox);

	textureQualityComboBox = new wiComboBox("Texture Quality:");
	textureQualityComboBox->SetSize(XMFLOAT2(100, 20));
	textureQualityComboBox->SetPos(XMFLOAT2(x, y += step));
	textureQualityComboBox->AddItem("Nearest");
	textureQualityComboBox->AddItem("Bilinear");
	textureQualityComboBox->AddItem("Trilinear");
	textureQualityComboBox->AddItem("Anisotropic");
	textureQualityComboBox->OnSelect([&](wiEventArgs args) {
		wiGraphicsTypes::SamplerDesc desc = wiRenderer::samplers[SSLOT_OBJECTSHADER]->GetDesc();
		SAFE_DELETE(wiRenderer::samplers[SSLOT_OBJECTSHADER]);

		switch (args.iValue)
		{
		case 0:
			desc.Filter = wiGraphicsTypes::FILTER_MIN_MAG_MIP_POINT;
			break;
		case 1:
			desc.Filter = wiGraphicsTypes::FILTER_MIN_MAG_LINEAR_MIP_POINT;
			break;
		case 2:
			desc.Filter = wiGraphicsTypes::FILTER_MIN_MAG_MIP_LINEAR;
			break;
		case 3:
			desc.Filter = wiGraphicsTypes::FILTER_ANISOTROPIC;
			break;
		default:
			break;
		}

		wiRenderer::samplers[SSLOT_OBJECTSHADER] = new wiGraphicsTypes::Sampler;
		wiRenderer::GetDevice()->CreateSamplerState(&desc, wiRenderer::samplers[SSLOT_OBJECTSHADER]);
	});
	textureQualityComboBox->SetSelected(3);
	textureQualityComboBox->SetEnabled(true);
	textureQualityComboBox->SetTooltip("Choose a texture sampling method.");
	rendererWindow->AddWidget(textureQualityComboBox);

	mipLodBiasSlider = new wiSlider(-2, 2, 0, 100000, "MipLOD Bias: ");
	mipLodBiasSlider->SetTooltip("Bias the rendered mip map level of the material textures.");
	mipLodBiasSlider->SetSize(XMFLOAT2(100, 30));
	mipLodBiasSlider->SetPos(XMFLOAT2(x, y += 30));
	mipLodBiasSlider->OnSlide([&](wiEventArgs args) {
		wiGraphicsTypes::SamplerDesc desc = wiRenderer::samplers[SSLOT_OBJECTSHADER]->GetDesc();
		SAFE_DELETE(wiRenderer::samplers[SSLOT_OBJECTSHADER]);
		desc.MipLODBias = args.fValue;
		wiRenderer::samplers[SSLOT_OBJECTSHADER] = new wiGraphicsTypes::Sampler;
		wiRenderer::GetDevice()->CreateSamplerState(&desc, wiRenderer::samplers[SSLOT_OBJECTSHADER]);
	});
	rendererWindow->AddWidget(mipLodBiasSlider);



	// Visualizer toggles:
	x = 600, y = 20;

	partitionBoxesCheckBox = new wiCheckBox("SPTree visualizer: ");
	partitionBoxesCheckBox->SetTooltip("Visualize the world space partitioning tree as boxes");
	partitionBoxesCheckBox->SetScriptTip("SetDebugPartitionTreeEnabled(bool enabled)");
	partitionBoxesCheckBox->SetPos(XMFLOAT2(x, y += step));
	partitionBoxesCheckBox->OnClick([](wiEventArgs args) {
		wiRenderer::SetToDrawDebugPartitionTree(args.bValue);
	});
	partitionBoxesCheckBox->SetCheck(wiRenderer::GetToDrawDebugPartitionTree());
	rendererWindow->AddWidget(partitionBoxesCheckBox);

	boneLinesCheckBox = new wiCheckBox("Bone line visualizer: ");
	boneLinesCheckBox->SetTooltip("Visualize bones of armatures");
	boneLinesCheckBox->SetScriptTip("SetDebugBonesEnabled(bool enabled)");
	boneLinesCheckBox->SetPos(XMFLOAT2(x, y += step));
	boneLinesCheckBox->OnClick([](wiEventArgs args) {
		wiRenderer::SetToDrawDebugBoneLines(args.bValue);
	});
	boneLinesCheckBox->SetCheck(wiRenderer::GetToDrawDebugBoneLines());
	rendererWindow->AddWidget(boneLinesCheckBox);

	debugEmittersCheckBox = new wiCheckBox("Emitter visualizer: ");
	debugEmittersCheckBox->SetTooltip("Visualize emitters");
	debugEmittersCheckBox->SetScriptTip("SetDebugEmittersEnabled(bool enabled)");
	debugEmittersCheckBox->SetPos(XMFLOAT2(x, y += step));
	debugEmittersCheckBox->OnClick([](wiEventArgs args) {
		wiRenderer::SetToDrawDebugEmitters(args.bValue);
	});
	debugEmittersCheckBox->SetCheck(wiRenderer::GetToDrawDebugEmitters());
	rendererWindow->AddWidget(debugEmittersCheckBox);

	debugForceFieldsCheckBox = new wiCheckBox("Force Field visualizer: ");
	debugForceFieldsCheckBox->SetTooltip("Visualize force fields");
	debugForceFieldsCheckBox->SetScriptTip("SetDebugForceFieldsEnabled(bool enabled)");
	debugForceFieldsCheckBox->SetPos(XMFLOAT2(x, y += step));
	debugForceFieldsCheckBox->OnClick([](wiEventArgs args) {
		wiRenderer::SetToDrawDebugForceFields(args.bValue);
	});
	debugForceFieldsCheckBox->SetCheck(wiRenderer::GetToDrawDebugForceFields());
	rendererWindow->AddWidget(debugForceFieldsCheckBox);

	envProbesCheckBox = new wiCheckBox("Env probe visualizer: ");
	envProbesCheckBox->SetTooltip("Toggle visualization of environment probes as reflective spheres");
	envProbesCheckBox->SetPos(XMFLOAT2(x, y += step));
	envProbesCheckBox->OnClick([](wiEventArgs args) {
		wiRenderer::SetToDrawDebugEnvProbes(args.bValue);
	});
	envProbesCheckBox->SetCheck(wiRenderer::GetToDrawDebugEnvProbes());
	rendererWindow->AddWidget(envProbesCheckBox);

	gridHelperCheckBox = new wiCheckBox("Grid helper: ");
	gridHelperCheckBox->SetTooltip("Toggle showing of unit visualizer grid in the world origin");
	gridHelperCheckBox->SetPos(XMFLOAT2(x, y += step));
	gridHelperCheckBox->OnClick([](wiEventArgs args) {
		wiRenderer::SetToDrawGridHelper(args.bValue);
	});
	gridHelperCheckBox->SetCheck(wiRenderer::GetToDrawGridHelper());
	rendererWindow->AddWidget(gridHelperCheckBox);

	freezeCullingCameraCheckBox = new wiCheckBox("Freeze culling camera: ");
	freezeCullingCameraCheckBox->SetTooltip("Freeze culling camera update. Scene culling will not be updated with the view");
	freezeCullingCameraCheckBox->SetPos(XMFLOAT2(x, y += step * 2));
	freezeCullingCameraCheckBox->OnClick([](wiEventArgs args) {
		wiRenderer::SetFreezeCullingCameraEnabled(args.bValue);
	});
	freezeCullingCameraCheckBox->SetCheck(wiRenderer::GetToDrawDebugForceFields());
	rendererWindow->AddWidget(freezeCullingCameraCheckBox);



	rendererWindow->Translate(XMFLOAT3(130, 20, 0));
	rendererWindow->SetVisible(false);
}


RendererWindow::~RendererWindow()
{
	rendererWindow->RemoveWidgets(true);
	GUI->RemoveWidget(rendererWindow);
	SAFE_DELETE(rendererWindow);
}

int RendererWindow::GetPickType()
{
	int pickType = PICK_VOID;
	if (pickTypeObjectCheckBox->GetCheck())
	{
		pickType |= PICK_OPAQUE;
		pickType |= PICK_TRANSPARENT;
		pickType |= PICK_WATER;
	}
	if (pickTypeEnvProbeCheckBox->GetCheck())
	{
		pickType |= PICK_ENVPROBE;
	}
	if (pickTypeLightCheckBox->GetCheck())
	{
		pickType |= PICK_LIGHT;
	}
	if (pickTypeDecalCheckBox->GetCheck())
	{
		pickType |= PICK_DECAL;
	}
	if (pickTypeForceFieldCheckBox->GetCheck())
	{
		pickType |= PICK_FORCEFIELD;
	}
	if (pickTypeEmitterCheckBox->GetCheck())
	{
		pickType |= PICK_EMITTER;
	}

	return pickType;
}
