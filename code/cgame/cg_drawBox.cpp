// cg_drawBox.cpp
#include "cg_drawBox.h"
#include "cg_headers.h"
#include "cg_media.h"

/*
Tool used to be able to draw boxes for triggers, NPCs and items.
Application : routing all secrets in Outcast and inspecting trigger range ingame instead of using an external mapping tool.

Still need improvement on colors / differentiation, white being the default as of now.
Shouldn't make the game heavily drop in fps if enabled, so even less when disabled (except some bool and int checks)

Entrypoint :
- CG_DrawActiveFrame : list every cgentity once per frame, and send their pointer here to prepare for rendering when conditions are met

Related variables :
- cg_drawBoxTriggers : 0 or 1 (any int not 0) : draw in different colors (ex : pink or orange for secrets) triggers around the map
- cg_drawBoxPlayer : 0 or 1 (any int not 0) : draw in RED, the box around the player
- cg_drawBoxPlayerFP : 0 or 1 (any int not 0) : allows rendering if the box even in first person if set to 1 or more
- cg_drawBoxNPC : 0 or 1 (any int not 0) : draw in GREEN, the boxes around NPCs (include spawned NPCs)
- cg_drawBoxItems : 0 or 1 (any int not 0) : draw in BLUE, the boxes around items (include dropped weapons)

Posto
*/

static void drawBoundingBox(const gentity_t* ent, const byte color[4])
{
	polyVert_t vertices[4];
	for (int i = 0; i < 4; ++i)
	{
		vertices[i].modulate[0] = color[0];
		vertices[i].modulate[1] = color[1];
		vertices[i].modulate[2] = color[2];
		vertices[i].modulate[3] = color[3];
		vertices[i].st[0] = 0.0f;
		vertices[i].st[1] = 0.0f;
	}

	for (int axis_fixed = 0; axis_fixed < 3; ++axis_fixed)
	{
		const int axis_prev = (axis_fixed == 0) ? 2 : (axis_fixed - 1);
		const int axis_next = (axis_fixed == 2) ? 0 : (axis_fixed + 1);

		vertices[0].xyz[axis_prev] = ent->absmin[axis_prev];
		vertices[0].xyz[axis_next] = ent->absmin[axis_next];

		vertices[1].xyz[axis_prev] = ent->absmax[axis_prev];
		vertices[1].xyz[axis_next] = ent->absmin[axis_next];

		vertices[2].xyz[axis_prev] = ent->absmax[axis_prev];
		vertices[2].xyz[axis_next] = ent->absmax[axis_next];

		vertices[3].xyz[axis_prev] = ent->absmin[axis_prev];
		vertices[3].xyz[axis_next] = ent->absmax[axis_next];

		for (int i = 0; i < 4; ++i) 
			vertices[i].xyz[axis_fixed] = ent->absmin[axis_fixed];
		cgi_R_AddPolyToScene(cgs.media.solidWhiteShader, 4, vertices);
		for (int i = 0; i < 4; ++i)
			vertices[i].xyz[axis_fixed] = ent->absmax[axis_fixed];
		cgi_R_AddPolyToScene(cgs.media.solidWhiteShader, 4, vertices);
	}
}

// Draws a rectangle given the center of two opposite end faces.
static void drawLine(vec3_t p1, vec3_t p2, byte color[4], float width, bool onFirstHit)
{
	vec3_t line, up, perp1, perp2;
	const float half = width * 0.5f;
	trace_t trace;

	// Copied from CG_ScanForCrosshairEntity
	memset(&trace, 0, sizeof(trace));
	if (onFirstHit)
	{
		gi.trace(&trace, p1, vec3_origin, vec3_origin, p2,
			0, MASK_OPAQUE | CONTENTS_SHOTCLIP | CONTENTS_BODY | CONTENTS_ITEM | CONTENTS_TERRAIN, G2_NOCOLLIDE, 10);
	}
	else
	{
		gi.trace(&trace, p1, vec3_origin, vec3_origin, p2,
			0, 0, G2_NOCOLLIDE, 10);
	}
	VectorCopy(trace.endpos, p2);
	

	// Build line from end-start
	VectorSubtract(p2, p1, line);
	VectorNormalize(line);

	// I hate the fact that I don't know maths, but maths is cool and good
	// Build two axes perpendicular to the beam (orthonormal basis)
	if (fabsf(line[2]) < 0.9f)
		VectorSet(up, 0.0f, 0.0f, 1.0f);
	else
		VectorSet(up, 1.0f, 0.0f, 0.0f);

	CrossProduct(line, up, perp1);
	VectorNormalize(perp1);
	CrossProduct(line, perp1, perp2); // already unit length

	// From drawBoundingBox
	polyVert_t verts[4];
	for (int i = 0; i < 4; i++) {
		verts[i].modulate[0] = color[0];
		verts[i].modulate[1] = color[1];
		verts[i].modulate[2] = color[2];
		verts[i].modulate[3] = color[3];
		verts[i].st[0] = 0.0f;
		verts[i].st[1] = 0.0f;
	}

	// Posto : yeah I'll let those who know decode this, I unfortunatly suck in maths.
	// 8 corners: 2 faces x 4 vertices, wound CCW when viewed from outside
	//   signs: (++), (+-), (--), (-+)  in (perp1, perp2)
	static const float signs[4][2] = { { 1, 1}, { 1,-1}, {-1,-1}, {-1, 1} };
	vec3_t corners[2][4];
	for (int fi = 0; fi < 2; fi++) {
		const float* center = (fi == 0) ? p1 : p2;
		for (int vi = 0; vi < 4; vi++) {
			corners[fi][vi][0] = center[0] + signs[vi][0] * half * perp1[0] + signs[vi][1] * half * perp2[0];
			corners[fi][vi][1] = center[1] + signs[vi][0] * half * perp1[1] + signs[vi][1] * half * perp2[1];
			corners[fi][vi][2] = center[2] + signs[vi][0] * half * perp1[2] + signs[vi][1] * half * perp2[2];
		}
	}

	// 2 end caps
	for (int fi = 0; fi < 2; fi++) {
		for (int vi = 0; vi < 4; vi++)
			VectorCopy(corners[fi][vi], verts[vi].xyz);
		cgi_R_AddPolyToScene(cgs.media.solidWhiteShader, 4, verts);
	}

	// 4 side faces
	for (int i = 0; i < 4; i++) {
		const int next = (i + 1) % 4;
		VectorCopy(corners[0][i], verts[0].xyz);
		VectorCopy(corners[0][next], verts[1].xyz);
		VectorCopy(corners[1][next], verts[2].xyz);
		VectorCopy(corners[1][i], verts[3].xyz);
		cgi_R_AddPolyToScene(cgs.media.solidWhiteShader, 4, verts);
	}
}

// Draws a cube given it's center. Mostly copied from drawBoundingBox
static void drawCubeFromCenter(vec3_t center, byte color[4], float width, float deltaZ)
{
	const float half = width * 0.5f;
	vec3_t absmin, absmax;
	
	byte empty[4] = { 0, 0, 0, 25 };
	if (color == empty) // We didn't override the empty color : return early and don't draw stuff
	{
		return;
	}

	VectorSet(absmin, center[0] - half, center[1] - half, center[2] - half);
	VectorSet(absmax, center[0] + half, center[1] + half, center[2] + half);

	polyVert_t vertices[4];
	for (int i = 0; i < 4; i++) {
		vertices[i].modulate[0] = color[0]; vertices[i].modulate[1] = color[1];
		vertices[i].modulate[2] = color[2]; vertices[i].modulate[3] = color[3];
		vertices[i].st[0] = 0.0f;           vertices[i].st[1] = 0.0f;
	}

	for (int axis_fixed = 0; axis_fixed < 3; axis_fixed++) {
		const int axis_prev = (axis_fixed == 0) ? 2 : (axis_fixed - 1);
		const int axis_next = (axis_fixed == 2) ? 0 : (axis_fixed + 1);

		vertices[0].xyz[axis_prev] = absmin[axis_prev]; vertices[0].xyz[axis_next] = absmin[axis_next];
		vertices[1].xyz[axis_prev] = absmax[axis_prev]; vertices[1].xyz[axis_next] = absmin[axis_next];
		vertices[2].xyz[axis_prev] = absmax[axis_prev]; vertices[2].xyz[axis_next] = absmax[axis_next];
		vertices[3].xyz[axis_prev] = absmin[axis_prev]; vertices[3].xyz[axis_next] = absmax[axis_next];

		for (int i = 0; i < 4; i++)
			vertices[i].xyz[axis_fixed] = absmin[axis_fixed];
		cgi_R_AddPolyToScene(cgs.media.solidWhiteShader, 4, vertices);
		for (int i = 0; i < 4; i++)
			vertices[i].xyz[axis_fixed] = absmax[axis_fixed];
		cgi_R_AddPolyToScene(cgs.media.solidWhiteShader, 4, vertices);
	}
}

static void drawLineOfSight(const gentity_t* ent, float width)
{
	const float half = width * 0.5f;
	byte color[4] = { 0, 0, 25, 25 };
	vec3_t p1, p2;
	trace_t trace;
	vec3_t d_f, d_rt, d_up;

	// Set the first point to be where the entity is looking at (eye) and the end where it's looking at (direction_forward)
	VectorCopy(ent->client->renderInfo.eyePoint, p1);
	AngleVectors(ent->currentAngles, d_f, d_rt, d_up);
	// Copied from CG_ScanForCrosshairEntity
	VectorMA(p1, 4096, d_f, p2);

	drawLine(p1, p2, color, width, true);
}

static void drawAggroCubes(gentity_t * self)
{
	// Width of 16, and deltaZ of 16, can be adjusted later of needs be
	/*
	drawCubeFromCenter(self, 10, 16, 1);
	drawCubeFromCenter(self, 10, 32, 2);
	drawCubeFromCenter(self, 10, 48, 3);
	drawCubeFromCenter(self, 10, 64, 4);
	drawCubeFromCenter(self, 10, 80, 5);
	drawCubeFromCenter(self, 10, 96, 6);
	*/
	vec3_t center;
	byte color[4] = { 0, 0, 0, 25 };

	VectorAdd(self->absmin, self->absmax, center);
	VectorScale(center, 0.5, center);
	// Add 16 units in the Z axis of the center of an NPC, above their max height of the bounding box.

	// ent->NPC should always be a valid pointer since only NPCs entities will reach this function
	if (self->NPC)
	{
		// Let's just call this function with a 'mode' parameter, we just want cubes 
		// Just remember to also change deltaZ so that the multiple cubes don't override eachother
		switch (self->NPC->behaviorState)
		{
			case BS_DEFAULT:
				break;
			case BS_ADVANCE_FIGHT:
				break;
			case BS_SLEEP:
				break;
			case BS_FOLLOW_LEADER:
				break;
			case BS_JUMP:
				break;
			case BS_SEARCH:
				break;
			case BS_WANDER:
				break;
			case BS_NOCLIP:
				break;
			case BS_REMOVE:
				break;
			case BS_CINEMATIC:
				break;
			case BS_WAIT:
				break;
			case BS_STAND_GUARD:
				break;
			case BS_PATROL:
				break;
			case BS_INVESTIGATE:
				break;
			case BS_STAND_AND_SHOOT:
				break;
			case BS_HUNT_AND_KILL:
				break;
			case BS_FLEE:
				break;
			default: // REMOVE ME
				color[0] = 25;
				color[1] = 25;
				color[2] = 25;
				break;

			
		}
		//drawCubeFromCenter(vec3_t center, byte color[4], float width, float deltaZ);
		//color = [0, 0, 0, 25];
		
		switch (self->NPC->aiFlags)
		{
			case NPCAI_CHECK_WEAPON:
				break;
			case NPCAI_BURST_WEAPON:
				break;
			case NPCAI_MOVING:
				break;
			case NPCAI_TOUCHED_GOAL:
				break;
			case NPCAI_PUSHED:
				break;
			case NPCAI_NO_COLL_AVOID:
				break;
			case NPCAI_BLOCKED:
				break;
			case NPCAI_OFF_PATH:
				break;
			case NPCAI_IN_SQUADPOINT:
				break;
			case NPCAI_STRAIGHT_TO_DESTPOS:
				break;
			case NPCAI_NO_SLOWDOWN:
				break;
			case NPCAI_LOST:
				break;
			case NPCAI_SHIELDS:
				break;
			case NPCAI_GREET_ALLIES:
				break;
			case NPCAI_FORM_TELE_NAV:
				break;
			case NPCAI_ENROUTE_TO_HOMEWP:
				break;
			case NPCAI_MATCHPLAYERWEAPON:
				break;
			case NPCAI_DIE_ON_IMPACT:
				break;
			default: // REMOVE ME
				color[0] = 25;
				color[1] = 25;
				color[2] = 25;
				break;
		}
		//drawCubeFromCenter(vec3_t center, byte color[4], float width, float deltaZ);
		//color = [0, 0, 0, 25];
		
		switch (self->NPC->squadState)
		{
			case SQUAD_IDLE:
				break;
			case SQUAD_STAND_AND_SHOOT:
				break;
			case SQUAD_RETREAT:
				break;
			case SQUAD_COVER:
				break;
			case SQUAD_TRANSITION:
				break;
			case SQUAD_POINT:
				break;
			case SQUAD_SCOUT:
				break;
			default: // REMOVE ME
				color[0] = 25;
				color[1] = 25;
				color[2] = 25;
				break;
		}
		//drawCubeFromCenter(vec3_t center, byte color[4], float width, float deltaZ);
	}
}

static void drawNPCPathing(gentity_t* self)
{
	vec3_t centerNPC;
	vec3_t centerGoal;
	byte color[4] = { 25, 25, 0, 25 };

	if (self->NPC && self->NPC->goalEntity)
	{
		VectorAdd(self->absmin, self->absmax, centerNPC);
		centerNPC[2] -= 20; // To start a little big lower
		VectorScale(centerNPC, 0.5, centerNPC);
		VectorAdd(self->NPC->goalEntity->absmin, self->NPC->goalEntity->absmax, centerGoal);
		VectorScale(centerGoal, 0.5, centerGoal);

		drawLine(centerNPC, centerGoal, color, 4, false);
		drawCubeFromCenter(centerGoal, color, 10, 0);
	}
}

static void setColorForTrigger(gentity_t* self, byte color[4])
{
	gentity_t* subTrigger = NULL;

	//while ((subTrigger = G_Find(subTrigger, FOFS(targetname), self->target)) != NULL)
	if ((subTrigger = G_Find(subTrigger, FOFS(targetname), self->target)) != NULL)
	{
		switch (subTrigger->e_UseFunc)
		{
			// CASE : spawns, in orange. Can be seen everywhere.
		case(useF_NPC_Spawn):
		case(useF_item_spawn_use):
			color[0] = 100;
			color[1] = 50;
			color[2] = 0;
			break;
			// CASE : world/save/map related, in pink.
		case(useF_target_autosave_use):
		case(useF_target_level_change_use):
		case(useF_target_secret_use):
			color[0] = 100;
			color[1] = 0;
			color[2] = 50;
			break;
			// CASE : interactible elements, in green.
		case(useF_security_panel_use):
		case(useF_misc_model_use):
		case(useF_Use_Item):
		case(useF_Use_Shooter):
		case(useF_funcBBrushUse):
		case(useF_target_activate_use):
		case(useF_target_deactivate_use):
		case(useF_health_use):
		case(useF_ammo_use):
		case(useF_mega_ammo_use):
		case(useF_turret_base_use):
		case(useF_laser_arm_use):
		case(useF_emplaced_gun_use):
		case(useF_shield_power_converter_use):
		case(useF_ammo_power_converter_use):
		case(useF_camera_use):
		case(useF_sentry_use):
		case(useF_misc_atst_use):
		case(useF_panel_turret_use):
		case(useF_misc_dlight_use):
		case(useF_TrainUse):
			//case(useF_bomb_planted_use):
		case(useF_ion_cannon_use):
		case(useF_spotlight_use):
		case(useF_welder_use):
		case(useF_jabba_cam_use):
		case(useF_misc_use):
		case(useF_pas_use):
			color[0] = 0;
			color[1] = 100;
			color[2] = 0;
			break;
			// CASE : 'scripts' / 'func', in greenish cyan.
		case(useF_Use_Multi):
		case(useF_func_usable_use):
		case(useF_trigger_entdist_use):
		case(useF_target_change_parm_use):
		case(useF_func_static_use):
		case(useF_func_bobbing_use):
		case(useF_func_rotating_use):
		case(useF_funcGlassUse):
		case(useF_func_timer_use):
			color[0] = 0;
			color[1] = 100;
			color[2] = 50;
			break;
			// CASE : 'target' ??????, in purple. Note : not found anything is purple during tests
		case(useF_Use_Target_Give):
		case(useF_Use_Target_Delay):
		case(useF_Use_Target_Score):
		case(useF_Use_Target_Print):
		case(useF_Use_Target_Speaker):
		case(useF_target_laser_use):
		case(useF_target_relay_use):
		case(useF_target_kill_use):
		case(useF_target_counter_use):
		case(useF_target_random_use):
		case(useF_target_scriptrunner_use):
		case(useF_target_gravity_change_use):
		case(useF_target_friction_change_use):
		case(useF_target_teleporter_use):
		case(useF_Use_target_push):
			color[0] = 100;
			color[1] = 0;
			color[2] = 100;
			break;
			// CASE : effects fx & sound, in yellow
		case(useF_fx_runner_use):
		case(useF_fx_explosion_trail_use):
		case(useF_fx_target_beam_use):
		case(useF_target_play_music_use):
			color[0] = 100;
			color[1] = 66;
			color[2] = 0;
			break;
			// CASE : unknown cases, let it be white
		case(useF_GoExplodeDeath):
		case(useF_Use_BinaryMover):
		case(useF_use_wall):
		case(useF_hurt_use):
		case(useF_NPC_Use):
		case(useF_crew_beam_in_use):
		case(useF_misc_model_useup):
		case(useF_misc_portal_use):
		case(useF_switch_models):
		case(useF_misc_replicator_item_spawn):
		case(useF_misc_replicator_item_remove):
		case(useF_misc_trip_mine_activate):
		default:
			color[0] = 20;
			color[1] = 20;
			color[2] = 20;
			break;
		}
		color[3] = 128;
	}
}

static void drawBoxPlayer(gentity_t* self)
{
	if (cg_drawBoxPlayer.integer && (cg.renderingThirdPerson || (!cg.renderingThirdPerson && cg_drawBoxPlayerFP.integer)))
	{
		// Make it red and semi-transparent
		byte color[4];
		color[0] = 50;
		color[1] = 0;
		color[2] = 0;
		color[3] = 25;

		drawBoundingBox(self, color);
	}
	if (cg_drawLineOfSight.integer && cg.renderingThirdPerson)
		// Contrary to trigger box, I don't want this rectangle to follow where I'm looking at
	{
		// Width of 4, can be adjusted later of needs be
		drawLineOfSight(self, 4);
	}
}

static void drawBoxNPC(gentity_t* self)
{
	if (cg_drawBoxNPC.integer)
	{
		// Make it green and semi-transparent
		byte color[4];
		color[0] = 0;
		color[1] = 50;
		color[2] = 0;
		color[3] = 25;

		drawBoundingBox(self, color);
	}
	if (cg_drawLineOfSight.integer)
	{
		// Width of 4, can be adjusted later of needs be
		drawLineOfSight(self, 4);
	}
	if (cg_drawBoxAggro.integer)
	{
		drawAggroCubes(self);
	}
	if (cg_drawNPCPath.integer)
	{
		drawNPCPathing(self);
	}
}

static void drawBoxItems(gentity_t* self)
{
	// Make it blue and semi-transparent
	byte color[4];
	color[0] = 0;
	color[1] = 0;
	color[2] = 50;
	color[3] = 25;

	drawBoundingBox(self, color);
}

static void drawBoxWorldTriggers(gentity_t* self)
{
	// Default color: blue
	byte color[4];
	color[0] = 0;
	color[1] = 0;
	color[2] = 100;
	color[3] = 25;

	// Override for some trigger in other color (ex : secrets)
	// Do not use blue nor red to know we correctly override the previous color.
	setColorForTrigger(self, color);

	// At the end, if the trigger has been used, display it in red
	if (self->e_TouchFunc == touchF_NULL)
	{
		// Red for deactivated triggers
		color[0] = 50;
		color[1] = 0;
		color[2] = 0;
		color[3] = 25;
	}

	drawBoundingBox(self, color);
}

static void drawBoxObjectTriggers(gentity_t* self)
{
	// Default color: blue
	byte color[4];
	color[0] = 0;
	color[1] = 0;
	color[2] = 100;
	color[3] = 25;

	// Change in other color (ex : secrets). The first secret in t1_fatal is obtained by destroying a 3D object, it will be colored in pink.
	// Do not use blue nor red to know we correctly override the previous color.
	setColorForTrigger(self, color);

	// Default color = no trigger found for this object : don't render the box
	if (color[0] == 0 && color[1] == 0 && color[2] == 100 && color[3] == 25)
	{
		return;
	}

	drawBoundingBox(self, color);
}

void CG_DrawBoxes()
{
	if (!cg_drawBoxPlayer.integer && !cg_drawBoxNPC.integer && !cg_drawBoxItems.integer && !cg_drawBoxTriggers.integer && !cg_drawBoxAggro.integer && !cg_drawLineOfSight.integer) {
		// No need to do the loop if none of these is enabled
		return;
	}

	for (int i = 0; i < MAX_GENTITIES; ++i)
	{
		// Player
		if (i == 0 && (cg_drawBoxPlayer.integer || cg_drawLineOfSight.integer))
		{
			// Player is always the first on this array
			drawBoxPlayer(&g_entities[i]);
		}
		// NPCs
		if ((cg_drawBoxNPC.integer || cg_drawLineOfSight.integer || cg_drawBoxAggro.integer || cg_drawNPCPath.integer) && g_entities[i].e_ThinkFunc == thinkF_NPC_Think)
		{
			drawBoxNPC(&g_entities[i]);
		}
		// Items
		else if (cg_drawBoxItems.integer && g_entities[i].e_TouchFunc == touchF_Touch_Item)
		{
			drawBoxItems(&g_entities[i]);
		}
		// Triggers, but related to the world (not associated with an ingame object like a button or a camera)
		else if (cg_drawBoxTriggers.integer && g_entities[i].classname &&
			(strcmp(g_entities[i].classname, "trigger_multiple") == 0 ||
				strcmp(g_entities[i].classname, "trigger_once") == 0))
		{
			drawBoxWorldTriggers(&g_entities[i]);
		}
		// Triggers, but related to an object like a button or a camera
		else if (cg_drawBoxTriggers.integer && g_entities[i].classname &&
			(strncmp(g_entities[i].classname, "func_", strlen("func_")) == 0 ||
				strncmp(g_entities[i].classname, "misc_", strlen("misc_")) == 0))
		{
			drawBoxObjectTriggers(&g_entities[i]);
		}
	}
}