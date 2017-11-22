// Created by Crowbar 0.49.0.0

$modelname "props_trainstation/trainstation_column001.mdl"

$staticprop

$bodygroup "Body"
{
	studio "trainstation_column001_reference.smd"
}

$lod 35
{
	replacemodel "trainstation_column001_reference.smd" "trainstation_column001_reference_lod1.smd"
}

$surfaceprop "metal"

$contents "solid"

// Only set this if you know what it does, and need it for special circumstances, such as with gibs.
// $illumposition 0 0 128

$cdmaterials "models\props_trainstation\"

// This list shows the VMT file names used in the SMD files.
// "Trainstation_Column001.vmt"

// Clipping box or view bounding box.
$cbox 0 0 0 0 0 0

// Bounding box or hull. Used for collision with a world object.
$bbox -10.281 -10.281 -0.281 10.281 10.281 256.281

// Hitbox info. Used for damage-based collision.
// The hitbox info below was automatically generated when compiled because no hitbox info was provided.
// $hboxset "default"
// $hbox 0 "static_prop" -10 -10 0 10.000009 10 256 ""


$sequence "idle" {
	"trainstation_column001_anims\idle.smd"
	fadein 0.2
	fadeout 0.2
	fps 30
}

$collisionmodel "trainstation_column001_physics.smd"
{
	$mass 500
	$inertia 1
	$damping 0
	$rotdamping 1
	$rootbone " "
	$concave

}
