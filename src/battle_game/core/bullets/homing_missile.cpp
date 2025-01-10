#include "homing_missile.h"
#include "battle_game/core/game_core.h"
#include "battle_game/graphics/graphics.h"

namespace battle_game::bullet {

HomingMissile::HomingMissile(GameCore *core, uint32_t id, uint32_t unit_id, uint32_t player_id, glm::vec2 position, float rotation, float damage_scale, glm::vec2 velocity)
    : Bullet(core, id, unit_id, player_id, position, rotation, damage_scale), velocity_(velocity) {
}

void HomingMissile::Render() {
  battle_game::SetTransformation(position_, rotation_);
  battle_game::SetTexture(0);
  battle_game::SetColor({1.0f, 0.0f, 0.0f, 1.0f});
  battle_game::DrawModel(0);  // Use a suitable model index for the missile
}

void HomingMissile::Update() {
  auto nearest_enemy = game_core_->GetNearestEnemyUnit(position_);
  if (nearest_enemy != nullptr) {
    auto direction = glm::normalize(nearest_enemy->GetPosition() - position_);
    velocity_ = glm::mix(velocity_, direction * glm::length(velocity_), 0.1f);
  }
  position_ += velocity_ * kSecondPerTick;

  game_core_->PushEventGenerateParticle<particle::Smoke>(position_, rotation_, glm::vec4{1.0f, 0.5f, 0.0f, 1.0f}, 0.1f);

  // Check for collision with enemy units
  for (const auto& unit_pair : game_core_->GetUnits()) {
    auto& unit = unit_pair.second;
    if (unit_pair.first == unit_id_) continue;
    if (unit->GetPlayerId() != player_id_ && unit->IsHit(position_)) {
      game_core_->PushEventDealDamage(unit_pair.first, id_, damage_scale_ * 20.0f);
      game_core_->PushEventRemoveBullet(id_);
      return;
    }
  }

  if (game_core_->IsOutOfRange(position_)) {
    game_core_->PushEventRemoveBullet(id_);
  }
}

}  // namespace battle_game::bullet