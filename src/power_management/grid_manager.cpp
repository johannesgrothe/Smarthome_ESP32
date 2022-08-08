#include "grid_manager.h"

GridManager::GridManager() :
    PowerManager(PowerMode::Grid_Mode) {
}

bool GridManager::refresh() {
  return false;
}
