Findings on this Randomizer : 
- Friendly and neutral NPC carry necessary items (for exemple, the key on kejim_base).
- Some NPC have to be locked, otherwise, we might get undefined behavior in cutscene or else.
- In ns_streets, no matter which npc (except the ones in the little arena) are forced on a path.
- IMPOSSIBLE to start a new game. 
- NPC Spawn are determined at the start of the map, not when you hit triggers. If so, then why does it lag ? (Other are lagging, I'm not).

TODO : 
- Lock all necessarry NPC per map (right now thinking about undercity and assembly, to open doors).
- Fixing G_FindConfigstringIndex (very hard).
- Stop the crashes (very hard).
- Maybe add some kind of items randomize ? Except the necessarry ones ofc.
- Some NPC don't spawn, maybe it's an entity not finished like monster_fish. Need to lock them out.

List of non-working NPC :
- monster_flier2
- monster_fish
- claw
- lizard
- swamp
- murjj

Crash : common.cpp, line 296
It happens because of G_FindConfigstringIndex each time it seems.
I don't know how to fix that, because if it's truly this case every time, it could be a cool mod :/
q_shared : upped the max sounds and models.
I mean, it works, but limiting the number of different kind of NPC is the better idea.

For items :
classname : item_bacta, origin : X,Y,Z

G_CreateG2AttachedWeaponModel : assert at line 249 crashes
You can't give yourself a lightsaber before trial ????
It makes the game crash for some reason
In trial : models/wepaons2/saber/saber_w.glm
In kejim_base : NULL, unable to read memory
In ns_street : models/weapons2/saber/saber_w.glm, so the same as in trial
g_utils.cpp line 1001 eventParm < 256
noweap.md3 ?????????

Idea : Lock it to 10 MAX, so that 1=Kyle, 2=NeededNPC, 3=NeededNPC, 4=maybe needednpc or a random one, 5+ = the rest
Here is what I could grab as the max number of different NPC on a map in Vanilla (thinking about big map) : 
- Artus_detention : Stormtrooper,Imperial,DroidGonk,Prisoner,MineMonster,R5D2,Kyle,Droid_Sentry, a total of 8, Kyle included
- Bespin_undercity : Kyle,Ugnaught,Rodian,R5D2,Gran,Weequay,Reborn, a total of 7, Kyle included (less than I expected


Needed NPC per map :
- None (but need confirmation): kejim_post, kejim_base, artus_mine, yavin_trial, cairn_bay, cairn_reactor, doom_comm, doom_shield, yavin_swamp, yavin_canyon, 
- artus_detention : the officer with the loooong sequence
- artus_topside : the prisoners, Desann spawn naturally
- valley : maybe ?????
- yavin_temple : maybe Luke or C3P0 ????? need to confirm
- ns_street : Bartender, but it's already forced. Also the entity that takes away the briar blaster need to be 'humanoid' or the game will crash, need to check that.
- ns_hideout : Lando and Reelo, but both are already forced
- ns_starpas : Lando and maybe Reelo
- bespin_undercity : the guy who opens the door, and R5D2 are forced (need to confirm)
- bespin_streets : R5D2 already forced
- bespin_platform : Tavion is forced
- cairn_assembly : The officer that open the door, maybe something else
- cairn_dock1 : maybe forcing everything here, because random NPC behavior are strange, also Luke and Desann
- doom_detention : the officer that open the door and Jan
- yavin_courtyard : maybe locking everything here (the trooper shoioting the rebel, the jedis are maybe necessary to unlock the scene)


CURRENT SITUATION : I locked the number of max unique NPC to 10 for now.
In DEBUG, the game is way less permissing and throw errors often.
But in RELEASE ? It seems plyable, I will do a full playthrought to confirm that.

I DID an entire playthought without ATST, and a max of 10 random unique NPC, here is a list of softlock I had  : 
Chicken robots are really too big, I think we should make them spawn naturally and not randomly.
- Two or more NPC spawn location being too close is a problem, if for exemple ATSTs tried to spawn, only one will THEN the other when there is enough space
- kejim_base : the NPC that spawned behind the door couldn't spawn, it was a big droid and Reelo. Making it impossible for the cutscene to execute
- artus_detention :
NPC will NOT respect that truce when the guard is activated, not really a softlock but careful. Also, any NPC can trigger the guard, not only Kyle.
The guard being too big will not work.
Fortunatly, no matter which team the NPC spawned, it will be overwritten and will always be havin the same path and behavior as the guardian.
- artus_topside :
Didn't manage to load the map under unknown conditions
The path finding of NPC that spawn instead of the stormtrooper seems to always be broken
- ns_starpad crashes during the final against Reelo, I should retry this
- cairn_assembly : if the imperial opening the door is too small (for exmeple a small droid), you can't reach the hitbox to hit persuasion
If the NPC remplacin the two reborn dies or don't spawn (don't know which), the ATST door won't open.
- cairn_dock : the map is imposible to play because the new NPCs are having undefefined behavior, making the discretion impossible and you are spoted instantly when by the window
The shadowtropper have to spawn as a shadowtrooper I think because the platform where it spawns DON'T ELEVATE other NPCs (humanoid or droids)
- doom_detention : I didn't any problem with the guard opening the doors, but there might be the same problem as cairn_assembly



