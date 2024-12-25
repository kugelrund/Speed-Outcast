//g_inventory.cpp

// leave this line at the top for all g_xxxx.cpp files...
#include "g_headers.h"

#include "g_local.h"

// Randomizer
extern	vmCvar_t		cg_enableRandomizer;

/*
================
Goodie Keys
================
*/
qboolean INV_GoodieKeyGive( gentity_t *target )
{
	if ( !target || !target->client )
	{
		return (qfalse);
	}

	target->client->ps.inventory[INV_GOODIE_KEY]++;
	return (qtrue);

}

qboolean INV_GoodieKeyTake( gentity_t *target )
{
	if ( !target || !target->client )
	{
		return (qfalse);
	}

	if (target->client->ps.inventory[INV_GOODIE_KEY])
	{
		target->client->ps.inventory[INV_GOODIE_KEY]--;
		return (qtrue);
	}

	//had no keys
	return (qfalse);
}

int INV_GoodieKeyCheck( gentity_t *target )
{
	if ( !target || !target->client )
	{
		return (qfalse);
	}

	if ( target->client->ps.inventory[INV_GOODIE_KEY] )
	{//found a key
		return (INV_GOODIE_KEY);
	}

	//no keys
	return (qfalse);
}

/*
================
Security Keys
================
*/
qboolean INV_SecurityKeyGive( gentity_t *target, const char *keyname )
{
	if (!cg_enableRandomizer.integer)
	{
		if (target == NULL || keyname == NULL || target->client == NULL)
		{
			return qfalse;
		}

		for (int i = 0; i <= 4; i++)
		{
			if (target->client->ps.security_key_message[i][0] == NULL)
			{//fill in the first empty slot we find with this key
				target->client->ps.inventory[INV_SECURITY_KEY]++;	// He got the key
				Q_strncpyz(target->client->ps.security_key_message[i], keyname, MAX_SECURITY_KEY_MESSSAGE, qtrue);
				return qtrue;
			}
		}
		//couldn't find an empty slot
		return qfalse;
	}
	else // Randomizer, make a random taken key universally usable
	{
		if (keyname != NULL) // Normal key, normal behavior
		{
			for (int i = 0; i <= 4; i++)
			{
				if (target->client->ps.security_key_message[i][0] == NULL)
				{//fill in the first empty slot we find with this key
					target->client->ps.inventory[INV_SECURITY_KEY]++;	// He got the key
					Q_strncpyz(target->client->ps.security_key_message[i], keyname, MAX_SECURITY_KEY_MESSSAGE, qtrue);
					return qtrue;
				}
			}
			//couldn't find an empty slot
			return qfalse;
		}
		else // Randomly spawned key
		{
			for (int i = 0; i <= 4; i++)
			{
				if (target->client->ps.security_key_message[i][0] == NULL)
				{//fill in the first empty slot we find with this key
					target->client->ps.inventory[INV_SECURITY_KEY]++;	// He got the key
					Q_strncpyz(target->client->ps.security_key_message[i], "rando_key", MAX_SECURITY_KEY_MESSSAGE, qtrue);
					return qtrue;
				}
			}
			//couldn't find an empty slot
			return qfalse;
		}
	}
	//couldn't find an empty slot
	return qfalse;
}

void INV_SecurityKeyTake( gentity_t *target, char *keyname )
{
	if (!cg_enableRandomizer.integer)
	{
		if (!target || !keyname || !target->client)
		{
			return;
		}
		for (int i = 0; i <= 4; i++)
		{
			if (target->client->ps.security_key_message[i])
			{
				if (!Q_stricmp(keyname, target->client->ps.security_key_message[i]))
				{
					target->client->ps.inventory[INV_SECURITY_KEY]--;	// Take the key
					target->client->ps.security_key_message[i][0] = NULL;
					return;
				}
			}
			/*
			//don't do this because we could have removed one that's between 2 valid ones
			else
			{
				break;
			}
			*/
		}
	}
	else // Randomizer, make a random taken key universally usable
	{
		for (int i = 0; i <= 4; i++) // Check for normal key first
		{
			if (target->client->ps.security_key_message[i])
			{
				if (!Q_stricmp(keyname, target->client->ps.security_key_message[i]))
				{
					target->client->ps.inventory[INV_SECURITY_KEY]--;	// Take the key
					target->client->ps.security_key_message[i][0] = NULL;
					return;
				}
			}
		}
		for (int i = 0; i <= 4; i++) // Check for rando_key
		{
			if (target->client->ps.security_key_message[i])
			{
				if (!Q_stricmp("rando_key", target->client->ps.security_key_message[i]))
				{
					target->client->ps.inventory[INV_SECURITY_KEY]--;	// Take the key
					target->client->ps.security_key_message[i][0] = NULL;
					return;
				}
			}
		}
	}
	
}

qboolean INV_SecurityKeyCheck( gentity_t *target, char *keyname )
{
	if (!cg_enableRandomizer.integer)
	{
		if (!target || !keyname || !target->client)
		{
			return (qfalse);
		}
		for (int i = 0; i <= 4; i++)
		{
			if (target->client->ps.inventory[INV_SECURITY_KEY] && target->client->ps.security_key_message[i])
			{
				if (!Q_stricmp(keyname, target->client->ps.security_key_message[i]))
				{
					return (qtrue);
				}
			}
			/*
			//don't do this because we could have removed one that's between 2 valid ones
			else
			{
				break;
			}
			*/
		}
	}
	else // Randomizer, make a random taken key universally usable
	{
		for (int i = 0; i <= 4; i++) // Check for normal key first
		{
			if (target->client->ps.inventory[INV_SECURITY_KEY] && target->client->ps.security_key_message[i])
			{
				if (!Q_stricmp(keyname, target->client->ps.security_key_message[i]))
				{
					return (qtrue);
				}
			}
		}
		for (int i = 0; i <= 4; i++) // Check for rando_key
		{
			if (target->client->ps.inventory[INV_SECURITY_KEY] && target->client->ps.security_key_message[i])
			{
				if (!Q_stricmp("rando_key", target->client->ps.security_key_message[i]))
				{
					return (qtrue);
				}
			}
		}

	}
	return (qfalse);
}
