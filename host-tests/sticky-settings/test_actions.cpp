#include <cassert>
#include <cstdio>

#include "../../src/util/SideButtonActions.h"

int main() {
  // Stable IDs are also the values already saved on the user's SD card.
  static_assert(int(SideButtonAction::Home) == 3);
  static_assert(int(SideButtonAction::ToggleWarmth) == 12);
  constexpr int sticky[] = {0, 1, 3, 4, 5, 8, 9, 11, 13, 14, 16};
  for (unsigned row = 0; row < sizeof(sticky) / sizeof(sticky[0]); ++row) {
    assert(sideActionAt(int(row), false, false, false) == sticky[row]);
    assert(sideActionRow(uint8_t(sticky[row]), false, false, false) == row);
  }
  assert(sideActionAt(11, false, false, false) == -1);
  assert(sideActionAt(-1, false, false, false) == -1);
  // Unsupported bindings copied from another model return to navigation.
  assert(!sideActionHardwareAvailable(SideButtonAction::ToggleFrontlight, false, false, false));
  assert(!sideActionHardwareAvailable(SideButtonAction::ToggleWarmth, false, false, false));
  assert(!sideActionHardwareAvailable(SideButtonAction::ToggleTouchscreen, false, false, false));
  assert(sideActionHardwareAvailable(SideButtonAction::Home, false, false, false));
  assert(sideActionHardwareAvailable(SideButtonAction::Navigation, false, false, false));
  // Light-equipped boards retain their supported actions. Filtered indices
  // must map back to the original ID, including actions after removed entries.
  for (int caps = 0; caps < 8; ++caps) {
    bool light = caps & 1, warm = caps & 2, lock = caps & 4;
    bool seen[int(SideButtonAction::Count)]{};
    for (int row = 0, value; (value = sideActionAt(row, light, warm, lock)) >= 0; ++row) {
      assert(!seen[value]);
      seen[value] = true;
      assert(sideActionRow(uint8_t(value), light, warm, lock) == row);
    }
    assert(seen[int(SideButtonAction::ToggleFrontlight)] == light);
    assert(seen[int(SideButtonAction::ToggleWarmth)] == warm);
    assert(seen[int(SideButtonAction::ToggleTouchscreen)] == lock);
    assert(seen[int(SideButtonAction::Home)]);
    assert(seen[int(SideButtonAction::Navigation)]);
  }
  std::puts("Sticky hardware actions: supported choices and saved-ID round trips passed.");
}
