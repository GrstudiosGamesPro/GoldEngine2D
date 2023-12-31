#include "InspectorComponents.h"
#include "ComponentUIDrawer.h"



void InspectorComponents::update(Entity* ownerActor) {
	ComponentUIDrawer::updateUI		<SpriteComponent>(ownerActor, 0, new CSprite());
	ComponentUIDrawer::updateUI		<ScriptCompiler>(ownerActor, 1, new CScript());
	ComponentUIDrawer::updateUI		<AudioSource>(ownerActor, 2, new CAudio());
	ComponentUIDrawer::updateUI		<RigidBody>(ownerActor, 3, new CRigidBody());
	ComponentUIDrawer::updateUI     <Animator2D>(ownerActor, 4, new CAnimator());
	ComponentUIDrawer::updateUI     <TileMapComponent>(ownerActor, 5, new CTileMap());
	ComponentUIDrawer::updateUI     <MaterialComponent>(ownerActor, 6, new CMaterial());
	ComponentUIDrawer::updateUI     <Canvas>(ownerActor, 7, new CCanvas());
	//ComponentUIDrawer::updateUI     <Skybox>					(ownerActor, 4, new CSkybox				()			);

	//ComponentUIDrawer::updateUI		<Rigidbody3d>			    (ownerActor, 4, new CRigidbody3D		()			);
	//ComponentUIDrawer::updateUI		<BoxCollider>			    (ownerActor, 5, new CBoxCollider3D		()			);
}