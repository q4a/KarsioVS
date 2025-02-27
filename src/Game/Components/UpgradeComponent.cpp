#include "UpgradeComponent.h"
#include "../../Engine/Entity.h"
#include "HealthComponent.h"
#include "../../engine/EntityManager.h"
#include "WeaponComponent.h"
#include "../../Engine/Core.h"


UpgradeComponent::UpgradeComponent() : Component(UPGRADE) {
}


UpgradeComponent::~UpgradeComponent() = default;

void UpgradeComponent::addResources(float value) {
    resources += value;

    if (static_cast<HealthComponent*>(owner->getComponent(HEALTH))->isPlayerInvolved(owner->id)) {
        const int roundedCrystal = round(value);
        std::string crystalString = std::to_string(roundedCrystal);
        EntityManager::getInstance()->createFloatingText(owner, glm::vec3(0.2,0.6,1), 0.6f, &crystalString);
    }
}

float UpgradeComponent::getCurrentRamMultiplier() {
    return ramMultiplierChange[ramLevel - 1];
}

bool UpgradeComponent::isUpgradeAvailable() const {
    return (resources >= resourcesForNextLevel) &&
    (chassisLevel < 3 || gunLevel < 5 || ramLevel < 5 || armorLevel < 5); // Can't upgrade if we are fully upgraded
}

bool UpgradeComponent::canUpgradeType(UpgradeType type) {
    if (!isUpgradeAvailable()) return false;
    switch (type) {
        case CHASSIS_UPGRADE:
            return (chassisLevel < 3 && numberOfUpgrades >= minUpgradesForChassisUpgrade[chassisLevel - 1]);
        case GUN_UPGRADE:
            return (gunLevel < maxLevelPerChassis[chassisLevel - 1]);
        case ARMOR_UPGRADE:
            return (armorLevel < maxLevelPerChassis[chassisLevel - 1]);
        case RAM_UPGRADE:
            return (ramLevel < maxLevelPerChassis[chassisLevel - 1]);
        default:
            return false;
    }
}

bool UpgradeComponent::upgradeVehicle(UpgradeType type) {

    if(!canUpgradeType(type)) return false; // Ensure we can upgrade this thing first

    resources -= resourcesForNextLevel;
    resourcesForNextLevel *= increasedResourceAmountModifier;
    numberOfUpgrades++;

    if (type == CHASSIS_UPGRADE) {
        chassisLevel++;
        static_cast<HealthComponent*>(owner->getComponent(HEALTH))->setMaxHealth(chassisHealth[chassisLevel-1], true);
        EntityManager::getInstance()->updateChassis(owner, 1.5f, chassisLevel);
        EntityManager::getInstance()->updateArmor(owner, chassisLevel, armorLevel);
        return true;
    } else if (type == GUN_UPGRADE) {
        // Upgrade the stats of the gun component.
        gunLevel++;
        static_cast<WeaponComponent*>(owner->getComponent(WEAPON))->updateGunValues(gunROFChange[gunLevel-1], gunDamageChange[gunLevel-1], projectileSpeedChange[gunLevel-1]);
        EntityManager::getInstance()->updateGun(owner, gunLevel);
        return true;
    } else if (type == ARMOR_UPGRADE) {
        armorLevel++;
        static_cast<HealthComponent*>(owner->getComponent(HEALTH))->setArmor(armorChange[armorLevel-1]);
        EntityManager::getInstance()->updateArmor(owner, chassisLevel, armorLevel);
        return true;
    } else if (type == RAM_UPGRADE) {
        // Tell the entity manager to update the ram look.
        ramLevel++;
        EntityManager::getInstance()->updateRam(owner, ramLevel);
        return true;
    }
    return false;
}

bool UpgradeComponent::fullyUpgraded() {
	if (chassisLevel == 3 && gunLevel == 5 && armorLevel == 5 && ramLevel == 5) {
		return true;
	}
	else {
		return false;
	}
}

void UpgradeComponent::setPreUpgradeLevels(int chassis, int armor, int gun, int ram) {
	chassisLevel = chassis;
	armorLevel = armor;
	gunLevel = gun;
	ramLevel = ram;
	
    // The - 4 takes into account that all of these values start at 1, but that isnt counted as an upgrade.
    numberOfUpgrades += chassis + armor + gun + ram - 4;
	// chassis
	for (int i = 1; i < chassis; i++) {
		static_cast<HealthComponent*>(owner->getComponent(HEALTH))->setMaxHealth(chassisHealth[i - 1], true);
		EntityManager::getInstance()->updateChassis(owner, 1.5f, i);
	}
	
	// armor
	for (int i = 1; i < armor; i++) {
		static_cast<HealthComponent*>(owner->getComponent(HEALTH))->setArmor(armorChange[i - 1]);
		EntityManager::getInstance()->updateArmor(owner, chassisLevel, i);
	}
	
	// gun
	static_cast<WeaponComponent*>(owner->getComponent(WEAPON))->updateGunValues(gunROFChange[gunLevel - 1], gunDamageChange[gunLevel - 1], projectileSpeedChange[gunLevel - 1]);
	EntityManager::getInstance()->updateGun(owner, gunLevel);
	
	// ram
	EntityManager::getInstance()->updateRam(owner, ramLevel);
}

int UpgradeComponent::getNextUpgradeBoundary() {
	return chassisLevel < 3 ? minUpgradesForChassisUpgrade[chassisLevel - 1] : -1;
}