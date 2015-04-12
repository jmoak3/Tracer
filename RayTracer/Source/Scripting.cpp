#include "Scripting.h"
#include "Sphere.h"
#include "PathRenderer.h"
#include <stdio.h>
#include "lua.hpp"

struct Scene
{
	std::vector<Primitive*>* primitives;
	Camera camera;
	QualityDesc quality;
};

static int l_scene_set(lua_State* L)
{
	const char* op = lua_tostring(L, 1);
	lua_getglobal(L, "_scene");
	Scene* scene = (Scene*)lua_touserdata(L, -1);

	if (strcmp(op, "Camera") == 0)
	{
		Transform* trans = (Transform*)lua_touserdata(L, 2);
		float width = luaL_checknumber(L, 3);
		float height = luaL_checknumber(L, 4);
		float focalDist = luaL_checknumber(L, 5);
		Camera cam(*trans, width, height, focalDist);
		scene->camera = cam;
	}
	else if (strcmp(op, "Samples") == 0)
	{
		int samples = luaL_checknumber(L, 2);
		scene->quality.Samples = samples;
	}
	else if (strcmp(op, "LightSamples") == 0)
	{
		int light_samples = luaL_checknumber(L, 2);
		scene->quality.LightSamples = light_samples;
	}
	else if (strcmp(op, "GlossyReflectiveSamples") == 0)
	{
		int glossy_reflective_samples = luaL_checknumber(L, 2);
		scene->quality.GlossyReflectiveSamples = glossy_reflective_samples;
	}
	else if (strcmp(op, "Depth") == 0)
	{
		int depth = luaL_checknumber(L, 2);
		scene->quality.Depth = depth;
	}
	else if (strcmp(op, "PathEnableDirectLighting") == 0)
	{
		bool direct_lighting = lua_toboolean(L, 2);
		scene->quality.PathEnableDirectLighting = direct_lighting;
	}
	else if (strcmp(op, "PathEnableIndirectIllum") == 0)
	{
		bool indirect_illum = lua_toboolean(L, 2);
		scene->quality.PathEnableIndirectIllum = indirect_illum;
	}
	else
	{
		lua_pushstring(L, "unknown property");
		lua_error(L);
	}

	return 0;
}

static int l_scene_add(lua_State* L)
{
	const char* op = lua_tostring(L, 1);
	lua_getglobal(L, "_scene");
	Scene* scene = (Scene*)lua_touserdata(L, -1);

	if (strcmp(op, "Sphere") == 0)
	{
		Material* mat = (Material*)lua_touserdata(L, 2);
		float r = luaL_checknumber(L, 3);
		float x = luaL_checknumber(L, 4);
		float y = luaL_checknumber(L, 5);
		float z = luaL_checknumber(L, 6);
		float a = 1.f;
		float b = 1.f;
		float c = 1.f;
		if (lua_isnumber(L, 7) && lua_isnumber(L, 8) && lua_isnumber(L, 9))
		{
			a = luaL_checknumber(L, 7);
			b = luaL_checknumber(L, 8);
			c = luaL_checknumber(L, 9);
		}

		Transform* T = new Transform((Translate(Vector(x, y, z)))(Scale(a, b, c)));
		Transform* InvT = new Transform(Inverse(*T));
		Sphere* sphere = new Sphere(T, InvT, *mat, r);
		scene->primitives->push_back(sphere);
	}
	else if (strcmp(op, ""))
	{
		
	}
	else
	{
		lua_pushstring(L, "unknown primitive");
		lua_error(L);
	}

	return 0;
}

// TODO: transpose, mul, inverse
static int l_matrix_new(lua_State* L)
{
	if (!lua_istable(L, 1))
	{
		luaL_argerror(L, 1, "expected 4x4 matrix");
	}

	float mat[4][4];

	for (int r = 1; r <= 4; ++r)
	{
		lua_pushinteger(L, r);
		lua_gettable(L, 1);
		for (int c = 1; c <= 4; ++c)
		{
			lua_pushinteger(L, c);
			lua_gettable(L, 2);
			mat[r - 1][c - 1] = luaL_checknumber(L, 2);
		}
		
		// pop row
		lua_pop(L, 1);
	}

	Matrix4x4* matrix = new Matrix4x4(mat);
	lua_pushlightuserdata(L, matrix);

	return 1;
}

static Material* checkmaterial(lua_State* L)
{
	void* ud = luaL_checkudata(L, 1, "RayTracer.material");
	luaL_argcheck(L, ud != NULL, 1, "expected material");
	return (Material*)ud;
}

static void* getmaterialprop(lua_State* L, const char ** propname)
{
	Material* mat = checkmaterial(L);
	const char* prop = lua_tostring(L, 2);
	if (propname != NULL)
	{
		*propname = prop;
	}

	if (strcmp(prop, "Specular") == 0)
	{
		return &mat->Specular;
	}
	else if (strcmp(prop, "Diffuse") == 0)
	{
		return &mat->Diffuse;
	}
	else if (strcmp(prop, "Reflective") == 0)
	{
		return &mat->Reflective;
	}
	else if (strcmp(prop, "GlossyReflective") == 0)
	{
		return &mat->GlossyReflective;
	}
	else if (strcmp(prop, "Emissive") == 0)
	{
		return &mat->Emissive;
	}
	else if (strcmp(prop, "Refractive") == 0)
	{
		return &mat->Refractive;
	}
	else if (strcmp(prop, "RefrAbsorbance") == 0)
	{
		return &mat->RefrAbsorbance;
	}
	else if (strcmp(prop, "Color") == 0)
	{
		return &mat->Color;
	}
	else
	{
		luaL_argerror(L, 2, "nonexistent material property");
	}
}

static int l_material_new(lua_State* L)
{
	void* loc = lua_newuserdata(L, sizeof(Material));
	Material *mat = new(loc)Material();
	luaL_setmetatable(L, "RayTracer.material");

	return 1;
}

static int setmaterial(lua_State* L)
{
	const char* propname;
	void* prop = getmaterialprop(L, &propname);
	if (strcmp(propname, "Color") == 0)
	{
		RGB* newval = (RGB*)lua_touserdata(L, 3);
		*(RGB*)prop = *newval;
	}
	else
	{
		float newval = luaL_checknumber(L, 3);
		*(float*)prop = newval;
	}

	return 0;
}

static int getmaterial(lua_State* L)
{
	const char* propname;
	void* prop = getmaterialprop(L, &propname);
	if (strcmp(propname, "Color") == 0)
	{
		RGB* rgb = (RGB*)lua_newuserdata(L, sizeof(RGB));
		*rgb = *(RGB*)prop;
	}
	else
	{
		lua_pushnumber(L, *(float*)prop);
	}

	return 1;
}

static int l_vector_new(lua_State* L)
{
	float x = luaL_checknumber(L, 1);
	float y = luaL_checknumber(L, 2);
	float z = luaL_checknumber(L, 3);
	void* ud = lua_newuserdata(L, sizeof(Vector));
	Vector* vec = new(ud)Vector(x, y, z);

	return 1;
}

static int l_transform_translate(lua_State* L)
{
	Vector* vec = (Vector*)lua_touserdata(L, 1);
	Transform* trans = (Transform*)lua_newuserdata(L, sizeof(Transform));
	*trans = Translate(*vec);

	return 1;
}

static int l_transform_scale(lua_State* L)
{
	float x = lua_tonumber(L, 1);
	float y = lua_tonumber(L, 2);
	float z = lua_tonumber(L, 3);
	Transform* trans = (Transform*)lua_newuserdata(L, sizeof(Transform));
	*trans = Scale(x, y, z);

	return 1;
}

static int l_transform_rotatex(lua_State* L)
{
	float angle = lua_tonumber(L, 1);
	Transform* trans = (Transform*)lua_newuserdata(L, sizeof(Transform));
	*trans = RotateX(angle);

	return 1;
}

static int l_transform_rotatey(lua_State* L)
{
	float angle = lua_tonumber(L, 1);
	Transform* trans = (Transform*)lua_newuserdata(L, sizeof(Transform));
	*trans = RotateY(angle);

	return 1;
}

static int l_transform_rotatez(lua_State* L)
{
	float angle = lua_tonumber(L, 1);
	Transform* trans = (Transform*)lua_newuserdata(L, sizeof(Transform));
	*trans = RotateZ(angle);

	return 1;
}


static int l_transform_rotate(lua_State* L)
{
	float angle = lua_tonumber(L, 1);
	Vector* vec = (Vector*)lua_touserdata(L, 2);
	Transform* trans = (Transform*)lua_newuserdata(L, sizeof(Transform));
	*trans = Rotate(angle, *vec);

	return 1;
}

// TODO: LookAt (need points)

static int l_transform_mul(lua_State* L)
{
	Transform* lhs = (Transform*)lua_touserdata(L, 1);
	Transform* rhs = (Transform*)lua_touserdata(L, 2);
	Transform* result = (Transform*)lua_newuserdata(L, sizeof(Transform));
	*result = (*rhs)(*lhs);

	return 1;
}

static int l_rgb_new(lua_State* L)
{
	float r = lua_tonumber(L, 1);
	float g = lua_tonumber(L, 2);
	float b = lua_tonumber(L, 3);
	RGB* rgb = (RGB*)lua_newuserdata(L, sizeof(RGB));
	*rgb = RGB(r, g, b);

	return 1;
}

//static int l_rgb_set(lua_State* L)
//{
//
//}
//
//static int l_rgb_set(lua_State* L)
//{
//
//}

static const struct luaL_Reg matrixlib[]
{
	{ "new", l_matrix_new },
	{ NULL, NULL }
};

static const struct luaL_Reg materiallib_f[]
{
	{ "new", l_material_new },
	{ NULL, NULL }
};

static const struct luaL_Reg materiallib_m[]
{
	{ "__newindex", setmaterial },
	{ "__index", getmaterial },
	{ NULL, NULL }
};

static const struct luaL_Reg vectorlib[]
{
	{ "new", l_vector_new },
	{ NULL, NULL }
};

static const struct luaL_Reg transformlib_f[]
{
	{ "translate", l_transform_translate },
	{ "scale", l_transform_scale },
	{ "rotateX", l_transform_rotatex },
	{ "rotateY", l_transform_rotatey },
	{ "rotateZ", l_transform_rotatez },
	{ "rotate", l_transform_rotate },
	{ NULL, NULL }
};

static const struct luaL_Reg transformlib_m[]
{
	{ "__mul", l_transform_mul },
	{ NULL, NULL }
};

static const struct luaL_Reg rgblib[]
{
	{ "new", l_rgb_new },
	{ NULL, NULL }
};

static const struct luaL_Reg scenelib[]
{
	{ "set", l_scene_set },
	{ "add", l_scene_add },
	{ NULL, NULL }
};

int luaopen_matrix(lua_State* L)
{
	luaL_newlib(L, matrixlib);
	lua_setglobal(L, "matrix");
	return 0;
}

int luaopen_material(lua_State* L)
{
	luaL_newmetatable(L, "RayTracer.material");
	luaL_setfuncs(L, materiallib_m, 0);
	luaL_newlib(L, materiallib_f);
	lua_setglobal(L, "material");
	return 0;
}

int luaopen_vector(lua_State* L)
{
	luaL_newlib(L, vectorlib);
	lua_setglobal(L, "vector");
	return 0;
}

int luaopen_transform(lua_State* L)
{
	luaL_newmetatable(L, "RayTracer.transform");
	luaL_setfuncs(L, transformlib_m, 0);
	luaL_newlib(L, transformlib_f);
	lua_setglobal(L, "transform");
	return 0;
}

int luaopen_rgb(lua_State* L)
{
	luaL_newlib(L, rgblib);
	lua_setglobal(L, "rgb");
	return 0;
}

int luaopen_scene(lua_State* L)
{
	luaL_newlib(L, scenelib);
	lua_setglobal(L, "scene");
	return 0;
}

Renderer* init_renderer_from_script(char const * filename, std::string* error)
{
	lua_State* L;

	L = luaL_newstate();
	luaL_openlibs(L);
	luaopen_scene(L);
	luaopen_material(L);
	luaopen_vector(L);
	luaopen_transform(L);
	luaopen_rgb(L);

	Scene* scene = new Scene;
	scene->primitives = new std::vector<Primitive*>();

	lua_pushlightuserdata(L, scene);
	lua_setglobal(L, "_scene");

	int ret = luaL_dofile(L, filename);
	if (ret)
	{
		size_t len;
		const char* err = lua_tolstring(L, -1, &len);
		*error = std::string(err);
		lua_close(L);
		return NULL;
	}

	lua_close(L);

	return new PathRenderer(scene->primitives, scene->camera, scene->quality);
}