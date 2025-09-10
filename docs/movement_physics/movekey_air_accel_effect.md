# Movement Key Effect on Air Acceleration

In vanilla Quake 3 you can accelerate basically just as well with a single input key (i.e. only A or only D) than with the usual combination of two keys (W+A or W+D).
But in the Jedi Outcast singleplayer, strafing with only a single key is significantly slower while in the air.
On the other hand, acceleration in Quake 3 is significantly slower when holding the jump key while in the air, whereas it has no influence whatsoever in Jedi Outcast.

Jedi Outcast inherits this behavior directly from its engine predecessor *Star Trek: Voyager - Elite Force*.
So the technical explanation in its Elite-Reinforce source port ([kugelrund/Elite-Reinforce/docs/movekey_air_accel_effect.md](https://github.com/kugelrund/Elite-Reinforce/blob/master/docs/movement_physics/movekey_air_accel_effect.md)) applies to Jedi Outcast too, with the relevant code snippets looking very much the same.

Just like in Elite Force, the line `wishspeed *= scale` is missing in `PM_AirMove` ([jedioutcast/code/game/bg_pmove.cpp#L1759](https://github.com/kugelrund/Speed-Outcast/blob/1963e9b17e0b3b897bb44a0bc4901a8cad007ff4/code/game/bg_pmove.cpp#L1759)), with the variable `scale` still being set ([jedioutcast/code/game/bg_pmove.cpp#L1734](https://github.com/kugelrund/Speed-Outcast/blob/1963e9b17e0b3b897bb44a0bc4901a8cad007ff4/code/game/bg_pmove.cpp#L1734)) but remaining unused.
Other functions like `PM_WalkMove` do apply the scaling ([jedioutcast/code/game/bg_pmove.cpp#L1936](https://github.com/kugelrund/Speed-Outcast/blob/1963e9b17e0b3b897bb44a0bc4901a8cad007ff4/code/game/bg_pmove.cpp#L1936)).
The added deceleration scaling is also identical ([jedioutcast/code/game/bg_pmove.cpp#L1771](https://github.com/kugelrund/Speed-Outcast/blob/1963e9b17e0b3b897bb44a0bc4901a8cad007ff4/code/game/bg_pmove.cpp#L1771)).
