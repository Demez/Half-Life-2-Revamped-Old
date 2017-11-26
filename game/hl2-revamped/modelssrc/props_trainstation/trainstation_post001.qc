// Created by Crowbar 0.49.0.0

$modelname "props_trainstation/trainstation_post001.mdl"

$staticprop

$bodygroup "Body"
{
	studio "trainstation_post001.smd"
}


$surfaceprop "metal"

$contents "solid"

// Only set this if you know what it does, and need it for special circumstances, such as with gibs.
// $illumposition 0 0 28.688

$cdmaterials "models\props_trainstation\"

// This list shows the VMT file names used in the SMD files.
// "Trainstation_Post001.vmt"

// Clipping box or view bounding box.
$cbox 0 0 0 0 0 0

// Bounding box or hull. Used for collision with a world object.
$bbox -3.281 -3.281 -0.281 3.281 3.281 57.375

// Hitbox info. Used for damage-based collision.
// The hitbox info below was automatically generated when compiled because no hitbox info was provided.
// $hboxset "default"
// $hbox 0 "static_prop" -3.124998 -3.125 0 3.125002 3.125 57.375 ""


$sequence "idle" "trainstation_post001.smd"

$collisionmodel "trainstation_post001_physics.smd"
{
	$mass 500
	$inertia 1
	$damping 0
	$rotdamping 0
	$rootbone " "
	$concave

}
