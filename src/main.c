/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h" 
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

void DrawBasicSphere(Color color);
void DrawAxes(float s);
void DrawTexturedCube(Texture tex, float size, float x, float y, float z);

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Model Viewer");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture texCrate= LoadTexture("texture_13.png");

	//"Skybox" super simple
	Image imgGradient = GenImageGradientLinear(GetScreenWidth(), GetScreenHeight(), 0, SKYBLUE, BEIGE);
	Texture textureGradient = LoadTextureFromImage(imgGradient);

	Matrix customProjection = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
	};

	Camera3D cam={ 0 };
	cam.fovy = 60;
	cam.projection = CAMERA_PERSPECTIVE;
	cam.position = (Vector3){ 0,1, 5 };
	cam.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	Vector3 camDirection = { 0,0,-1 };
	//Vector ortogonal a la direccion de la camara
	Vector3 camDirectionRight = { 1,0,0 };

	//variables de control de camara
	float camSpeed = 1;


	float planetRot = 0;
	float moonRot = 0;

	//labels para el hud
	char* labelbuffer[64];

	DisableCursor();

	// Create a RenderTexture2D to be used for render to texture 
	RenderTexture2D rendertarget = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	Shader ppShader = LoadShader(0, "bloom.fs"); 

	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		camDirectionRight = Vector3Normalize(Vector3CrossProduct(camDirection, cam.up));
		//Procesamiento de los inputs
		if (IsKeyPressed(KEY_R))
		{
			cam.position = (Vector3){ 0,0,0 };
		}
		if (IsKeyDown(KEY_W))
		{
			cam.position = Vector3Add(cam.position, Vector3Scale(camDirection, camSpeed * GetFrameTime()));
		}
		if (IsKeyDown(KEY_S))
		{
			cam.position= Vector3Add(cam.position, Vector3Scale(camDirection, -camSpeed * GetFrameTime()));
		}
		if (IsKeyDown(KEY_A))
		{
			cam.position = Vector3Add(cam.position, Vector3Scale(camDirectionRight, -camSpeed * GetFrameTime()));
		}
		if (IsKeyDown(KEY_D))
		{
			cam.position = Vector3Add(cam.position, Vector3Scale(camDirectionRight, camSpeed * GetFrameTime()));
		}
		if (IsKeyDown(KEY_Q))
		{
			cam.position.y += 1.0f * GetFrameTime();
		}
		if (IsKeyDown(KEY_E))
		{
			cam.position.y += -1.0f * GetFrameTime();
		}
		Vector2 mousedelta = GetMouseDelta();
		Matrix Mrot = MatrixRotateXYZ((Vector3) { mousedelta.y * -0.005f, mousedelta.x * -0.005f, 0 });
		camDirection = Vector3Transform(camDirection, Mrot);

		cam.target = Vector3Add(cam.position, camDirection);
			
		// drawing
		BeginDrawing();
		//BeginTextureMode(rendertarget);
		ClearBackground(BLACK);
		//skybox

		DrawTexture(textureGradient, 0, 0, WHITE);

		BeginMode3D(cam);
		DrawGrid(15, 0.5f);

		DrawAxes(1.0f);
		//DrawCube((Vector3){ 0,0,0 }, 1, 1, 1, RED);

		/*rlBegin(RL_TRIANGLES);
		rlColor4ub(120, 120, 240, 255);
		rlVertex3f(0, 0, 0);
		rlVertex3f(0, 3, 0);
		rlVertex3f(1, 0, 0);
		rlEnd();*/


		//planetRot += 6 * GetFrameTime();
		//moonRot += 360 * GetFrameTime();
		//rlPushMatrix();
		//	rlScalef(0.4f, 0.4f, 0.4f);
		//	DrawBasicSphere(GOLD);
		//rlPopMatrix();
		//rlPushMatrix();
		//	rlRotatef(planetRot, 0,1,0);
		//	rlTranslatef(1, 0, 0);
		//	rlScalef(0.1f, 0.1f, 0.1f);
		//	DrawBasicSphere(BLUE);
		//	rlPushMatrix();
		//		rlRotatef(moonRot, 0, 1, 0);
		//		rlTranslatef(2, 0, 0);
		//		rlScalef(0.3f, 0.3f, 0.3f);
		//		DrawBasicSphere(GRAY);
		//	rlPopMatrix();
		//rlPopMatrix();

		rlSetTexture(texCrate.id);
		rlBegin(RL_QUADS);
		rlColor4ub(255, 255, 255, 255);
		rlTexCoord2f(0, 1);
		rlVertex3f(0, 0, 0);
		rlTexCoord2f(1, 1);
		rlVertex3f(1, 0, 0);
		rlTexCoord2f(1, 0);
		rlVertex3f(1, 1, 0);
		rlTexCoord2f(0, 0);
		rlVertex3f(0, 1 , 0);
		rlEnd();
		DrawTexturedCube(texCrate, 1, 0, 0, 0);
		EndMode3D();

		//dibujar hud
		sprintf(labelbuffer, "Camara X:%f",cam.position.x);
		DrawText(labelbuffer, 20, 20, 12, YELLOW);
		sprintf(labelbuffer, "Camara Y:%f", cam.position.y);
		DrawText(labelbuffer, 20, 32, 12, YELLOW);
		sprintf(labelbuffer, "Camara Z:%f", cam.position.z);
		DrawText(labelbuffer, 20, 44, 12, YELLOW);

		sprintf(labelbuffer, "FPS: %.1f", 1 / GetFrameTime());
		DrawText(labelbuffer, GetScreenWidth()-100, 20,16, DARKGREEN);

		EndDrawing();
		//EndTextureMode();
		//BeginDrawing();
		//ClearBackground(WHITE);
		//BeginShaderMode(ppShader);
		//DrawTextureRec(rendertarget.texture, (Rectangle) { 0, 0, (float)rendertarget.texture.width, -(float)rendertarget.texture.height }, (Vector2) { 0, 0 }, WHITE);
		//EndShaderMode();
		//EndDrawing();
	}

	UnloadTexture(texCrate);
	CloseWindow();
	return 0;
}

void DrawAxes(float scale)
{
	float punta = scale / 10;
	float s = scale;
	rlBegin(RL_LINES);
	//eje X
	rlColor4ub(255, 0, 0, 255);
	rlVertex3f(0, 0, 0);
	rlVertex3f(s,0, 0);

	rlVertex3f(s, 0, 0);
	rlVertex3f(s-punta, 0, punta);

	rlVertex3f(s, 0, 0);
	rlVertex3f(s - punta, 0, -punta);
	//eje Y
	rlColor4ub(0, 255, 0, 255);
	rlVertex3f(0, 0, 0);
	rlVertex3f(0, s, 0);

	rlVertex3f(0,s, 0);
	rlVertex3f(0,s-punta,punta);

	rlVertex3f(0, s, 0);
	rlVertex3f(0, s - punta, -punta);
	//eje Z
	rlColor4ub(0, 0, 255, 255);
	rlVertex3f(0, 0, 0);
	rlVertex3f(0, 0, s);

	rlVertex3f(0, 0, s);
	rlVertex3f(punta, 0, s - punta);

	rlVertex3f(0, 0, s);
	rlVertex3f(-punta, 0, s - punta);
	rlEnd();
}

void DrawTexturedCube(Texture tex, float size, float x, float y, float z)
{
	rlPushMatrix();
	rlTranslatef(x, y, z);
	rlSetTexture(tex.id);
	rlBegin(RL_QUADS);
	//cara 1
	rlColor4ub(255, 255, 255, 255);
	rlTexCoord2f(0,1);
	rlVertex3f(-size / 2, -size / 2, size / 2);
	rlTexCoord2f(1, 1);
	rlVertex3f(size / 2, -size / 2, size / 2);
	rlTexCoord2f(1, 0);
	rlVertex3f(size / 2, size / 2, size / 2);
	rlTexCoord2f(0, 0);
	rlVertex3f(-size / 2, size / 2, size / 2);
	//cara 2
	rlTexCoord2f(0, 1);
	rlVertex3f(size / 2, -size / 2, size / 2);
	rlTexCoord2f(1, 1);
	rlVertex3f(size / 2, -size / 2, -size / 2);
	rlTexCoord2f(1, 0);
	rlVertex3f(size / 2, size / 2, -size / 2);
	rlTexCoord2f(0, 0);
	rlVertex3f(size / 2, size / 2, size / 2);
	///cara3
	rlTexCoord2f(0,1);
	rlVertex3f(size / 2, -size / 2, -size / 2);
	rlTexCoord2f(1, 1);
	rlVertex3f(-size / 2, -size / 2, -size / 2);
	rlTexCoord2f(1, 0);
	rlVertex3f(-size / 2, size / 2, -size / 2);
	rlTexCoord2f(0, 0);
	rlVertex3f(size / 2, size / 2, -size / 2);
	//cara 4
	rlTexCoord2f(0, 1);
	rlVertex3f(-size / 2, -size / 2, -size / 2);
	rlTexCoord2f(1, 1);
	rlVertex3f(-size / 2, -size / 2, size / 2);
	rlTexCoord2f(1, 0);
	rlVertex3f(-size / 2, size / 2, size / 2);
	rlTexCoord2f(0, 0);
	rlVertex3f(-size / 2, size / 2, -size / 2);
	//tapa sup
	rlTexCoord2f(0, 1);
	rlVertex3f(-size / 2, size / 2, size / 2);
	rlTexCoord2f(1, 1);
	rlVertex3f(size / 2, size / 2, size / 2);
	rlTexCoord2f(1, 0);
	rlVertex3f(size / 2, size / 2, -size / 2);
	rlTexCoord2f(0, 0);
	rlVertex3f(-size / 2, size / 2, -size / 2);
	//tapa inf
	rlTexCoord2f(0, 1);
	rlVertex3f(-size / 2, -size / 2, -size / 2);
	rlTexCoord2f(1, 1);
	rlVertex3f(size / 2, -size / 2, -size / 2);
	rlTexCoord2f(1, 0);
	rlVertex3f(size / 2, -size / 2, size / 2);
	rlTexCoord2f(0, 0);
	rlVertex3f(-size / 2, -size / 2, size / 2);
	rlEnd();
	
	
	rlEnd();
	rlPopMatrix();
}

void DrawBasicSphere(Color color)
{
	int rings = 16;
	int slices = 32;
	rlCheckRenderBatchLimit((rings + 2) * slices * 6);
	rlBegin(RL_TRIANGLES);
	rlColor4ub(color.r, color.g, color.b, color.a);
	for (int i = 0; i < (rings + 2); i++)
	{
		for (int j = 0; j < slices; j++)
		{
			rlVertex3f(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * i)) * sinf(DEG2RAD * (j * 360 / slices)),
				sinf(DEG2RAD * (270 + (180 / (rings + 1)) * i)),
				cosf(DEG2RAD * (270 + (180 / (rings + 1)) * i)) * cosf(DEG2RAD * (j * 360 / slices)));
			rlVertex3f(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * sinf(DEG2RAD * ((j + 1) * 360 / slices)),
				sinf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))),
				cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * cosf(DEG2RAD * ((j + 1) * 360 / slices)));
			rlVertex3f(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * sinf(DEG2RAD * (j * 360 / slices)),
				sinf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))),
				cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * cosf(DEG2RAD * (j * 360 / slices)));

			rlVertex3f(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * i)) * sinf(DEG2RAD * (j * 360 / slices)),
				sinf(DEG2RAD * (270 + (180 / (rings + 1)) * i)),
				cosf(DEG2RAD * (270 + (180 / (rings + 1)) * i)) * cosf(DEG2RAD * (j * 360 / slices)));
			rlVertex3f(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i))) * sinf(DEG2RAD * ((j + 1) * 360 / slices)),
				sinf(DEG2RAD * (270 + (180 / (rings + 1)) * (i))),
				cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i))) * cosf(DEG2RAD * ((j + 1) * 360 / slices)));
			rlVertex3f(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * sinf(DEG2RAD * ((j + 1) * 360 / slices)),
				sinf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))),
				cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * cosf(DEG2RAD * ((j + 1) * 360 / slices)));
		}
	}
}