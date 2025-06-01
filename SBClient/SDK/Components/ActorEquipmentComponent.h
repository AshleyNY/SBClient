#pragma once
#include "../World/Inventory/SimpleContainer.h"

struct ActorEquipmentComponent {
	SimpleContainer* offhandContainer;
	SimpleContainer* armorContainer;
};