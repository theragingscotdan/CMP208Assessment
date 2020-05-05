#include "scene_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <maths/math_utils.h>
#include <input/sony_controller_input_manager.h>
#include <graphics/sprite.h>
#include "load_texture.h"
#include <input/keyboard.h>

const int noOfPlat = 15;
const int noOfCollect = 20;

SceneApp::SceneApp(gef::Platform& platform) :
	Application(platform),
	sprite_renderer_(NULL),
	renderer_3d_(NULL),
	primitive_builder_(NULL),
	input_manager_(NULL),
	font_(NULL),
	world_(NULL),
	//player_body_(NULL),
	button_icon_(NULL)
{
}

void SceneApp::Init()
{
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	InitFont();

	// initialise input manager
	input_manager_ = gef::InputManager::Create(platform_);
	
	for (int i = 0; i < noOfPlat; ++i)
	{
		platforms_[i] = new platforms;
	}

	for (int i = 0; i < noOfCollect; ++i)
	{
		collect_[i] = new Collectable;
	}
	//collect_ = new Collectable;
	

	initialise = new Initial;

	game_state = INIT;

	InitGameState();
}

void SceneApp::CleanUp()
{
	delete input_manager_;
	input_manager_ = NULL;

	CleanUpFont();

	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	delete initialise;
	initialise = NULL;

	for (int i = 0; i < noOfPlat; ++i)
	{
		delete platforms_[i];
		platforms_[i] = NULL;
	}

	for (int i = 0; i < noOfCollect; ++i)
	{
		delete collect_[i];
		collect_[i] = NULL;
	}
	/*delete collect_;
	collect_ = NULL;*/

	ReleaseGameState();
}

bool SceneApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;


	input_manager_->Update();

	//UpdateGameStateMachine(frame_time);

	switch (game_state)
	{
	case INIT:
		
		FrontendUpdate(frame_time);

		break;

	case LEVEL1:
		GameUpdate(frame_time);
		break;



	}


	return true;
}

void SceneApp::Render()
{
	//InitGameState();
	
	switch (game_state)
	{
	case INIT:
		
		FrontendRender();
		
	break;

	case LEVEL1:
	
		GameRender();
		
	break;
		
	}
	
}

void SceneApp::InitGround()
{
	// ground dimensions
	gef::Vector4 ground_half_dimensions(10.0f, 0.5f, 0.5f);
	ground_.SetScale(gef::Vector4(1.0, 1.0, 1.0));

	// setup the mesh for the ground
	ground_mesh_ = primitive_builder_->CreateBoxMesh(ground_half_dimensions);
	ground_.set_mesh(ground_mesh_);

	// create a physics body
	b2BodyDef body_def;
	body_def.type = b2_staticBody;
	body_def.position = b2Vec2(0.0f, -4.0f);

	ground_body_ = world_->CreateBody(&body_def);

	// create the shape
	b2PolygonShape shape;
	shape.SetAsBox(ground_half_dimensions.x(), ground_half_dimensions.y());

	// create the fixture
	b2FixtureDef fixture_def;
	fixture_def.shape = &shape;

	// create the fixture on the rigid body
	ground_body_->CreateFixture(&fixture_def);

	// update visuals from simulation data
	ground_.UpdateFromSimulation(ground_body_);
}

void SceneApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void SceneApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void SceneApp::DrawHUD()
{
	if(font_)
	{
		// display frame rate
	
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
		if (!game_state == INIT)
		{
			font_->RenderText(sprite_renderer_, gef::Vector4(0.0f, 0.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Score: %i", player_.GetScore());
			font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 0.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Lives: %i", player_.GetLives());
		}
	}
}

void SceneApp::SetupLights()
{
	// grab the data for the default shader used for rendering 3D geometry
	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();

	// set the ambient light
	default_shader_data.set_ambient_light_colour(gef::Colour(0.25f, 0.25f, 0.25f, 1.0f));

	// add a point light that is almost white, but with a blue tinge
	// the position of the light is set far away so it acts light a directional light
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-500.0f, 400.0f, 700.0f));
	default_shader_data.AddPointLight(default_point_light);
}

void SceneApp::UpdateSimulation(float frame_time)
{
	// update physics world
	float timeStep = 1.0f / 60.0f;

	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	world_->Step(timeStep, velocityIterations, positionIterations);

	// update object visuals from simulation data
	player_.UpdateFromSimulation(player_.GetBody());

	// don't have to update the ground visuals as it is static

	// collision detection
	// get the head of the contact list
	b2Contact* contact = world_->GetContactList();
	// get contact count
	int contact_count = world_->GetContactCount();

	for (int contact_num = 0; contact_num<contact_count; ++contact_num)
	{
		if (contact->IsTouching())
		{
		//	// get the colliding bodies
		//	b2Body* bodyA = contact->GetFixtureA()->GetBody();
		//	b2Body* bodyB = contact->GetFixtureB()->GetBody();

		//	// DO COLLISION RESPONSE HERE
		//	Player* player = NULL;

		//	GameObject* gameObjectA = NULL;
		//	GameObject* gameObjectB = NULL;

		//	gameObjectA = (GameObject*)bodyA->GetUserData();
		//	gameObjectB = (GameObject*)bodyB->GetUserData();

		//	if (gameObjectA)
		//	{
		//		if (gameObjectA->type() == PLAYER)
		//		{
		//			gef::DebugOut("%s \n", gameObjectB->GetType());
		//			if (gameObjectB->GetType() == COLLECTABLE)
		//			{

		//				//world_->DestroyBody(bodyB);
		//				//player_.AddScore(100);
		//				player->AddScore(100);

		//			}
		//		}
		//	}
		//	//		else if (gameObjectB->GetType() == PLATFORM)
		//	//		{

		//	//			//player_.AddScore(100);
		//	//			player->AddScore(100);

		//	//		}
		//	//		//player = (Player*)bodyA->GetUserData();
		//	//	}
		//	//}

		//	if (gameObjectB)
		//	{
		//		if (gameObjectB->type() == PLAYER)
		//		{
		//			//gef::DebugOut("working \n");
		//			//player_ = (Player*)bodyB->GetUserData();
		//			if (gameObjectA->GetType() == COLLECTABLE)
		//			{
		//				world_->DestroyBody(bodyA);
		//				player_.AddScore(100);
		//				player->AddScore(100);
		//			}
		//			else if (gameObjectA->GetType() == PLATFORM)
		//			{

		//				player_.AddScore(100);
		//				player->AddScore(100);

		//			}
		//		}
		//	}

			b2Body* bodyA = contact->GetFixtureA()->GetBody();
			b2Body* bodyB = contact->GetFixtureB()->GetBody();

			// DO COLLISION RESPONSE HERE
			Player* player = NULL;
			Collectable* collect = NULL;

			GameObject* gameObjectA = NULL;
			GameObject* gameObjectB = NULL;

			gameObjectA = (GameObject*)bodyA->GetUserData();
			gameObjectB = (GameObject*)bodyB->GetUserData();


			if (gameObjectA)
			{
				switch (gameObjectA->type())
				{
				case PLAYER:
					player = (Player*)bodyA->GetUserData();
					break;

				case COLLECTABLE:
					collect = (Collectable*)bodyA->GetUserData();
					break;
				}
			}

			if (gameObjectB)
			{
				switch (gameObjectB->type())
				{
				case PLAYER:
					player = (Player*)bodyB->GetUserData();
					break;

				case COLLECTABLE:
					collect = (Collectable*)bodyA->GetUserData();
					break;
				}
			}

			if (player && collect)
			{
				gef::DebugOut("Collide\n");
				player->AddScore(100);
				break;
			}
	
		}

		// Get next contact point
		contact = contact->GetNext();
	}
}

void SceneApp::FrontendInit()
{
	button_icon_ = CreateTextureFromPNG("playstation-cross-dark-icon.png", platform_);
}

void SceneApp::FrontendRelease()
{
	delete button_icon_;
	button_icon_ = NULL;
}

void SceneApp::FrontendUpdate(float frame_time)
{
	const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);
	const gef::Keyboard* keyboards = input_manager_->keyboard();

	if (controller)
	{
		if (controller->buttons_pressed() == gef_SONY_CTRL_CROSS || keyboards->IsKeyPressed(gef::Keyboard::KC_RETURN))
		{
			ReleaseGameState();
			game_state = LEVEL1;
			InitGameState();
		}
	}
}

void SceneApp::FrontendRender()
{
	sprite_renderer_->Begin();

	// render "PRESS" text
	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width()*0.5f, platform_.height()*0.5f - 56.0f, -0.99f),
		1.0f,
		0xffffffff,
		gef::TJ_CENTRE,
		"PRESS");

	// Render button icon
	gef::Sprite button;
	button.set_texture(button_icon_);
	button.set_position(gef::Vector4(platform_.width()*0.5f, platform_.height()*0.5f, -0.99f));
	button.set_height(32.0f);
	button.set_width(32.0f);
	sprite_renderer_->DrawSprite(button);


	// render "TO START" text
	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width()*0.5f, platform_.height()*0.5f + 32.0f, -0.99f),
		1.0f,
		0xffffffff,
		gef::TJ_CENTRE,
		"OR ENTER TO START");


	DrawHUD();
	sprite_renderer_->End();
}

void SceneApp::GameInit()
{
	// create the renderer for draw 3D geometry
	renderer_3d_ = gef::Renderer3D::Create(platform_);

	// initialise primitive builder to make create some 3D geometry easier
	primitive_builder_ = new PrimitiveBuilder(platform_);


	SetupLights();

	// initialise the physics world
	b2Vec2 gravity(0.0f, -9.81f);
	world_ = new b2World(gravity);

	InitGround();
	player_.InitPlayer(primitive_builder_, world_);
	//initialise->InitGround(primitive_builder_, world_, ground_mesh_, &ground_, ground_body_);
	platforms_[0]->InitPlatforms(primitive_builder_, world_, 4.5, 3.0);
	platforms_[1]->InitPlatforms(primitive_builder_, world_, 5.0, 5.0);
	platforms_[2]->InitPlatforms(primitive_builder_, world_, 1.25, 1.0);
	platforms_[3]->InitPlatforms(primitive_builder_, world_, -5.25, -2.0);
	platforms_[4]->InitPlatforms(primitive_builder_, world_, -1.25, -1.0);
	platforms_[5]->InitPlatforms(primitive_builder_, world_, -5.25, 7);
	platforms_[6]->InitPlatforms(primitive_builder_, world_, -1.0, 9.0);
	platforms_[7]->InitPlatforms(primitive_builder_, world_, 4.25, 10.5);	
	platforms_[8]->InitPlatforms(primitive_builder_, world_, -1.25, 12.3);	
	platforms_[9]->InitPlatforms(primitive_builder_, world_, -3.25, 14.5);
	platforms_[10]->InitPlatforms(primitive_builder_, world_, 6.25, 16.0);
	platforms_[11]->InitPlatforms(primitive_builder_, world_, -5.5, 17.9);
	platforms_[12]->InitPlatforms(primitive_builder_, world_, 0, 19.3);


	collect_[0]->InitCollectable(primitive_builder_, world_, 5.0, 3.5);
	//collect_->InitCollectable(primitive_builder_, world_, 5.0, 3.5);
}

void SceneApp::GameRelease()
{
	// destroying the physics world also destroys all the objects within it
	delete world_;
	world_ = NULL;

	delete ground_mesh_;
	ground_mesh_ = NULL;

	delete primitive_builder_;
	primitive_builder_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;

}

void SceneApp::GameUpdate(float frame_time)
{
	const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);
	const gef::Keyboard* keyboards = input_manager_->keyboard();
	
	if (keyboards->IsKeyPressed(gef::Keyboard::KC_SPACE))
	{
		player_.GetBody()->ApplyForceToCenter(b2Vec2(0, 100), true); // 350 did work with previous mass
		player_.SetState(Player::JUMP);
	}

	if (keyboards->IsKeyPressed(gef::Keyboard::KC_R))
	{
		// reload level
		//ReleaseGameState();
		game_state = INIT;
	}
	
	if (keyboards->IsKeyDown(gef::Keyboard::KC_RIGHT) || keyboards->IsKeyDown(gef::Keyboard::KC_D))
	{
		if (player_.GetBody()->GetLinearVelocity().x <= 5)
		{
	
		//player_body_->ApplyForceToCenter(b2Vec2(1.25, player_body_->GetLinearVelocity().y), true);
			player_.GetBody()->ApplyForceToCenter(b2Vec2(1.25, 0), true);
		
		}
		else if (player_.GetBody()->GetLinearVelocity().x > 5)
		{
			player_.GetBody()->SetLinearVelocity(b2Vec2(5.0, player_.GetBody()->GetLinearVelocity().y));
		}
	}

	if (keyboards->IsKeyDown(gef::Keyboard::KC_LEFT) || keyboards->IsKeyDown(gef::Keyboard::KC_A))
	{
		if (player_.GetBody()->GetLinearVelocity().x >= -5)
		{
			//player_body_->ApplyForceToCenter(b2Vec2(-1.25, player_body_->GetLinearVelocity().y), true);
			player_.GetBody()->ApplyForceToCenter(b2Vec2(-1.25, 0), true);
		}
		else if (player_.GetBody()->GetLinearVelocity().x < -5)
		{
			player_.GetBody()->SetLinearVelocity(b2Vec2(-5.0, player_.GetBody()->GetLinearVelocity().y));
		}
	}
	
	// to be used in future
	if (keyboards->IsKeyDown(gef::Keyboard::KC_E))
	{
		player_.SetState(Player::ATTACK);
	}

	// attempt to wrap the player
	// when the player reaches the edge of the screen, set the position to the opposite side of the screen
	if (player_.GetBody()->GetPosition().x < -8)
	{
		gef::DebugOut("position hit");//, " Mass 2 is %f %f \n", ground_body_->GetMass());
		//player_body_->
		b2Vec2 newPositionL(8.0f, player_.GetBody()->GetPosition().y);
		player_.GetBody()->SetTransform(newPositionL, player_.GetBody()->GetAngle());
	}

	if (player_.GetBody()->GetPosition().x > 8)
	{
		gef::DebugOut("position hit");//, " Mass 2 is %f %f \n", ground_body_->GetMass());
									  //player_body_->
		b2Vec2 newPositionR(-8.0f, player_.GetBody()->GetPosition().y);
		player_.GetBody()->SetTransform(newPositionR, player_.GetBody()->GetAngle());
	}

	player_.Update(frame_time);
	collect_[0]->UpdateFromSimulation(collect_[0]->GetBody());
	//collect_->UpdateFromSimulation(collect_->GetBody());
	UpdateSimulation(frame_time);
	

}

void SceneApp::GameRender()
{
	// setup camera

	// projection
	float fov = gef::DegToRad(45.0f);
	float aspect_ratio = (float)platform_.width() / (float)platform_.height();
	gef::Matrix44 projection_matrix;
	projection_matrix = platform_.PerspectiveProjectionFov(fov, aspect_ratio, 0.1f, 100.0f);
	renderer_3d_->set_projection_matrix(projection_matrix);

	float player_pos = player_.GetBody()->GetPosition().y;

	if (player_pos < ground_body_->GetPosition().y + 4)
	{
		player_pos = ground_body_->GetPosition().y + 4;
	}
	// view
	gef::Vector4 camera_eye(0.0f, player_pos, 10.0f);
	gef::Vector4 camera_lookat(0.0f, player_pos, 0.0f);
	gef::Vector4 camera_up(0.0f, 1.0f, 0.0f);
	gef::Matrix44 view_matrix;
	view_matrix.LookAt(camera_eye, camera_lookat, camera_up);
	renderer_3d_->set_view_matrix(view_matrix);


	// draw 3d geometry
	renderer_3d_->Begin();

	// draw ground
	renderer_3d_->DrawMesh(ground_);

	for (int i = 0; i < noOfPlat; ++i)
	{
		renderer_3d_->DrawMesh(*platforms_[i]);
	}

	for (int i = 0; i < noOfCollect; ++i)
	{
		renderer_3d_->DrawMesh(*collect_[i]);
	}
	//renderer_3d_->DrawMesh(*collect_);

	// draw player
	renderer_3d_->set_override_material(&primitive_builder_->red_material());
	renderer_3d_->DrawMesh(player_);
	renderer_3d_->set_override_material(NULL);

	renderer_3d_->End();

	// start drawing sprites, but don't clear the frame buffer
	sprite_renderer_->Begin(false);
	DrawHUD();
	sprite_renderer_->End();
}


void SceneApp::InitGameState()
{
	switch (game_state)
	{
	case INIT:
		FrontendInit();
		break;
	case LEVEL1:
		GameInit();
		break;

	}
}

void SceneApp::ReleaseGameState()
{
	switch (game_state)
	{
	case INIT:
		FrontendRelease();
		break;
	case LEVEL1:
		GameRelease();
		break;

	}
}