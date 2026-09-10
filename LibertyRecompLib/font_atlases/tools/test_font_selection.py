"""Regressions for font identity and atlas-bank selection.

Run with a Python environment that has the atlas builder's Pillow dependency:
    python3 -m unittest discover -s LibertyRecompLib/font_atlases/tools -p 'test_*.py'
"""
from pathlib import Path
import tempfile
import unittest

import build_font_atlases as atlas


class FontSelectionTests(unittest.TestCase):
    def bank(self, profile, name, section):
        matches = [item for item in atlas.PROFILE_FACE_RANGES[profile]
                   if item.atlas == name and item.range_section == section]
        self.assertEqual(len(matches), 1)
        return matches[0]

    def test_standard_text_selects_din_in_every_profile(self):
        for profile in atlas.PROFILE_FACE_RANGES:
            with self.subTest(profile=profile):
                bank = self.bank(profile, 'font1', 'MAINFONT')
                self.assertEqual(bank.font_key, 'din_mittelschrift')
                self.assertEqual((bank.first, bank.last_exclusive), (0, 134))
                self.assertIsNone(bank.fixed_transform,
                                  'Do not reuse Helvetica-specific metrics for DIN')
                self.assertTrue(bank.calibrate_sampled_width)

    def test_base_name_labels_use_compressed_not_standard_din(self):
        bank = self.bank('gta4', 'font3', 'SUBFONT_2')
        self.assertEqual(bank.font_key, 'helvetica_compressed')
        self.assertEqual((bank.first, bank.last_exclusive), (93, 162))

    def test_episode_name_fonts_are_not_replaced_by_base_name_font(self):
        self.assertEqual(self.bank('tlad', 'font3', 'SUBFONT_2').font_key, 'mesquite')
        self.assertEqual(self.bank('tbogt', 'font3', 'SUBFONT_2').font_key, 'din_bold')

    def test_pricedown_is_restricted_to_its_numeric_heading_bank(self):
        for profile, ranges in atlas.PROFILE_FACE_RANGES.items():
            selected = [item for item in ranges if item.font_key == 'pricedown']
            with self.subTest(profile=profile):
                self.assertEqual(len(selected), 1)
                self.assertEqual((selected[0].atlas, selected[0].range_section,
                                  selected[0].first, selected[0].last_exclusive),
                                 ('font1', 'SUBFONT_1', 134, 150))

    def test_menu_and_roman_banks_are_retained(self):
        for profile in atlas.PROFILE_FACE_RANGES:
            with self.subTest(profile=profile):
                self.assertEqual(self.bank(profile, 'font2', 'MAINFONT').font_key,
                                 'helvetica_heavy')
                self.assertEqual(self.bank(profile, 'font2', 'SUBFONT_1').font_key,
                                 'helvetica_roman')

    def test_common_symbols_taxi_and_chalk_are_not_broadly_replaced(self):
        for profile, ranges in atlas.PROFILE_FACE_RANGES.items():
            for item in ranges:
                with self.subTest(profile=profile, atlas=item.atlas, bank=item.range_section):
                    self.assertNotEqual(item.range_section, 'COMMON_FONT')
                    if item.atlas == 'font3':
                        self.assertEqual(item.range_section, 'SUBFONT_2')

    def test_declared_ranges_do_not_overlap(self):
        for profile, ranges in atlas.PROFILE_FACE_RANGES.items():
            for name in atlas.ATLAS_NAMES:
                banks = sorted((item.first, item.last_exclusive) for item in ranges
                               if item.atlas == name)
                for previous, following in zip(banks, banks[1:]):
                    with self.subTest(profile=profile, atlas=name):
                        self.assertLessEqual(previous[1], following[0])

    def test_title_sharing_follows_the_verified_stock_atlases(self):
        self.assertEqual(atlas.ATLAS_FONT_IDS, {'font1': 0, 'font2': 2, 'font3': 1})
        self.assertEqual(atlas.PROFILE_SHARED_ATLASES,
                         {'gta4': (), 'tlad': ('font1', 'font2'), 'tbogt': ('font1',)})

    def test_explicit_new_pack_overrides_older_same_filename(self):
        with tempfile.TemporaryDirectory(prefix='liberty-font-selection-test-') as directory:
            root = Path(directory)
            for pack in ('pack0', 'pack1'):
                (root / pack).mkdir()
                (root / pack / 'din1451alt.ttf').write_bytes(pack.encode())
                (root / pack / 'Pricedown Bl.otf').write_bytes(pack.encode())
            (root / 'pack1' / 'din1451alt G.ttf').write_bytes(b'not the requested face')
            compressed = root / 'pack1' / 'helvetica-compressed-5871d14b6903a.otf'
            compressed.write_bytes(b'compressed')
            selected = atlas.locate_fonts(root, {'din_mittelschrift', 'pricedown',
                                                 'helvetica_compressed'})
            self.assertEqual(selected['din_mittelschrift'], root / 'pack1/din1451alt.ttf')
            self.assertEqual(selected['pricedown'], root / 'pack1/Pricedown Bl.otf')
            self.assertEqual(selected['helvetica_compressed'], compressed)

    def test_missing_requested_face_is_an_error_not_another_font(self):
        with tempfile.TemporaryDirectory(prefix='liberty-font-selection-test-') as directory:
            root = Path(directory)
            (root / 'din1451alt G.ttf').write_bytes(b'wrong DIN variant')
            with self.assertRaises(RuntimeError):
                atlas.locate_fonts(root, {'din_mittelschrift'})


if __name__ == '__main__':
    unittest.main()
