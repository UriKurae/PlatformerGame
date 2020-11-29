#include "Executioner.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "Collisions.h"

Executioner::Executioner(iPoint pos) : Enemy(pos)
{
    // Idle animation
    idleAnim.PushBack({ 15, 421, 68, 93 });
    idleAnim.PushBack({ 104, 420, 68, 93 });
    idleAnim.PushBack({ 191, 420, 68, 91 });
    idleAnim.PushBack({ 6, 523, 68, 90 });
    idleAnim.PushBack({ 95, 522, 68, 90 });
    idleAnim.PushBack({ 182, 522, 68, 87 });
    idleAnim.PushBack({ 275, 524, 68, 87 });
    


    Start();
}

bool Executioner::Start()
{
    texture = app->tex->Load("Assets/textures/Executioner/executioner.png");
    
    currentAnim = &idleAnim;
    collider = app->collisions->AddCollider({ position.x, position.y, 68,90 }, Collider::TYPE::ENEMY);

    return true;
}

bool Executioner::Update(float dt)
{
    idleAnim.speed = 4.0f * dt;

    currentAnim->Update();
    collider->SetPos(position.x, position.y);

    return true;
}


void Executioner::Attack()
{
}
