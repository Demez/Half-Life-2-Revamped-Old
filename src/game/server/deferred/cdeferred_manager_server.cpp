
#include "cbase.h"
#include "deferred/deferred_shared_common.h"

#ifdef DEFERRED_LIGHTGEN
#include "filesystem.h"
#include "bspfile.h"

#include "tier0/memdbgon.h"
#endif

static CDeferredManagerServer __g_defmanager;
CDeferredManagerServer *GetDeferredManager()
{
	return &__g_defmanager;
}

CDeferredManagerServer::CDeferredManagerServer() : BaseClass( "DeferredManagerServer" )
{
}

CDeferredManagerServer::~CDeferredManagerServer()
{
}

bool CDeferredManagerServer::Init()
{
	return true;
}

void CDeferredManagerServer::Shutdown()
{
}

#ifdef DEFERRED_LIGHTGEN
#define LIGHT_MIN_LIGHT_VALUE 0.03f
float ComputeLightRadius(const dworldlight_t& light)
{
	float flLightRadius = light.radius;
	if (flLightRadius == 0.0f)
	{
		// Compute the light range based on attenuation factors
		const float flIntensity = sqrtf(DotProduct(light.intensity, light.intensity));
		if (light.quadratic_attn == 0.0f)
		{
			if (light.linear_attn == 0.0f)
			{
				// Infinite, but we're not going to draw it as such
				flLightRadius = 2000;
			}
			else
			{
				flLightRadius = (flIntensity / LIGHT_MIN_LIGHT_VALUE - light.constant_attn) / light.linear_attn;
			}
		}
		else
		{
			const float a = light.quadratic_attn;
			const float b = light.linear_attn;
			const float c = light.constant_attn - flIntensity / LIGHT_MIN_LIGHT_VALUE;
			const float discrim = b * b - 4 * a * c;
			if (discrim < 0.0f)
			{
				// Infinite, but we're not going to draw it as such
				flLightRadius = 2000;
			}
			else
			{
				flLightRadius = (-b + sqrtf(discrim)) / (2.0f * a);
				if (flLightRadius < 0)
					flLightRadius = 0;
			}
		}
	}

	return flLightRadius;
}
#endif

void CDeferredManagerServer::LevelInitPreEntity()
{
#ifdef DEFERRED_LIGHTGEN
	FileHandle_t hFile = g_pFullFileSystem->Open(MapName(), "rb");
	if (!hFile)
		return;

	BSPHeader_t header;
	g_pFullFileSystem->Read(&header, sizeof(BSPHeader_t), hFile);

	lump_t &lightLump = header.lumps[LUMP_WORLDLIGHTS];

	if (lightLump.filelen % sizeof(dworldlight_t))
		//return;
		return g_pFullFileSystem->Close(hFile);

	g_pFullFileSystem->Seek(hFile, lightLump.fileofs, FILESYSTEM_SEEK_HEAD);

	const size_t lightCount = lightLump.filelen / sizeof(dworldlight_t);

	dworldlight_t* lights = new dworldlight_t[lightCount];

	g_pFullFileSystem->Read(lights, lightLump.filelen, hFile);
	g_pFullFileSystem->Close(hFile);

	const char* szParamDiffuse = GetLightParamName(LPARAM_DIFFUSE);
	const char* szParamLightType = GetLightParamName(LPARAM_LIGHTTYPE);
	const char* szParamSpotConeInner = GetLightParamName(LPARAM_SPOTCONE_INNER);
	const char* szParamSpotConeOuter = GetLightParamName(LPARAM_SPOTCONE_OUTER);
	const char* szParamPower = GetLightParamName(LPARAM_POWER);
	const char* szParamRadius = GetLightParamName(LPARAM_RADIUS);
	const char* szParamVisDist = GetLightParamName(LPARAM_VIS_DIST);
	const char* szParamVisRange = GetLightParamName(LPARAM_VIS_RANGE);
	const char* szParamShadowDist = GetLightParamName(LPARAM_SHADOW_DIST);
	const char* szParamShadowRange = GetLightParamName(LPARAM_SHADOW_RANGE);

	for (size_t i = 0; i < lightCount; ++i)
	{
		const dworldlight_t& light = lights[i];

		if (light.type != emit_spotlight && light.type != emit_point)
			continue;

		const float radius = ComputeLightRadius(light);

		if (radius == 0)
			continue;

		CDeferredLight* lightEntity = static_cast<CDeferredLight*>(CBaseEntity::CreateNoSpawn("light_deferred", light.origin, vec3_angle));
		if (!lightEntity)
			break;

		lightEntity->KeyValue("spawnflags", "3");
		char	string[256];

		Vector intensity = light.intensity;
		const float ratio = light.constant_attn + 100 * light.linear_attn + 100 * 100 * light.quadratic_attn;
		if (ratio > 0)
			VectorScale(light.intensity, 1.f / ratio, intensity);
		intensity *= 255.f;
		//V_sprintf_safe(string, "%f %f %f 255", intensity.x, intensity.y, intensity.z);
		lightEntity->KeyValue(szParamDiffuse, string);
		if (light.type == emit_spotlight)
		{
			QAngle angle;
			VectorAngles(light.normal, angle);
			lightEntity->SetAbsAngles(angle);
			lightEntity->KeyValue(szParamLightType, "1");
			lightEntity->KeyValue(szParamSpotConeInner, acos(light.stopdot) * 180.f / M_PI_F);
			lightEntity->KeyValue(szParamSpotConeOuter, acos(light.stopdot2) * 180.f / M_PI_F);
			lightEntity->KeyValue(szParamPower, light.exponent);
		}
		else
		{
			lightEntity->KeyValue(szParamLightType, "0");
			lightEntity->KeyValue(szParamPower, "1");
		}

		lightEntity->KeyValue(szParamRadius, radius);
		lightEntity->KeyValue(szParamVisDist, radius * 2);
		lightEntity->KeyValue(szParamVisRange, radius * 1.25f);
		lightEntity->KeyValue(szParamShadowDist, radius * (5.f / 6.f));
		lightEntity->KeyValue(szParamShadowRange, radius * (2.f / 3.f));

		DispatchSpawn(lightEntity);
	}

	delete[] lights;
#endif
}

int CDeferredManagerServer::AddCookieTexture( const char *pszCookie )
{
	Assert( g_pStringTable_LightCookies != NULL );

	return  g_pStringTable_LightCookies->AddString( true, pszCookie );
}

void CDeferredManagerServer::AddWorldLight( CDeferredLight *l )
{
	CDeferredLightContainer *pC = FindAvailableContainer();

	if ( !pC )
		pC = assert_cast< CDeferredLightContainer* >( CreateEntityByName( "deferred_light_container" ) );

	pC->AddWorldLight( l );
}