#pragma once
#include "battle_game/core/unit.h"

namespace battle_game::unit {
class larry_tank : public Unit {
 public:
  larry_tank(GameCore *game_core, uint32_t id, uint32_t player_id);
  void Render() override;
  void Update() override;
  [[nodiscard]] bool IsHit(glm::vec2 position) const override;

 protected:
  void Move(float move_speed, float rotate_angular_speed);
  void RotateTurret();
  void Fire();
  void FireMissile();
  void ActivateShield();
  [[nodiscard]] const char *UnitName() const override;
  [[nodiscard]] const char *Author() const override;

  float turret_rotation_{0.0f};
  uint32_t fire_count_down_{0};
  bool shield_active_{false};
  uint32_t shield_duration_{0};
};
}  // namespace battle_game::unit