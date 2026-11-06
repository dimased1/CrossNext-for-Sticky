"""Check native system/reader translation coverage and Vietnamese glyphs."""
from pathlib import Path
import re
import sys
import unicodedata

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from scripts.gen_i18n import parse_yaml_file

en = parse_yaml_file(ROOT / 'lib/I18n/translations/english.yaml')
vi = parse_yaml_file(ROOT / 'lib/I18n/translations/vietnamese.yaml')
paths = []
for folder in ('src/activities', 'src/components', 'src/apps_local/home',
               'src/apps_local/readertext', 'src/apps_local/tools'):
    for suffix in ('*.cpp', '*.h'):
        paths.extend((ROOT / folder).rglob(suffix))
paths.extend(ROOT / name for name in (
    'src/SettingsList.h', 'src/SideButtonSettings.h', 'src/main.cpp',
    'src/apps_local/ui/SettingsHome.h', 'src/apps_local/ui/SettingsMenuStyle.h',
    'src/apps_local/ui/MenuFontScreen.h', 'src/apps_local/ui/QuickSettingsScreen.h'))
needed = set()
for path in paths:
    needed.update(re.findall(r'\bSTR_[A-Z0-9_]+\b', path.read_text(encoding='utf-8')))
needed &= en.keys()
assert not (needed - vi.keys()), f'Missing Vietnamese keys: {sorted(needed - vi.keys())}'
printf = re.compile(r'%(?:[1-9][0-9]*\$)?[-+ #0]*(?:[0-9]+|\*)?(?:\.(?:[0-9]+|\*))?(?:hh|ll|h|l|j|z|t|L)?[diuoxXfFeEgGaAcspn%]')
for key in needed:
    if printf.findall(en[key]):
        assert printf.findall(en[key]) == printf.findall(vi[key]), f'Format mismatch: {key}'
    assert vi[key] == unicodedata.normalize('NFC', vi[key]), f'Non-normalized accents: {key}'
# The builtin fallback must render the accents even without SD interface fonts.
text = (ROOT / 'lib/EpdFont/builtinFonts/notosans_12_regular.h').read_text()
intervals = text.split('static const EpdUnicodeInterval notosans_12_regularIntervals[] = {', 1)[1].split('};', 1)[0]
ranges = [(int(a, 16), int(b, 16)) for a, b in re.findall(r'\{\s*0x([0-9A-F]+),\s*0x([0-9A-F]+),', intervals)]
chars = set(''.join(vi[key] for key in needed))
missing = [c for c in chars if ord(c) > 127 and not any(a <= ord(c) <= b for a, b in ranges)]
assert not missing, f'Missing fallback glyphs: {missing}'
print(f'Vietnamese: {len(needed)} native system/reader strings covered; formats, accents and fallback glyphs passed.')
