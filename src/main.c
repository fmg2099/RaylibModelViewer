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
#include <stdio.h>

struct Settings
{
	int resX;
	int resY;
	bool fullscreen; //no implementado aún
	bool VSync;
	bool MSAA;
	float CameraSpeed;
	float MouseSensitivity;
	bool MouseInverted;
};

void LoadSettings(struct Settings*);
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

	Camera3D cam={ 0 };
	cam.fovy = 60;
	cam.projection = CAMERA_PERSPECTIVE;
	cam.position = (Vector3){ 2,0,0};
	cam.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	Vector3 camDirection = { -1,0, 0};
	//Vector ortogonal a la direccion de la camara
	Vector3 camDirectionRight = { 0,0,0 };
	Quaternion camRot = QuaternionFromVector3ToVector3(camDirection, (Vector3) { -1, 0, 0 });

	//variables de control de camara
	float camSpeed = 5;

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
		if (GetMouseWheelMove() != 0)
		{
			cam.fovy -= 5*GetMouseWheelMove();
			cam.fovy = Clamp(cam.fovy, 1, 160);
		}

		//procesamiento de la camara
		camDirectionRight = Vector3Normalize(Vector3CrossProduct(camDirection, cam.up));

		Vector2 mousedelta = GetMouseDelta();
		//primero el pitch, es decir, rotar en el eje X
		float pitch = -mousedelta.y * 0.1 * GetFrameTime();
		float angle = Vector3Angle(camDirection, cam.up);
		//printf("pitch, angle: %f,%f\n", pitch,angle);
		if (angle - pitch < 0.1 || angle - pitch > 3.1)
		{
			pitch = 0;
		}
		camDirection = Vector3RotateByAxisAngle(camDirection, camDirectionRight, pitch);
		//luego el yaw, es decir, rotar en el eje Y
		float yaw = -mousedelta.x * 0.1 * GetFrameTime();
		cam.target = Vector3Add(cam.position, camDirection);
		camDirection = Vector3RotateByAxisAngle(camDirection, cam.up, yaw);
		cam.target = Vector3Add(cam.position, camDirection);

		// drawing
		BeginDrawing();
		//BeginTextureMode(rendertarget);
		ClearBackground(BLACK);
		//skybox
		DrawTexture(textureGradient, 0, 0, WHITE);
		BeginMode3D(cam);
		DrawSphere(camDirection, 0.05, RED);
		DrawGrid(15, 0.5f);

		DrawTexturedCube(texCrate, 1, 0, 0, 0);
		EndMode3D();

		//dibujar hud
		sprintf(labelbuffer, "Camara X:%f",cam.position.x);
		DrawText(labelbuffer, 20, 20, 14, YELLOW);
		sprintf(labelbuffer, "Camara Y:%f", cam.position.y);
		DrawText(labelbuffer, 20, 35, 14, YELLOW);
		sprintf(labelbuffer, "Camara Z:%f", cam.position.z);
		DrawText(labelbuffer, 20, 50, 14, YELLOW);

		sprintf(labelbuffer, "Camara Dir X:%f", camDirection.x);
		DrawText(labelbuffer, 20, 75, 14, RED);
		sprintf(labelbuffer, "Camara Dir Y:%f", camDirection.y);
		DrawText(labelbuffer, 20, 90, 14, GREEN);
		sprintf(labelbuffer, "Camara Dir Z:%f", camDirection.z);
		DrawText(labelbuffer, 20, 105, 14, BLUE);

		sprintf(labelbuffer, "CamRight X:%f", camDirectionRight.x);
		DrawText(labelbuffer, 20, 120, 14, RED);
		sprintf(labelbuffer, "CamRight Y:%f", camDirectionRight.y);
		DrawText(labelbuffer, 20, 135, 14, GREEN);
		sprintf(labelbuffer, "CamRight Z:%f", camDirectionRight.z);
		DrawText(labelbuffer, 20, 150, 14, BLUE);

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


void LoadSettings(struct Settings *cfg)
{
	//defaults
	cfg->resX = 1024;
	cfg->resY = 768;
	cfg->fullscreen = false;
	cfg->VSync = true;
	cfg->MSAA = false;
	cfg->CameraSpeed = 3;
	cfg->MouseSensitivity = 0.1;
	cfg->MouseInverted = false;

	char line[100] = { 0 };
	FILE* f = fopen("settings.ini", "r");
	if (f == NULL)
	{
		printf("No se pudo abrir el archivo de configuracion\n");
		return;
	}
	else
	{
		fgets(line, 16, f);
		printf("Leido: %s\n", line);
		char* token;
		char* copy = (char*)malloc(strlen(line) + 1);
		strcpy(copy, line);
		token = strtok(copy, "X");
		if (token != NULL)
			printf("Leido: %s\n", token);
		else
			printf("Token nulo\n");

		/*if (strcmp(token, "resX") == 0)
		{
			token = strtok(NULL, "=");
			cfg->resX = atoi(token);
		}*/
	}
}
