# Speed Outcast

Source port of the singleplayer of Star Wars Jedi Knight II: Jedi Outcast.
Focuses on staying true to the original game to be used for speedrun purposes.
See the [speedrun.com leaderboard](https://www.speedrun.com/jk2) for more information.

Based on the original source code released by Raven, see README.txt.

## New Console Variables

### Input

#### `in_mouse_raw` (0 or 1)

Use raw mouse input. Only applies after an `in_restart` (or after restarting the game).
Default: `1`.

### HUD

#### `cg_drawOverbounceInfo` (0 or 1)

Draw overbounce probabilities, G is for Go, J is for Jump.
Default: `0`.

#### `cg_drawSecrets` (0 or 1)

Show a tracker for found versus total number of secrets on the current level.
Default `0`.

#### `cg_drawServerFPS` (0 or 1)

Show current value of `sv_fps`.
For verification of speedruns for the leaderboards.
Default: `1`.

#### `cg_drawSpeedrunTotalTimer` (0-3)

Draw a total run timer.
Higher values show more decimals.
Default: `0`.

#### `cg_drawSpeedrunLevelTimer` (0-3)

Draw a current level timer.
Higher values show more decimals.
Default: `0`.

#### `cg_drawJumpHelper` (0 or 1)

Draw a helper for correctly timing jump release and crouch jumping.

#### `cg_jumpHelperX` (#)

Horizontal position of the jump helper.
Default: `0.0` (center of the screen).

#### `cg_jumpHelperY` (#)

Vertical position of the jump helper.
Default: `0.0` (center of the screen).

#### `cg_jumpHelperWidth` (#)

Width of the jump helper.
Note that this becomes height when using the vertical jump helper (`cg_jumpHelperHorizontal 0`).
Default: `300`.

#### `cg_jumpHelperHeight` (#)

Height of the jump helper.
Note that this becomes width when using the vertical jump helper (`cg_jumpHelperHorizontal 0`).
Default: `20`.

#### `cg_jumpHelperHorizontal` (-1, 0 or 1)

Whether to draw the jump helper horizontally left-to-right (`1`), horizontally right-to-left (`-1`) or vertically bottom-to-top (`0`).
Default: `1`.

#### `cg_jumpHelperMirror` (0 or 1)

Whether to mirror the jump helper.
Mirrors horizontally when drawing horizontally, vertically otherwise.
Default: `1`.

#### `cg_jumpHelperAutoScale` (0 or 1)

Whether to rescale the jump helper to the same size for each force level.
Default: `0`.

#### `cg_jumpHelperCrouch` (0 or 1)

Show an additional height section for crouch jumping, appended to normal jump height.
Default: `1`.

#### `cg_jumpHelperCurrentScale` (#)

Scale of the indicator for the current height within the jump helper.
Negative values draw from top to bottom, positive values from bottom to top.
Default: `0.5`.

#### `cg_drawSpeed` (0 or 1)

Draw the current player speed.
Default: `0`.

#### `cg_speedScale` (#)

Scale of the text for speed display.
Default: `0.9`.

#### `cg_speedX` (#)

Horizontal offset for drawing speed text, relative to the center of the screen.
Default: `0.0` (center of the screen).

#### `cg_speedY` (#)

Vertical offset for drawing speed text, relative to the center of the screen.
Default: `75.0` (Slightly below center of the screen).

#### `cg_drawStrafeHelper` (0 or 1)

Draw a strafe helper.
Default: `0`.

#### `cg_strafeHelperCenter` (0 or 1)

Put the strafe helper in the center of the screen.
Default: `1`.

#### `cg_strafeHelperCenterMarker` (0 or 1)

Draw a marker in the center of the strafe helper.
Default: `1`.

#### `cg_strafeHelperHeight` (#)

Height of strafe helper bar.
Default: `20.0`.

#### `cg_strafeHelperScale` (#)

Horizontal scale of the strafe helper.
Angles correspond directly to the field of view if this scale is set to `360/cg_fov`.
Default: `4.0`.

#### `cg_strafeHelperSpeedScale` (#)

Scale of the text for speed display tied to strafe helper.
Note that this shows the speed after friction but prior to acceleration, as this is what is relevant for optimal strafing.
It is not the intuitive current player speed.
For that, see `cg_drawSpeed`.
Default: `0.9`.

#### `cg_strafeHelperSpeedX` (#)

Horizontal offset of the text for speed display, relative to the center of the screen.
Default: `0.0` (center of the screen).

#### `cg_strafeHelperSpeedY` (#)

Vertical offset of the text for speed display, relative to the strafe helper.
Default: `0.0` (directly under the strafe helper).

#### `cg_strafeHelperY` (#)

Vertical position of the strafe helper.
Default: `50.0`.

#### `cg_strafeHelperColor[...]` (0.0 - 1.0)

Color components (red, green, blue, alpha) for different strafe helper elements.
These are `Accelerating`, `Optimal`, `CenterMarker` and `Speed`.
Colors can be set more conveniently with the corresponding commands.

### Cosmetics

#### `g_saber_color` (color)

Set saber color.
Valid colors: "red", "orange", "yellow", "green", "blue", and "purple".
Default: "blue"

### Gameplay

#### `g_speedFactorLanding` (0 or 1)

To play with factors to multiply speed by on landing.
A value of `0.5` will obtain Jedi Academy behaviour, `1.0` is the original behaviour in Jedi Outcast.
**Cheat protected**.
Default: `1`.

#### `g_selfKnockback` (0 or 1)

Allow players to damage boost themselves with explosive weapons.
**Cheat protected**.
Default: `0`.

#### `sv_speedrunModeIL` (0 or 1)

Individual-level run mode.
When set to `1`, ingame timer will reset every time an autosave is loaded.
When set to `0`, ingame timer will reset only when `kejim_post` is loaded.
Default: `0`.

#### `sv_speedrunModeCheckpoint` (0 or 1)

Mode for running from quicksave to a specified checkpoint on a map.
Coordinates can be set with the command `settimedcheckpoint x1 y1 z1 x2 y2 z2`.
When the player enters the box spanned by the two points `(x1 y1 z1)` and `(x2 y2 z2)` the run is finished.
If `x2`, `y2` and `z2` are omitted, a checkpoint of reasonable size will be created at `(x1 y1 z1)`.
Default: `0`.

#### `sv_speedrunModeCheckpointSave` (0 or 1)

Create an automatic save when a checkpoint is reached in checkpoint mode.
Default: `0`.

## New Commands

### HUD

#### `jumpHelperColor[...] <r> <g> <b> <a>` (components in range 0.0 to 1.0)

Sets the color for one of the jump helper elements `Background`, `Crouch`, `CrouchExtend`, `Extend`, `Optimal`, `Current` or `Cancel` to the given red, green, blue and alpha values.

#### `speedColor <r> <g> <b> <a>` (components in range 0.0 to 1.0)

Sets the color for the speed display to the given red, green, blue and alpha values.

#### `strafeHelperColor[...] <r> <g> <b> <a>` (components in range 0.0 to 1.0)

Sets the color for one of the strafe helper elements `Accelerating`, `Optimal`, `CenterMarker` or `Speed` to the given red, green, blue and alpha values.
