#include <Windows.h>

#include "Globals.h"
#include "Application.h"
#include "Module.h"
#include "3DShapes.h"
#include "Importer.h"
//#include "glew/include/glew.h"
//#include "SDL\include\SDL_opengl.h"
//#include <gl/GL.h>
//#include <gl/GLU.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glew/libx86/glew32.lib")


Renderer3D::Renderer3D(bool start_enabled) : Module(start_enabled), context()
{
}

// Destructor
Renderer3D::~Renderer3D()
{}

// Called before render is available
bool Renderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (ret == true)
	{
		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			//LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			/*LOG("Error initializing OpenGL! %s\n", gluErrorString(error));*/
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			//LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	//Initialize glew

	GLenum glew_init_return = glewInit();									// glew must be initialized after an OpenGL rendering context has been created.

	if (glew_init_return != GLEW_NO_ERROR)
	{
		LOG("GLEW could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	return ret;
}

bool Renderer3D::Start()
{
	Importer::Init();

	//Initialize DevIL
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	//CreateCheckerTexture(); //not working
	ImportMesh("Assets/models/BakerHouse.FBX");

	//Textures provisional
	TextureData* tmp = Importer::LoadTexture("Assets/models/Baker_house.png");
	houseID = tmp->ID;


	return true;
}


// PreUpdate: clear buffer
update_status Renderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);

	glLoadMatrixf(App->camera->GetRawViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();


	//RENDER OPTIONS
	if (App->gui->wireframe == true)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (App->gui->gl_cull_face == true)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

	if (App->gui->gl_depth == true)
		glEnable(GL_DEPTH);
	else
		glDisable(GL_DEPTH);
	
	if (App->gui->gl_light == true)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	if (App->gui->gl_color_mat == true)
		glEnable(GL_COLOR_MATERIAL);
	else
		glDisable(GL_COLOR_MATERIAL);
	

	//Call Draw
	std::vector<MeshData*>::iterator item = mesh_container.begin();

	for (; item != mesh_container.end(); ++item)
	{
		DrawMesh((*item));

		if(App->gui->draw_normals_dir)
			DrawNormalDir((*item));
	}

	//CreateDirectCube();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status Renderer3D::PostUpdate(float dt)
{

	DropMesh();
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool Renderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	Importer::CleanUp();
	SDL_GL_DeleteContext(context);

	return true;
}


void Renderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Renderer3D::ImportMesh(char* file_path)
{
	Importer::LoadMesh(file_path, mesh_container);
	std::vector<MeshData*>::iterator item = mesh_container.begin();

	for (; item != mesh_container.end(); ++item)
	{
		MeshBuffer((*item));
	}
}

//Mesh

void Renderer3D::MeshBuffer(MeshData* currentmesh)
{
	glGenBuffers(1, (GLuint*)& currentmesh->buffersId[MeshData::vertex]);
	glBindBuffer(GL_ARRAY_BUFFER, currentmesh->buffersId[MeshData::vertex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * currentmesh->buffersLength[MeshData::vertex] * 3, currentmesh->vertices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)& currentmesh->buffersId[MeshData::index]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentmesh->buffersId[MeshData::index]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * currentmesh->buffersLength[MeshData::index], currentmesh->indices, GL_STATIC_DRAW);

	if (currentmesh->normals != nullptr)
	{
		glGenBuffers(1, (GLuint*)& currentmesh->buffersId[MeshData::normal]);
		glBindBuffer(GL_ARRAY_BUFFER, currentmesh->buffersId[MeshData::normal]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * currentmesh->buffersLength[MeshData::normal], currentmesh->normals, GL_STATIC_DRAW);
	}	

	if (currentmesh->texture_coord != nullptr)
	{
		glGenBuffers(1, (GLuint*)& currentmesh->buffersId[MeshData::texture]);
		glBindBuffer(GL_ARRAY_BUFFER, currentmesh->buffersId[MeshData::texture]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * currentmesh->buffersLength[MeshData::texture] * 2, currentmesh->texture_coord, GL_STATIC_DRAW);
	}
}
void Renderer3D::DrawNormalDir(MeshData* currentmesh)
{
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);

		for (size_t i = 0; i < currentmesh->buffersLength[MeshData::vertex] * 3; i += 3)
		{
			glVertex3f(currentmesh->vertices[i] , currentmesh->vertices[i + 1], currentmesh->vertices[i + 2]); // X vertex, Y vertex and Z vertex
			glVertex3f(currentmesh->vertices[i] + currentmesh->normals[i], currentmesh->vertices[i + 1] + currentmesh->normals[i+1], currentmesh->vertices[i + 2] + currentmesh->normals[i+2]);  
		}
	glEnd();
}
void Renderer3D::DrawMesh(MeshData* mymesh)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, houseID);

	glBindBuffer(GL_ARRAY_BUFFER, mymesh->buffersId[MeshData::texture]);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, mymesh->buffersId[MeshData::vertex]);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, mymesh->buffersId[MeshData::normal]);
	glNormalPointer(GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mymesh->buffersId[MeshData::index]);

	glDrawElements(GL_TRIANGLES, mymesh->buffersLength[MeshData::index], GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer3D::DropMesh()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_DROPFILE)
		{
			ImportMesh(event.drop.file);

			LOG("Mesh imported");
		}
	}
}
void Renderer3D::CreateCheckerTexture()
{

	GLubyte checkerImage[64][64][4];
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &checkerID);
	glBindTexture(GL_TEXTURE_2D, checkerID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//SHAPES
void Renderer3D::CreateDirectCube()
{
	uint my_indices = 0;
	glGenBuffers(1, (GLuint*) & (my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 36, indices, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	glBindTexture(GL_TEXTURE_2D, checkerID);

	glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

