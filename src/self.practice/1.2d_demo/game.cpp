/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "text_renderer.h"
#include <learnopengl/filesystem.h>
#include <sstream>
#include <learnopengl/model.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Game-related State data
SpriteRenderer  *Renderer;
SpriteRenderer  *RendererMask;
TextRenderer      *Text;
Model *planet;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 800.0f));

static float remap(float inVal, float inMin, float inMax, float outMin, float outMax) {
	return outMin + ((inVal - inMin) / (inMax - inMin)) * (outMax - outMin);
}

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return out.str();
}

Game::Game(GLuint width, GLuint height)
	: 
	State(GAME_ACTIVE), 
	Keys(), 
	Width(width), 
	Height(height), 
	rotateTest(0)
{

}

Game::~Game()
{
	delete Renderer;
	delete RendererMask;
	delete Text;
	delete planet;
}

void Game::Init()
{
	// Load shaders
	ResourceManager::LoadShader("sprite.vs", "sprite.fs", nullptr, "sprite");
	ResourceManager::LoadShader("sprite2texture.vs", "sprite2texture.fs", nullptr, "sprite2texture");
	ResourceManager::LoadShader("10.2.instancing.vs", "10.2.instancing.fs", nullptr, "instancing");

	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("texture0", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::GetShader("sprite2texture").Use().SetInteger("texture0", 0);
	ResourceManager::GetShader("sprite2texture").SetInteger("texture1", 1);
	ResourceManager::GetShader("sprite2texture").SetMatrix4("projection", projection);
	// Load textures
	ResourceManager::LoadTexture("Images/Bg.png", GL_FALSE, "Bg");
	ResourceManager::LoadTexture("Images/SpeedBg.png", GL_TRUE, "SpeedBg");
	ResourceManager::LoadTexture("Images/SpeedPointer2.png", GL_TRUE, "pointer");
	ResourceManager::LoadTexture("Images/SpeedLight_CBmp.png", GL_TRUE, "SpeedLight");
	ResourceManager::LoadTexture("Images/SpeedMaskBmp.png", GL_TRUE, "SpeedMask");
	ResourceManager::LoadTexture("Images/RpmBg.png", GL_TRUE, "RpmBg");
	ResourceManager::LoadTexture("Images/DefaultTextureImage.png", GL_TRUE, "DefaultTextureImage");
	// load models
	//planet = new Model(FileSystem::getPath("resources/objects/planet/planet.obj"));
	planet = new Model(FileSystem::getPath("resources/OBJ/q.FBX"));
	// Set render-specific controls
	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	RendererMask = new SpriteRenderer(ResourceManager::GetShader("sprite2texture"));
	Text = new TextRenderer(1920, 720);
	Text->Load("fonts/OCRAEXT.TTF", 40);
}

void Game::Update(GLfloat dt)
{
	if (rotateTest < 100.0) {
		rotateTest += 0.005;
	}
	else {
		rotateTest = 0.0;
	}
}


void Game::ProcessInput(GLfloat dt)
{

}

void Game::SetCameraView(Camera_Movement movement, GLfloat dt)
{
	camera.ProcessKeyboard(movement, dt);
}


void Game::Render()
{
	Renderer->DrawSprite(ResourceManager::GetTexture("Bg"), glm::vec2(0, 0), glm::vec2(1920, 720), 0, glm::vec3(1.0f, 1.0f, 1.0f));
	Renderer->DrawSprite(ResourceManager::GetTexture("SpeedBg"), glm::vec2(74, 56), glm::vec2(ResourceManager::GetTexture("SpeedBg").Width, ResourceManager::GetTexture("SpeedBg").Height), 0, glm::vec3(1.0f, 1.0f, 1.0f));
	ResourceManager::GetShader("sprite2texture").Use().SetFloat("angle", remap(rotateTest, 0, 100, 115, -155));
	RendererMask->DrawSprite2Texture(ResourceManager::GetTexture("SpeedLight"), ResourceManager::GetTexture("SpeedMask"), glm::vec2(74, 56), glm::vec2(ResourceManager::GetTexture("SpeedLight").Width, ResourceManager::GetTexture("SpeedLight").Height), 0, glm::vec3(1.0f, 1.0f, 1.0f));
	Renderer->DrawSprite(ResourceManager::GetTexture("pointer"), glm::vec2(76, 56), glm::vec2(ResourceManager::GetTexture("pointer").Width, ResourceManager::GetTexture("pointer").Height), remap(rotateTest, 0, 100, -300, -30), glm::vec3(1.0f, 1.0f, 1.0f));
	Renderer->DrawSprite(ResourceManager::GetTexture("RpmBg"), glm::vec2(1251, 56), glm::vec2(ResourceManager::GetTexture("RpmBg").Width, ResourceManager::GetTexture("RpmBg").Height), 0, glm::vec3(1.0f, 1.0f, 1.0f));
	Renderer->DrawSprite(ResourceManager::GetTexture("pointer"), glm::vec2(1246, 58), glm::vec2(ResourceManager::GetTexture("pointer").Width, ResourceManager::GetTexture("pointer").Height), remap(rotateTest, 0, 100, 121, -151), glm::vec3(1.0f, 1.0f, 1.0f));
	//Renderer->DrawSprite(ResourceManager::GetTexture("DefaultTextureImage"), glm::vec2(960, 360), glm::vec2(ResourceManager::GetTexture("DefaultTextureImage").Width, ResourceManager::GetTexture("DefaultTextureImage").Height), rotateTest, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0, 0.0));
	Text->RenderText(to_string_with_precision<float>(remap(rotateTest, 0, 100, 0, 240), 0), 360.0f, this->Height / 2.0f, 1.0f);

	// configure transformation matrices
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), this->Width / (float)this->Height, 0.1f, 1000.0f);
	glm::mat4 view = camera.GetViewMatrix();;
	ResourceManager::GetShader("instancing").Use();
	ResourceManager::GetShader("instancing").SetMatrix4("projection", projection);
	ResourceManager::GetShader("instancing").SetMatrix4("view", view);

	// draw planet
	glm::mat4 model = glm::mat4(1.0f);
	//Î»ÒÆ
	model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
	static float rotate = 0;
	rotate += 0.03;
	//Ðý×ª
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 1.0f, 0.0f)); // then rotate
	//Ëõ·Å
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	ResourceManager::GetShader("instancing").Use().SetMatrix4("model", model);
	planet->Draw(ResourceManager::GetShader("instancing"));

}