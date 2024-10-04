Findings on this Randomizer : 
- Friendly and neutral NPC carry necessary items (for exemple, the key on kejim_base).
- Some NPC have to be locked, otherwise, we might get undefined behavior in cutscene or else.
- In ns_streets, no matter which npc (except the ones in the little arena) are forced on a path.
- IMPOSSIBLE to start a new game. 

TODO : 
- Lock all necessarry NPC per map (right now thinking about undercity and assembly, to open doors).
- Fixing G_FindConfigstringIndex (very hard).
- Stop the crashes (very hard).
- Maybe add some kind of items randomize ? Except the necessarry ones ofc.

Crash : common.cpp, line 296
It happens because of G_FindConfigstringIndex each time it seems.
I don't know how to fix that, because if it's truly this case every time, it could be a cool mod :/
q_shared : upped the max sounds and models.

For items :
classname : item_bacta, origin : X,Y,Z