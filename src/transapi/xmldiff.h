#ifndef _XMLDIFF_H
#define _XMLDIFF_H

#include <stdbool.h>
#include <libxml/tree.h>
#include "yinparser.h"
#include "transapi_internal.h"
#include "../transapi.h"

/**
 * @ingroup transapi
 * @brief buffer for storing callback priorities
 */
struct xmldiff_prio {
	int* values;
	size_t used;
	size_t alloc;
};

/**
 * @ingroup transapi
 * @brief tree structure holding all differencies found in compared files
 */
struct xmldiff_tree {
	char* path;
	xmlNodePtr node;
	XMLDIFF_OP op;

	/*
	 * priority is limited for nodes on the same level, priorities between
	 * different element levels (distance from the root) is irrelevant.
	 */
	int priority;
	/* pointer to the callback connected with this node */
	int (*callback)(void**, XMLDIFF_OP, xmlNodePtr, struct nc_err**);
	CLBCKS_APPLIED applied;

	struct xmldiff_tree* next;
	struct xmldiff_tree* parent;
	struct xmldiff_tree* children;
};

/**
 * @ingroup transapi
 * @brief Destroy and free whole xmldiff structure
 *
 * @param diff	pointer to xmldiff structure
 */
void xmldiff_free (struct xmldiff_tree* diff);

/**
 * @ingroup transapi
 * @brief Module top level function
 *
 * @param old		old version of XML document
 * @param new		new version of XML document
 * @param model	data model in YANG format
 *
 * @return xmldiff structure holding all differences between XML documents or NULL
 */
XMLDIFF_OP xmldiff_diff (struct xmldiff_tree** diff, xmlDocPtr old, xmlDocPtr new, struct model_tree * model);

/**
 * @ingroup transapi
 * @brief this function assigns the callback priority for every change in the tree.
 *		If a change does not have callback, its priority becomes the lowest of
 *		the children priorities.
 * @param tree	difference tree
 * @param callbacks list of transapi callbacks connected with this datastore
 * @param clbk_count Number of callbacks in the callbacks list.
 *
 * @return EXIT_SUCCES on success, EXIT_FAILURE if no callback can
 *		be called for the configuration change
 */
int xmldiff_set_priorities(struct xmldiff_tree* tree, struct clbk *callbacks, int clbk_count);

#endif
