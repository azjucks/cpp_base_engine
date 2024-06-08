/*

#include <scene.h>

#include <raymath.h>

#include <draw_primitives.h>

#include <structs.h>

#include <intersection.h>

#include <collision.h>

#include <TreeNode.h>;

Scene::Scene()
{
	position = { 0, 0, 0 };
	rotSpeed = 2.0f;

	yaw = 0.0f;
	pitch = 0.0f;
	roll = 0.0f;
	scale = 1.0f;

	balls.push_back(Ball({ 5, 18, 5 }, 1.2f, 10.0f, { 0, 0, 0 }));

	place_boxes();

	collisionSound = LoadSound("resources/sound.wav");

	// GENERATE OCTREE
	octree = TreeNode({ 0, 0, 0 }, 35);
	for (auto& box : boxes)
	{
		octree.colliders.push_back(&box);
	}
	octree.generate_octree(5);
}

Scene::~Scene()
{
	StopSound(collisionSound);
	UnloadSound(collisionSound);
}

// ===========

void Scene::updateInputs(float speed)
{
	if (IsKeyDown(KEY_LEFT)) roll += speed;
	else if (IsKeyDown(KEY_RIGHT)) roll -= speed;

	if (IsKeyDown(KEY_DOWN)) pitch += speed;
	else if (IsKeyDown(KEY_UP)) pitch -= speed;

	if (IsKeyDown(KEY_KP_3)) yaw += speed;
	else if (IsKeyDown(KEY_KP_1)) yaw -= speed;

	if (IsKeyDown(KEY_KP_ADD)) scale += speed;
	else if (IsKeyDown(KEY_KP_SUBTRACT)) scale -= speed;

	if (IsKeyDown(KEY_KP_6)) position.x += speed;
	else if (IsKeyDown(KEY_KP_4)) position.x -= speed;

	if (IsKeyDown(KEY_LEFT_SHIFT)) position.y += speed;
	else if (IsKeyDown(KEY_LEFT_CONTROL)) position.y -= speed;

	if (IsKeyDown(KEY_KP_2)) position.z += speed;
	else if (IsKeyDown(KEY_KP_8)) position.z -= speed;

	if (IsKeyPressed(KEY_PAGE_UP)) currentScene += 1;
	else if (IsKeyPressed(KEY_PAGE_DOWN)) currentScene -= 1;
	if (currentScene < 0) currentScene = sceneNb - 1;
	if (currentScene >= sceneNb) currentScene = 0;

	if (IsKeyPressed(KEY_ENTER))
		wireframe = !wireframe;
}

void Scene::update(float deltaTime)
{
	//drawAxis();

	updateInputs(deltaTime * rotSpeed);

	switch (currentScene)
	{
	case 0:
		ball_physics(deltaTime);
		break;
	case 1:
		sphere_segment();
		break;
	case 2:
		cylinder_segment();
		break;
	case 3:
		quad_segment();
		break;
	case 4:
		box_segment();
		break;
	case 5:
		capsule_segment();
		break;
	case 6:
		plane_segment();
		break;
	default:
		break;
	}
}

// ===========

void Scene::plane_segment()
{
	// NORMAL VECTOR
	// -------------
	static Plane plane({ 0, 1, 0 }, 0.0f);

	plane.normal = Vector3RotateByQuaternion({ 0, 1, 0 }, QuaternionFromEuler(roll, pitch, yaw));
	plane.distance = length(position);

	drawPlane(plane, scale, WHITE);

	// Normal plane
	drawVector(plane.normal * plane.distance, plane.normal, PURPLE);

	// SEGMENT
	// -------
	vec3 a(0.0f, 3.5f, 0.0f);
	vec3 b(2.2f, 0.0f, 1.1f);
	DrawLine3D(a.toRlVec(), b.toRlVec(), PINK);

	// INTERSECTION POINT
	// ------------------
	vec3 interPt;
	vec3 interNormal;

	if (intersection_segment_plane(a, b, plane, interPt, interNormal))
	{
		DrawSphere(interPt.toRlVec(), 0.1f, RED);
		drawVector(interPt, interNormal, RED);
	}
}

void Scene::sphere_segment()
{
	// SPHERE
	// ------
	static Sphere sphere;
	sphere.center = position;
	sphere.radius = scale;

	if (wireframe)
		drawSphereWireframe(sphere, 20, BLACK);
	else
		drawSphere(sphere, 20, RAYWHITE);

	//DrawSphere(sphere.center.toRlVec(), sphere.radius, RAYWHITE);

	// SEGMENT
	// -------
	vec3 a = { 0.0f, 3.5f, 0.0f };
	vec3 b = { 2.2f, 0.0f, 0.0f };
	DrawLine3D(a.toRlVec(), b.toRlVec(), PINK);

	vec3 interPt;
	vec3 interNormal;

	if (intersection_segment_sphere(a, b, sphere, interPt, interNormal))
	{
		DrawSphere(interPt.toRlVec(), 0.1f, RED);
		drawVector(interPt, interNormal, RED);
	}
}

void Scene::cylinder_segment()
{
	// CYLINDRE
	// --------
	Cylinder cyl = Cylinder(QuaternionFromEuler(roll, pitch, yaw), position, scale, scale);

	DrawSphere(cyl.ptA.toRlVec(), 0.1f, PINK);
	DrawSphere(cyl.ptB.toRlVec(), 0.1f, PINK);

	// SEGMENT
	// -------
	vec3 a = { 0.0f, 3.5f, 0.0f };
	vec3 b = { 2.2f, 0.0f, 0.0f };
	DrawLine3D(a.toRlVec(), b.toRlVec(), PINK);

	vec3 interPt;
	vec3 interNormal;

	if (intersection_segment_cylinder(a, b, cyl, interPt, interNormal))
	{
		if (wireframe)
			drawCylinderWireframe(cyl, 21, BLACK, true);
		else
			drawCylinder(cyl, 21, PURPLE, true);

		DrawSphere(interPt.toRlVec(), 0.1f, RED);
		drawVector(interPt, interNormal, RED);
	}
	else
	{
		if (wireframe)
			drawCylinderWireframe(cyl, 21, BLACK, true);
		else
			drawCylinder(cyl, 21, YELLOW, true);
	}
}

void Scene::quad_segment()
{
	static Quad quad;
	quad.quaternion = QuaternionFromEuler(roll, pitch, yaw);
	quad.position = position;
	quad.extensions = { scale, scale, scale };

	if (wireframe)
		drawQuadWireframe(quad, BLACK);
	else
		drawQuad(quad, RAYWHITE);

	// SEGMENT
	// -------
	vec3 a = { 0.0f, 3.5f, 0.0f };
	vec3 b = { 2.2f, 0.0f, 0.0f };
	DrawLine3D(a.toRlVec(), b.toRlVec(), PINK);

	vec3 interPt;
	vec3 interNormal;
	if (intersection_segment_quad(a, b, quad, interPt, interNormal))
	{
		DrawSphere(interPt.toRlVec(), 0.1f, RED);
		drawVector(interPt, interNormal, RED);
	}
}

void Scene::box_segment()
{
	static Box box;
	box.quaternion = QuaternionFromEuler(roll, pitch, yaw);
	box.position = position;
	box.extensions = vec3{ 3.0f, 3.0f, 2.0f };
	box.roundness = scale;

	if (wireframe)
		drawBoxWireframe(box, BLACK);
	else
		drawBox(box, WHITE);

	// SEGMENT
	// -------
	vec3 a = { 0.0f, 5.0f, 0.0f };
	vec3 b = { 4.2f, 0.0f, 0.0f };
	DrawLine3D(a.toRlVec(), b.toRlVec(), PINK);

	vec3 interPt;
	vec3 interNormal;
	if (intersection_segment_box(a, b, box, interPt, interNormal))
	{
		DrawSphere(interPt.toRlVec(), 0.1f, RED);
		drawVector(interPt, interNormal, RED);
	}
}

void Scene::capsule_segment()
{
	Capsule caps(QuaternionFromEuler(roll, pitch, yaw), position, 2.0f * scale, 1.0f * scale);

	if (wireframe)
		drawCapsuleWireframe(caps, 20, BLACK);
	else
		drawCapsule(caps, 20, RAYWHITE);

	// SEGMENT
	// -------
	vec3 a = { 0.0f, 3.5f, 0.0f };
	vec3 b = { 2.2f, 0.0f, 0.0f };
	DrawLine3D(a.toRlVec(), b.toRlVec(), PINK);

	vec3 interPt;
	vec3 interNormal;
	if (intersection_segment_capsule(a, b, caps, interPt, interNormal))
	{
		DrawSphere(interPt.toRlVec(), 0.1f, RED);
		drawVector(interPt, interNormal, RED);
	}
}

// SCENE RENDU
// ===========

void Scene::place_boxes()
{
	srand(0);

	boxes.push_back(Box(QuaternionIdentity(), { 0, 0, 0 }, { 22.f, 4.0f, 22.f }, 0.0f));
	boxes.push_back(Box(QuaternionIdentity(), { 22, 10, 0 }, { 4.0f, 14.0f, 20.f }, 0.0f));
	boxes.push_back(Box(QuaternionIdentity(), { -22, 10, 0 }, { 4.0f, 14.0f, 20.f }, 0.0f));
	boxes.push_back(Box(QuaternionIdentity(), { 0, 10, 22 }, { 20.0f, 14.0f, 4.0f }, 0.0f));
	boxes.push_back(Box(QuaternionIdentity(), { 0, 10, -22 }, { 20.0f, 14.0f, 4.0f }, 0.0f));

	float offset = 7.5f;
	bool rounded = true;

	for (int i = -1; i <= 2; i++)
	{
		for (int j = -1; j <= 2; j++)
		{
			float roundness = 0.0f;

			if (!rounded)
			{
				roundness = (.5 + rand() % 10) / 10.0f;
			}

			float sizeX = (rand() % 20 + 10.0f) / 10.0f;
			float sizeY = (rand() % 20 + 10.0f) / 10.0f;
			float sizeZ = (rand() % 20 + 10.0f) / 10.0f;

			float rotX = rand() % 35 * DEG2RAD;
			float rotY = rand() % 35 * DEG2RAD;
			float rotZ = rand() % 35 * DEG2RAD;

			Box b = Box(QuaternionFromEuler(rotX, rotY, rotZ), { offset * i - offset * 0.5f, 7 + (float)(rand() % 4), offset * j - offset * 0.5f}, { sizeX, sizeY, sizeZ }, roundness);
			//Box b = Box(QuaternionFromEuler(0, 0, 0), { offset * i - offset * 0.5f, 7 + (float)(rand() % 4), offset * j - offset * 0.5f }, { sizeX, sizeY, sizeZ }, roundness);

			boxes.push_back(b);

			rounded = !rounded;
		}
	}
}

void Scene::updateBallPhysic(Ball& curBall, std::vector<Box*> colliders, float deltaTime)
{
	vec3 ptA = curBall.sphere_collider.center;
	vec3 ptB = curBall.computeNextPosition(deltaTime);

	vec3 interPt;
	vec3 interNormal;

	bool hit = false;

	for (int i = 0; i < colliders.size(); ++i)
	{
		Box box = *colliders[i];
		if (collision_sphere_box(curBall.sphere_collider, ptA, ptB, box, interPt, interNormal))
		{
			SetSoundPitch(collisionSound, 1.0f + (rand() % 50) / 100.0f);
			PlaySound(collisionSound);
			//
			//if (i > 4) // SHOW BOX IMPACT
			//{
			//	Box rBox = Box(box.quaternion, box.position, box.extensions + curBall.sphere_collider.radius, curBall.sphere_collider.radius);
			//	drawBoxWireframe(rBox, Color{ 0, 255, 0, 255 });
			//}

			// Compute bounce angle
			vec3 reflectDir = curBall.velocity - interNormal * 2 * dot_product(curBall.velocity, interNormal);

			// Change velocity dir
			curBall.velocity = reflectDir;

			hit = true;
			break;
		}
	}

	if (!hit)
		curBall.updatePosition(ptB);


	// DEBUG DRAW SEGMENT TEST LINE	
	vec3 ba = ptB - ptA;
	vec3 baLonger = normalized(ba) * curBall.sphere_collider.radius;
	ptA = ptA + baLonger;
	
	//drawCylinder(Cylinder(ptA, ptB, 0.1f), 5, WHITE, true);
	DrawLine3D(ptA.toRlVec(), ptB.toRlVec(), ORANGE);
}

void Scene::ball_physics(float deltaTime)
{
	//if (IsKeyPressed(KEY_B)) balls.push_back(Ball({ rand() % 10 - 5, rand() % 8  + 12, rand() % 10 - 5 }, 1.0f, 10.0f, { 1, -1, 1 }));

	if (IsKeyDown(KEY_KP_ADD))
		timeMultiplier = Clamp(timeMultiplier + 0.1, 0, 5);

	if (IsKeyDown(KEY_KP_SUBTRACT))
		timeMultiplier = Clamp(timeMultiplier - 0.1, 0, 5);

	static bool debug = false;
	if (IsKeyPressed(KEY_D)) debug = !debug;
	
	std::vector<Box*> potentialBoxes;
	octree.get_sphere_potential_colliders(balls[0].sphere_collider, potentialBoxes);

	timeStock += deltaTime * timeMultiplier;
	while (timeStock > fixedDeltaTime)
	{
		timeStock -= fixedDeltaTime;
		for (int i = 0; i < balls.size(); ++i)
		{
			updateBallPhysic(balls[i], potentialBoxes, fixedDeltaTime);	
		}
	}
	

	

	if (debug)
	{
		octree.draw(PURPLE);
		Box sphereBox = Box(QuaternionIdentity(), balls[0].sphere_collider.center, vec3{ 1, 1, 1 } * balls[0].sphere_collider.radius, 0.0f);
		drawBoxWireframe(sphereBox, GREEN);
		for (auto& collider : potentialBoxes)
		{
			Box b = *collider;
			b.roundness = 0.0f;
			drawBoxWireframe(b, RED);
		}
	}

	// DRAW ALL BALLS
	for (int i = 0; i < balls.size(); ++i)
	{
		//drawSphere(balls[i].sphere_collider, 10, RED);
		drawSphereWireframe(balls[i].sphere_collider, 10, RED);
	}

	// DRAW GROUNBD
	drawBox(boxes[0], DARKGRAY);
	drawBoxWireframe(boxes[0], GRAY);



	if (debug)
	{
		//drawBoxWireframe(boxes[1], DARKGRAY);
		//drawBoxWireframe(boxes[2], DARKGRAY);
		//drawBoxWireframe(boxes[3], DARKGRAY);
		//drawBoxWireframe(boxes[4], DARKGRAY);
		
	}
	else
	{
		for (int i = 5; i < boxes.size(); i++)
		{
			drawBox(boxes[i], Color{ 100, 100, 150, 255 });
			if (boxes[i].roundness <= 0.01f)
				drawBoxWireframe(boxes[i], RAYWHITE);
		}
		//drawBox(boxes[1], ORANGE);
		//drawBox(boxes[2], ORANGE);
		//drawBox(boxes[3], ORANGE);
		//drawBox(boxes[4], ORANGE);
	}

	
	
	



	
}
*/