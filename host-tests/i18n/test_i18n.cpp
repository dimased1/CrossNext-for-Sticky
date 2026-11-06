#include <I18n.h>
#include <I18nStrings.h>

#include <cassert>
#include <cstdio>
#include <cstring>
#include <initializer_list>

int main() {
  static constexpr UiLabel absent = nullptr;
  assert(absent.get() == nullptr && UiLabel{}.get() == nullptr);
  static constexpr UiLabel label = trLabel(STR_QUEST_HERO);
  I18N.setLanguage(Language::EN);
  assert(std::strcmp(label.get(), "Hero") == 0);
  for (const auto language : {Language::EN, Language::ES}) {
    I18N.setLanguage(language);
    assert(std::strlen(tr(STR_WORDLE_SAMPLE_4)) == 4);
    assert(std::strlen(tr(STR_WORDLE_SAMPLE_5)) == 5);
    assert(std::strlen(tr(STR_WORDLE_SAMPLE_6)) == 6);
  }
  I18N.setLanguage(Language::EN);
  I18N.setLanguage(Language::ES);
  assert(std::strcmp(label.get(), "Héroe") == 0);
  bool vietnameseVisible = false;
  for (const auto language : SORTED_LANGUAGE_INDICES)
    if (language == static_cast<uint8_t>(Language::VI)) vietnameseVisible = true;
  assert(sizeof(SORTED_LANGUAGE_INDICES) == (vietnameseVisible ? 3 : 2));
  const auto index = static_cast<unsigned>(StrId::STR_WALLPAPER_NOTE_CBA12088);
  // This late source entry crosses the former 15-bit offset limit.
  assert(i18n_strings::OFFSETS_EN[index] > 32767);
  assert(std::strcmp(tr(STR_WALLPAPER_NOTE_CBA12088), "se alternan. Toca para cambiar.") == 0);
  unsigned fallback = 0;
  for (unsigned n = 0; n < static_cast<unsigned>(StrId::_COUNT); ++n) {
    const auto offset = i18n_strings::OFFSETS_ES[n];
    const auto* text = I18N.get(static_cast<StrId>(n));
    assert(std::strcmp(text, "???") != 0);
    if (offset & 0x80000000u) {
      assert(std::strcmp(text, i18n_strings::STRINGS_EN_DATA + (offset & 0x7fffffffu)) == 0);
      ++fallback;
    }
  }
  assert(fallback > 0);
  if (vietnameseVisible) {
    I18N.setLanguage(Language::VI);
    assert(std::strcmp(tr(STR_SETTINGS_TITLE), "Cài đặt") == 0);
    assert(std::strcmp(tr(STR_GO_HOME_BUTTON), "Về màn hình chính") == 0);
    assert(std::strcmp(tr(STR_READER_SEARCH), "Tìm trong sách") == 0);
    assert(std::strcmp(I18N.getLanguageName(Language::VI), "Tiếng Việt") == 0);
    // Game/story content retains the same per-key English fallback contract.
    assert(std::strcmp(label.get(), "anh hùng") == 0 || std::strcmp(label.get(), "Hero") == 0);
  }
  I18N.setLanguage(Language::EN);
  assert(std::strcmp(label.get(), "Hero") == 0);
  std::printf("Localized labels, late offsets, %u English fallback entries and %zu visible languages passed.\n",
              fallback, sizeof(SORTED_LANGUAGE_INDICES));
}
