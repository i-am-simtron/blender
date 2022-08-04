/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#include "BKE_node.h"

#ifdef __cplusplus
extern "C" {
#endif

	extern struct bNodeTreeType* ntreeType_StoryBoard;

	void register_node_tree_type_storyboard(void);

	
	// TODO: @Sreetejasimha
	// void register_node_type_storyboard_scene(void);

#ifdef __cplusplus
}
#endif
