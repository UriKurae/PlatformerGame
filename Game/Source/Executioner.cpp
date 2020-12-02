#include "Executioner.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "EnemyManager.h"
#include "Pathfinding.h"
#include "Collisions.h"
#include "Input.h"
#include "Player.h"

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
    idleAnim.loop = true;

    // Skill animation
    skillAnim.PushBack({ 433, 398, 67, 93 });
    skillAnim.PushBack({ 529, 392, 72, 99 });
    skillAnim.PushBack({ 623, 395, 45, 97 });
    skillAnim.PushBack({ 710, 395, 45, 97 });
    skillAnim.PushBack({ 772, 396, 47, 96 });
    skillAnim.PushBack({ 836, 395, 50, 96 });
    skillAnim.PushBack({ 423, 503, 69, 96 });
    skillAnim.PushBack({ 500, 507, 116, 96 });
    skillAnim.PushBack({ 625, 505, 76, 96 });
    skillAnim.PushBack({ 713, 504, 50, 96 });
    skillAnim.PushBack({ 784, 506, 57, 97 });
    skillAnim.PushBack({ 854, 500, 72, 102 });
    skillAnim.loop = false;

    // Death animation
    deathAnim.PushBack({ 1135, 16, 79, 94 });
    deathAnim.PushBack({ 1230, 16, 79, 94 });
    deathAnim.PushBack({ 1327, 21, 79, 90 });
    deathAnim.PushBack({ 1418, 29, 48, 81 });
    deathAnim.PushBack({ 1488, 29, 46, 81 });
    deathAnim.PushBack({ 1550, 28, 31, 81 });
    deathAnim.PushBack({ 1597, 30, 30, 66 });
    deathAnim.PushBack({ 1643, 39, 33, 58 });
    deathAnim.PushBack({ 1691, 45, 39, 45 });
    deathAnim.PushBack({ 1752, 45, 39, 45 });



}

bool Executioner::Start()
{
    texture = app->tex->Load("Assets/textures/Executioner/executioner.png");
    
    currentAnim = &idleAnim;
    collider = app->collisions->AddCollider({ position.x - 2, position.y + 10, 37, 80 }, Collider::Type::ENEMY); // 10 stands for offset

    return true;
}

bool Executioner::Update(float dt)
{
    if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
    {
        app->pathFinding->ResetPath(iPoint(position.x / 16, position.y / 16));
    }
   
    idleAnim.speed = 4.0f * dt;

    currentAnim->Update();
    collider->SetPos(position.x - 2, position.y + 10);

   // app->pathFinding->DrawPath();
    return true;
}

bool Executioner::CleanUp()
{
    app->tex->UnLoad(texture);
    collider->pendingToDelete = true;
    app->enemyManager->DeleteEnemy(this);

    return true;
}

void Executioner::Attack()
{
}

void Executioner::Draw()
{
    if (currentAnim != nullptr)
    {
        app->render->DrawTexture(texture, position.x, position.y, &currentAnim->GetCurrentFrame());
    }
}

bool Executioner::FindTarget(Player* player)
{

    
    app->pathFinding->PropagateBFS(player);
    app->pathFinding->DrawPath();
    app->pathFinding->ComputePath(player->GetPosition().x, player->GetPosition().y);

    return true;
}

bool Executioner::ChaseTarget(iPoint position)
{

    return true;
}