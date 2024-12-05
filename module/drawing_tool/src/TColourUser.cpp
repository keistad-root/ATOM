#include "TColourUser.h"

Int_t TColourUser::getColour(const std::string colorName) {
	if ( colorName == "alice_blue" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xF0 / 255., 0xF8 / 255., 0xFF / 255.);
		return index;
	} else if ( colorName == "antique_white" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFA / 255., 0xEB / 255., 0x07 / 255.);
		return index;
	} else if ( colorName == "aqua" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x00 / 255., 0xFF / 255., 0xFF / 255.);
		return index;
	} else if ( colorName == "aquamarine" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x7F / 255., 0xFF / 255., 0xD4 / 255.);
		return index;
	} else if ( colorName == "azure" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xF0 / 255., 0xFF / 255., 0xFF / 255.);
		return index;
	} else if ( colorName == "beige" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xF5 / 255., 0xF5 / 255., 0xDC / 255.);
		return index;
	} else if ( colorName == "bisque" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xE4 / 255., 0xC4 / 255.);
		return index;
	} else if ( colorName == "black" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x00 / 255., 0x00 / 255., 0x00 / 255.);
		return index;
	} else if ( colorName == "blanched_almond" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xEB / 255., 0xCD / 255.);
		return index;
	} else if ( colorName == "blue" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x00 / 255., 0x00 / 255., 0xFF / 255.);
		return index;
	} else if ( colorName == "blueViolet" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x8A / 255., 0x2B / 255., 0xE2 / 255.);
		return index;
	} else if ( colorName == "brown" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xA5 / 255., 0x2A / 255., 0x2A / 255.);
		return index;
	} else if ( colorName == "burly_wood" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xDE / 255., 0xB8 / 255., 0x87 / 255.);
		return index;
	} else if ( colorName == "cedet_blue" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x5F / 255., 0x9E / 255., 0xA0 / 255.);
		return index;
	} else if ( colorName == "chartreuse" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x7F / 255., 0xFF / 255., 0x00 / 255.);
		return index;
	} else if ( colorName == "chocolate" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xD2 / 255., 0x69 / 255., 0x1E / 255.);
		return index;
	} else if ( colorName == "coral" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0x7F / 255., 0x50 / 255.);
		return index;
	} else if ( colorName == "cornflower_blue" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x64 / 255., 0x95 / 255., 0xED / 255.);
		return index;
	} else if ( colorName == "cornsilk" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xF8 / 255., 0xDC / 255.);
		return index;
	} else if ( colorName == "crimson" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xDC / 255., 0x14 / 255., 0x3C / 255.);
		return index;
	} else if ( colorName == "cyan" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x00 / 255., 0xFF / 255., 0xFF / 255.);
		return index;
	} else if ( colorName == "dark_blue" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x00 / 255., 0x00 / 255., 0x8B / 255.);
		return index;
	} else if ( colorName == "dark_cyan" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x00 / 255., 0x8B / 255., 0x8B / 255.);
		return index;
	} else if ( colorName == "dark_golden_rod" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xB8 / 255., 0x86 / 255., 0x0B / 255.);
		return index;
	} else if ( colorName == "dark_grey" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xA9 / 255., 0xA9 / 255., 0xA9 / 255.);
		return index;
	} else if ( colorName == "dark_green" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x00 / 255., 0x64 / 255., 0x00 / 255.);
		return index;
	} else if ( colorName == "dark_khaki" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xBD / 255., 0xB7 / 255., 0x6B / 255.);
		return index;
	} else if ( colorName == "dark_magenta" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x8B / 255., 0x00 / 255., 0x8B / 255.);
		return index;
	} else if ( colorName == "dark_olive_green" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x55 / 255., 0x6B / 255., 0x2F / 255.);
		return index;
	} else if ( colorName == "dark_orange" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0x8C / 255., 0x00 / 255.);
		return index;
	} else if ( colorName == "dark_orchid" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x99 / 255., 0x32 / 255., 0xCC / 255.);
		return index;
	} else if ( colorName == "dark_red" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x8B / 255., 0x00 / 255., 0x00 / 255.);
		return index;
	} else if ( colorName == "dark_salmon" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xE9 / 255., 0x96 / 255., 0x7A / 255.);
		return index;
	} else if ( colorName == "dark_sea_green" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x8F / 255., 0xBC / 255., 0x8F / 255.);
		return index;
	} else if ( colorName == "dark_slate_blue" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x48 / 255., 0x3D / 255., 0x8B / 255.);
		return index;
	} else if ( colorName == "dark_slate_grey" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x2F / 255., 0x4F / 255., 0x4F / 255.);
		return index;
	} else if ( colorName == "dark_turquoise" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x00 / 255., 0xCE / 255., 0xD1 / 255.);
		return index;
	} else if ( colorName == "dark_violet" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x94 / 255., 0x00 / 255., 0xD3 / 255.);
		return index;
	} else if ( colorName == "deep_pink" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0x14 / 255., 0x93 / 255.);
		return index;
	} else if ( colorName == "deep_sky_blue" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x00 / 255., 0xBF / 255., 0xFF / 255.);
		return index;
	} else if ( colorName == "dim_grey" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x69 / 255., 0x69 / 255., 0x69 / 255.);
		return index;
	} else if ( colorName == "dodger_blue" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x1E / 255., 0x90 / 255., 0xFF / 255.);
		return index;
	} else if ( colorName == "fire_brick" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xB2 / 255., 0x22 / 255., 0x22 / 255.);
		return index;
	} else if ( colorName == "floral_white" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xFA / 255., 0xF0 / 255.);
		return index;
	} else if ( colorName == "forest_green" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x22 / 255., 0x8B / 255., 0x22 / 255.);
		return index;
	} else if ( colorName == "fuchsia" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0x00 / 255., 0xFF / 255.);
		return index;
	} else if ( colorName == "gainsboro" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xDC / 255., 0xDC / 255., 0xDC / 255.);
		return index;
	} else if ( colorName == "ghost_white" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xF8 / 255., 0xF8 / 255., 0xFF / 255.);
		return index;
	} else if ( colorName == "gold" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xD7 / 255., 0x00 / 255.);
		return index;
	} else if ( colorName == "golden_rod" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xDA / 255., 0xA5 / 255., 0x20 / 255.);
		return index;
	} else if ( colorName == "grey" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x80 / 255., 0x80 / 255., 0x80 / 255.);
		return index;
	} else if ( colorName == "green" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x00 / 255., 0x80 / 255., 0x00 / 255.);
		return index;
	} else if ( colorName == "green_yellow" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xAD / 255., 0xFF / 255., 0x2F / 255.);
		return index;
	} else if ( colorName == "honey_dew" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xF0 / 255., 0xFF / 255., 0xF0 / 255.);
		return index;
	} else if ( colorName == "hot_pink" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0x69 / 255., 0xB4 / 255.);
		return index;
	} else if ( colorName == "indian_red" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xCD / 255., 0x5C / 255., 0x5C / 255.);
		return index;
	} else if ( colorName == "indigo" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x4B / 255., 0x00 / 255., 0x82 / 255.);
		return index;
	} else if ( colorName == "ivory" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xFF / 255., 0xF0 / 255.);
		return index;
	} else if ( colorName == "khaki" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xF0 / 255., 0xE6 / 255., 0x8C / 255.);
		return index;
	} else if ( colorName == "lavender" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xE6 / 255., 0xE6 / 255., 0xFA / 255.);
		return index;
	} else if ( colorName == "lavender_blush" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xF0 / 255., 0xF5 / 255.);
		return index;
	} else if ( colorName == "lawn_green" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x7C / 255., 0xFC / 255., 0x00 / 255.);
		return index;
	} else if ( colorName == "lemon_chiffon" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xFA / 255., 0xCD / 255.);
		return index;
	} else if ( colorName == "light_blue" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xAD / 255., 0xD8 / 255., 0xE6 / 255.);
		return index;
	} else if ( colorName == "light_coral" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xF0 / 255., 0x80 / 255., 0x80 / 255.);
		return index;
	} else if ( colorName == "light_cyan" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xE0 / 255., 0xFF / 255., 0xFF / 255.);
		return index;
	} else if ( colorName == "light_golden_rod_yellow" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFA / 255., 0xFA / 255., 0xD2 / 255.);
		return index;
	} else if ( colorName == "light_grey" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xD3 / 255., 0xD3 / 255., 0xD3 / 255.);
		return index;
	} else if ( colorName == "light_green" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x90 / 255., 0xEE / 255., 0x90 / 255.);
		return index;
	} else if ( colorName == "light_pink" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xB6 / 255., 0xC1 / 255.);
		return index;
	} else if ( colorName == "light_salmon" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xA0 / 255., 0x7A / 255.);
		return index;
	} else if ( colorName == "light_sea_green" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x20 / 255., 0xB2 / 255., 0xAA / 255.);
		return index;
	} else if ( colorName == "light_sky_blue" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x87 / 255., 0xCE / 255., 0xFA / 255.);
		return index;
	} else if ( colorName == "light_slate_grey" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x77 / 255., 0x88 / 255., 0x99 / 255.);
		return index;
	} else if ( colorName == "light_steel_blue" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xB0 / 255., 0xC4 / 255., 0xDE / 255.);
		return index;
	} else if ( colorName == "light_yellow" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xFF / 255., 0xE0 / 255.);
		return index;
	} else if ( colorName == "lime" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x00 / 255., 0xFF / 255., 0x00 / 255.);
		return index;
	} else if ( colorName == "lime_green" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x32 / 255., 0xCD / 255., 0x32 / 255.);
		return index;
	} else if ( colorName == "linen" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFA / 255., 0xF0 / 255., 0xE6 / 255.);
		return index;
	} else if ( colorName == "magenta" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0x00 / 255., 0xFF / 255.);
		return index;
	} else if ( colorName == "maroon" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x80 / 255., 0x00 / 255., 0x00 / 255.);
		return index;
	} else if ( colorName == "medium_aqua_marine" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x66 / 255., 0xCD / 255., 0xAA / 255.);
		return index;
	} else if ( colorName == "medium_blue" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x00 / 255., 0x00 / 255., 0xCD / 255.);
		return index;
	} else if ( colorName == "medium_orchid" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xBA / 255., 0x55 / 255., 0xD3 / 255.);
		return index;
	} else if ( colorName == "medium_purple" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x93 / 255., 0x70 / 255., 0xDB / 255.);
		return index;
	} else if ( colorName == "medium_sea_green" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x3C / 255., 0xB3 / 255., 0x71 / 255.);
		return index;
	} else if ( colorName == "medium_slate_blue" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x7B / 255., 0x68 / 255., 0xEE / 255.);
		return index;
	} else if ( colorName == "medium_spring_green" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x00 / 255., 0xFA / 255., 0x9A / 255.);
		return index;
	} else if ( colorName == "medium_turquoise" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x48 / 255., 0xD1 / 255., 0xCC / 255.);
		return index;
	} else if ( colorName == "medium_violet_red" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xC7 / 255., 0x15 / 255., 0x85 / 255.);
		return index;
	} else if ( colorName == "midnight_blue" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x19 / 255., 0x19 / 255., 0x70 / 255.);
		return index;
	} else if ( colorName == "mint_cream" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xF5 / 255., 0xFF / 255., 0xFA / 255.);
		return index;
	} else if ( colorName == "misty_rose" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xE4 / 255., 0xE1 / 255.);
		return index;
	} else if ( colorName == "moccasin" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xE4 / 255., 0xB5 / 255.);
		return index;
	} else if ( colorName == "navajo_white" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xDE / 255., 0xAD / 255.);
		return index;
	} else if ( colorName == "navy" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x00 / 255., 0x00 / 255., 0x80 / 255.);
		return index;
	} else if ( colorName == "old_lace" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFD / 255., 0xF5 / 255., 0xE6 / 255.);
		return index;
	} else if ( colorName == "olive" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x80 / 255., 0x80 / 255., 0x00 / 255.);
		return index;
	} else if ( colorName == "olive_drab" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x6B / 255., 0x8E / 255., 0x23 / 255.);
		return index;
	} else if ( colorName == "orange" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xA5 / 255., 0x00 / 255.);
		return index;
	} else if ( colorName == "orange_red" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0x45 / 255., 0x00 / 255.);
		return index;
	} else if ( colorName == "orchid" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xDA / 255., 0x70 / 255., 0xD6 / 255.);
		return index;
	} else if ( colorName == "pale_golden_rod" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xEE / 255., 0xE8 / 255., 0xAA / 255.);
		return index;
	} else if ( colorName == "pale_green" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x98 / 255., 0xFB / 255., 0x98 / 255.);
		return index;
	} else if ( colorName == "pale_turquoise" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xAF / 255., 0xEE / 255., 0xEE / 255.);
		return index;
	} else if ( colorName == "pale_violet_red" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xDB / 255., 0x70 / 255., 0x93 / 255.);
		return index;
	} else if ( colorName == "papaya_whip" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xEF / 255., 0xD5 / 255.);
		return index;
	} else if ( colorName == "peach_puff" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xDA / 255., 0xB9 / 255.);
		return index;
	} else if ( colorName == "peru" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xCD / 255., 0x85 / 255., 0x3F / 255.);
		return index;
	} else if ( colorName == "pink" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xC0 / 255., 0xCB / 255.);
		return index;
	} else if ( colorName == "plum" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xDD / 255., 0xA0 / 255., 0xDD / 255.);
		return index;
	} else if ( colorName == "powder_blue" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xB0 / 255., 0xE0 / 255., 0xE6 / 255.);
		return index;
	} else if ( colorName == "purple" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x80 / 255., 0x00 / 255., 0x80 / 255.);
		return index;
	} else if ( colorName == "rebecca_purple" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x66 / 255., 0x33 / 255., 0x99 / 255.);
		return index;
	} else if ( colorName == "red" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0x00 / 255., 0x00 / 255.);
		return index;
	} else if ( colorName == "rosy_brown" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xBC / 255., 0x8F / 255., 0x8F / 255.);
		return index;
	} else if ( colorName == "royal_blue" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x41 / 255., 0x69 / 255., 0xE1 / 255.);
		return index;
	} else if ( colorName == "saddle_brown" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x8B / 255., 0x45 / 255., 0x13 / 255.);
		return index;
	} else if ( colorName == "salmon" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFA / 255., 0x80 / 255., 0x72 / 255.);
		return index;
	} else if ( colorName == "sandy_brown" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xF4 / 255., 0xA4 / 255., 0x60 / 255.);
		return index;
	} else if ( colorName == "sea_green" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x2E / 255., 0x8B / 255., 0x57 / 255.);
		return index;
	} else if ( colorName == "sea_shell" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xF5 / 255., 0xEE / 255.);
		return index;
	} else if ( colorName == "sienna" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xA0 / 255., 0x52 / 255., 0x2D / 255.);
		return index;
	} else if ( colorName == "silver" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xC0 / 255., 0xC0 / 255., 0xC0 / 255.);
		return index;
	} else if ( colorName == "sky_blue" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x87 / 255., 0xCE / 255., 0xEB / 255.);
		return index;
	} else if ( colorName == "slate_blue" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x6A / 255., 0x5A / 255., 0xCD / 255.);
		return index;
	} else if ( colorName == "slate_grey" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x70 / 255., 0x80 / 255., 0x90 / 255.);
		return index;
	} else if ( colorName == "snow" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xFA / 255., 0xFA / 255.);
		return index;
	} else if ( colorName == "spring_green" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x00 / 255., 0xFF / 255., 0x7F / 255.);
		return index;
	} else if ( colorName == "steel_blue" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x46 / 255., 0x82 / 255., 0xB4 / 255.);
		return index;
	} else if ( colorName == "tan" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xD2 / 255., 0xB4 / 255., 0x8C / 255.);
		return index;
	} else if ( colorName == "teal" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x00 / 255., 0x80 / 255., 0x80 / 255.);
		return index;
	} else if ( colorName == "thistle" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xD8 / 255., 0xBF / 255., 0xD8 / 255.);
		return index;
	} else if ( colorName == "tomato" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0x63 / 255., 0x47 / 255.);
		return index;
	} else if ( colorName == "turquoise" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x40 / 255., 0xE0 / 255., 0xD0 / 255.);
		return index;
	} else if ( colorName == "violet" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xEE / 255., 0x82 / 255., 0xEE / 255.);
		return index;
	} else if ( colorName == "wheat" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xF5 / 255., 0xDE / 255., 0xB3 / 255.);
		return index;
	} else if ( colorName == "white" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xFF / 255., 0xFF / 255.);
		return index;
	} else if ( colorName == "white_smoke" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xF5 / 255., 0xF5 / 255., 0xF5 / 255.);
		return index;
	} else if ( colorName == "yellow" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0xFF / 255., 0xFF / 255., 0x00 / 255.);
		return index;
	} else if ( colorName == "yellow_green" ) {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x9A / 255., 0xCD / 255., 0x32 / 255.);
		return index;
	} else {
		int index = TColor::GetFreeColorIndex();
		new TColor(index, 0x00 / 255., 0x00 / 255., 0x00 / 255.);
		return index;
	}
}