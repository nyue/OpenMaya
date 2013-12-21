#include "Indigo.h"
#include "utilities/logging.h"
#include "utilities/tools.h"
#include "utilities/attrTools.h"
#include "../mtin_common/mtin_renderGlobals.h"
#include "../mtin_common/mtin_mayaScene.h"
#include "../mtin_common/mtin_mayaObject.h"

static Logging logger;

void IndigoRenderer::defineEnvironment()
{
	MObject indigoGlobals = objectFromName(MString("indigoGlobals"));
	MFnDependencyNode globalsNode(indigoGlobals);

	switch( this->mtin_renderGlobals->environmentType )
	{
		case 0: // off
		{ 
			break;
		}
		case 1: // environment light map
		{
			MString texName;
			bool useTexture = false;
			Indigo::String texturePath = "";
			if( getConnectedFileTexturePath(MString("environmentColor"), MString("indigoGlobals"), texName) )
			{
				useTexture = true;
				texturePath = texName.asChar();
			}
			MColor bgColor(1,1,1);
			getColor("environmentColor", globalsNode, bgColor);
			if( (bgColor.r + bgColor.g + bgColor.b) <= 0.0)
			{
				logger.warning("BGColor is black, setting to red.");
				bgColor.r = 1.0f;
				bgColor.g = bgColor.b = 0.0f;
			}

			Indigo::SceneNodeBackgroundSettingsRef background_settings(new Indigo::SceneNodeBackgroundSettings());

			Reference<Indigo::DiffuseMaterial> mat(new Indigo::DiffuseMaterial());

			// Albedo should be zero.
			mat->albedo = Reference<Indigo::WavelengthDependentParam>(new Indigo::ConstantWavelengthDependentParam(Reference<Indigo::Spectrum>(new Indigo::UniformSpectrum(0))));


			Indigo::Texture texture;
			// Emission is a texture parameter that references our texture that we will create below.
			//mat->emission = Reference<Indigo::WavelengthDependentParam>(new Indigo::TextureWavelengthDependentParam(0));
			if( useTexture )
			{
				texture.path = texturePath;
				texture.exponent = 1; // Since we will usually use a HDR image, the exponent (gamma) should be set to one.
				texture.tex_coord_generation = Reference<Indigo::TexCoordGenerator>(new Indigo::SphericalTexCoordGenerator(Reference<Indigo::Rotation>(new Indigo::MatrixRotation())));
				mat->emission = Reference<Indigo::WavelengthDependentParam>(new Indigo::TextureWavelengthDependentParam(0));
				mat->textures.push_back(texture);
			}else{
				Indigo::RGBSpectrum *iBgColor = new Indigo::RGBSpectrum(Indigo::Vec3d(bgColor.r,bgColor.g,bgColor.b), 2.2);
				mat->emission = Reference<Indigo::WavelengthDependentParam>(new Indigo::ConstantWavelengthDependentParam(Reference<Indigo::Spectrum>(iBgColor)));
			}
			// Base emission is the emitted spectral radiance. No effect here?
			mat->base_emission = Reference<Indigo::WavelengthDependentParam>(new Indigo::ConstantWavelengthDependentParam(Reference<Indigo::Spectrum>(new Indigo::UniformSpectrum(1.0e10))));

			background_settings->background_material = mat;

			sceneRootRef->addChildNode(background_settings);
			break;
		}
		case 2: // sun/sky
		{
			// first get the globals node and serach for a directional light connection
			MObjectArray nodeList;
			getConnectedInNodes(MString("environmentSun"), indigoGlobals, nodeList);
			if( nodeList.length() > 0)
			{
				MObject sunObj = nodeList[0];
				if(sunObj.hasFn(MFn::kTransform))
				{
					// we suppose what's connected here is a dir light transform
					MVector lightDir(0,0,1); // default dir light dir
					MFnDagNode sunDagNode(sunObj);
					lightDir *= sunDagNode.transformationMatrix() * this->mtin_renderGlobals->globalConversionMatrix;
					lightDir.normalize();

					Indigo::SceneNodeBackgroundSettingsRef background_settings_node(new Indigo::SceneNodeBackgroundSettings());
					Reference<Indigo::SunSkyMaterial> sun_sky_mat(new Indigo::SunSkyMaterial());
					
					MString sky_model;
					int modelId;
					getEnum("sky_model", globalsNode, modelId, sky_model);
					sun_sky_mat->model = sky_model.asChar();
					sun_sky_mat->enable_sky = true;
					getBool("extra_atmospheric", globalsNode, sun_sky_mat->extra_atmospheric);
					sun_sky_mat->name = "sunsky";
					getUInt("sky_layer", globalsNode, sun_sky_mat->sky_layer);
					getUInt("sun_layer", globalsNode, sun_sky_mat->sun_layer);
					getDouble(MString("turbidity"), globalsNode, sun_sky_mat->turbidity);

					sun_sky_mat->sundir = Indigo::Vec3d(lightDir.x, lightDir.y, lightDir.z); // Direction to sun.

					background_settings_node->background_material = sun_sky_mat;
					sceneRootRef->addChildNode(background_settings_node);
				}
			}

			break;
		}
	}
}

