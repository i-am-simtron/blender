/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <cstring>

#include "MEM_guardedalloc.h"

#include "NOD_storyboard.h"

#include "BKE_context.h"
#include "BKE_node.h"

#include "BLT_translation.h"

#include "DNA_node_types.h"

#include "RNA_access.h"
#include "RNA_prototypes.h"

#include "UI_resources.h"

#include "node_common.h"
#include <BKE_object.h>
#include <DNA_modifier_types.h>

bNodeTreeType *ntreeType_StoryBoard;

static void storyboard_node_tree_get_from_context(const bContext *C,
                                                bNodeTreeType *UNUSED(treetype),
                                                bNodeTree **r_ntree,
                                                ID **r_id,
                                                ID **r_from)
{
  ViewLayer *view_layer = CTX_data_view_layer(C);
  Object *ob = OBACT(view_layer);

  if (ob == nullptr) {
    return;
  }

  const ModifierData *md = BKE_object_active_modifier(ob);

  if (md == nullptr) {
    return;
  }

  if (md->type == eModifierType_Nodes) {
    NodesModifierData *nmd = (NodesModifierData *)md;
    if (nmd->node_group != nullptr) {
      *r_from = &ob->id;
      *r_id = &ob->id;
      *r_ntree = nmd->node_group;
    }
  }
}

static void storyboard_node_tree_update(bNodeTree *ntree)
{
  ntreeSetOutput(ntree);

  /* Needed to give correct types to reroutes. */
  ntree_update_reroute_nodes(ntree);
}

static void foreach_nodeclass(Scene *UNUSED(scene), void *calldata, bNodeClassCallback func)
{
  func(calldata, NODE_CLASS_INPUT, N_("Input"));
  func(calldata, NODE_CLASS_GEOMETRY, N_("Geometry"));
  func(calldata, NODE_CLASS_ATTRIBUTE, N_("Attribute"));
  func(calldata, NODE_CLASS_OP_COLOR, N_("Color"));
  func(calldata, NODE_CLASS_OP_VECTOR, N_("Vector"));
  func(calldata, NODE_CLASS_CONVERTER, N_("Converter"));
  func(calldata, NODE_CLASS_LAYOUT, N_("Layout"));
}

static bool storyboard_node_tree_validate_link(eNodeSocketDatatype type_a,
                                             eNodeSocketDatatype type_b)
{
  /* Geometry, string, object, material, texture and collection sockets can only be connected to
   * themselves. The other types can be converted between each other. */
  if (ELEM(type_a, SOCK_FLOAT, SOCK_RGBA, SOCK_BOOLEAN, SOCK_INT) &&
      ELEM(type_b, SOCK_FLOAT, SOCK_RGBA, SOCK_BOOLEAN, SOCK_INT)) {
    return true;
  }
  return type_a == type_b;
}

static bool storyboard_node_tree_socket_type_valid(bNodeTreeType *UNUSED(ntreetype),
                                                 bNodeSocketType *socket_type)
{
  return nodeIsStaticSocketType(socket_type) && ELEM(socket_type->type,
                                                     SOCK_FLOAT,
                                                     SOCK_RGBA,
                                                     SOCK_BOOLEAN,
                                                     SOCK_INT,
                                                     SOCK_STRING,
                                                     SOCK_COLLECTION,
                                                     SOCK_IMAGE);
}

void register_node_tree_type_storyboard()
{
  bNodeTreeType *tt = ntreeType_StoryBoard = static_cast<bNodeTreeType *>(
      MEM_callocN(sizeof(bNodeTreeType), "Storyboard node tree type"));
  tt->type = NTREE_STORYBOARD;
  strcpy(tt->idname, "StoryBoardTree");
  strcpy(tt->ui_name, N_("StoryBoard Editor"));
  tt->ui_icon = ICON_MATERIAL_DATA;
  strcpy(tt->ui_description, N_("StoryBoard nodes"));
  tt->rna_ext.srna = &RNA_StoryBoardNodeTree;
  tt->update = storyboard_node_tree_update;
  tt->get_from_context = storyboard_node_tree_get_from_context;
  tt->foreach_nodeclass = foreach_nodeclass;
  tt->valid_socket_type = storyboard_node_tree_socket_type_valid;
  tt->validate_link = storyboard_node_tree_validate_link;

  ntreeTypeAdd(tt);
}
