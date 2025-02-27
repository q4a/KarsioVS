#pragma once
#include "../../Engine/Component.h"


enum UpgradeType { CHASSIS_UPGRADE, RAM_UPGRADE, ARMOR_UPGRADE, GUN_UPGRADE };

class UpgradeComponent : public Component
{
private:
    float resources = 0.f;
    float resourcesForNextLevel = 100.0f;
    float increasedResourceAmountModifier = 1.25f; // multiplied by the resourcesForNextLevel value after each upgrade
    
    int numberOfUpgrades = 0; // This is how many upgrades the vehicle has undertaken.
    int chassisLevel = 1; // 1-3
    int gunLevel = 1; // 1-5
    int armorLevel = 1; // 1-5
    int ramLevel = 1; // 1-5
    int minUpgradesForChassisUpgrade[2] = { 4, 8 };
    int maxLevelPerChassis[3] = { 3, 4, 5 };
    
    float chassisHealth[3] =  {200.f, 300.f, 400.f};
    float armorChange[5] = {0, 1.f, 2.f, 4.f, 5.f};
    float ramMultiplierChange[5] = {1.f, 1.5f, 2.f, 2.5f, 3.f};
    
    // The gun changes are a bit more drastic.
    float gunROFChange[5] = {3.f, 5.f, 7.f, 7.f, 9.f}; // Per second
    float gunDamageChange[5] = {3.f, 4.f, 5.f, 8.f, 10.f};
    float projectileSpeedChange[5] = {275.f, 275.f, 275.f, 275.f, 275.f};

public:
    UpgradeComponent();
    ~UpgradeComponent();
    void addResources(float value);

    float getCurrentRamMultiplier();

    bool isUpgradeAvailable() const;
    bool canUpgradeType(UpgradeType type);
    bool upgradeVehicle(UpgradeType type);

    float getCurrentGunROF() {return gunROFChange[gunLevel-1];}
    float getCurrentGunDamage() {return gunDamageChange[gunLevel-1];}
    float getCurrentProjSpeed() {return projectileSpeedChange[gunLevel-1];}
    int getChassisLevel() {return chassisLevel;}
    int getArmorLevel() {return armorLevel;}
    int getGunLevel() {return gunLevel;}
    int getRamLevel() {return ramLevel;}
    float getCurrentResources() {return resources; }
    float getMaxResources() {return resourcesForNextLevel; }

	bool fullyUpgraded();
	void setPreUpgradeLevels(int chassis = 1, int armor = 1, int gun = 1, int ram = 1);
	int getNextUpgradeBoundary();
};

